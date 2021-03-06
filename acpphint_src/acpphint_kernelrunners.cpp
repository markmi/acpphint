//
//  acpphint_kernelrunners.cpp
//  acpphint (a C++ variation on the old HINT benchmark)
//
//  Copyright (c) 2015-2020 Mark Millard
//  Copyright (C) 1994 by Iowa State University Research Foundation, Inc.
//
//  Note: Any acpphint*.{h,cpp} code or makefile code
//        that is insufficiently original comes under
//        the Iowa State University Research Foundation,
//        Inc. copyright.
//
//        This does not apply to cpp_*.{h,cpp} or
//        sys_*.{h,cpp} code: that is strictly mine.
//
//  This program is free software; you can redistribute it
//  and/or modify it under the terms of the GNU General
//  version 2 Public License as published by the Free
//  Software Foundation.
//
//  This program is distributed in the hope that it will
//  be useful, but WITHOUT ANY WARRANTY; without even the
//  implied warranty of MERCHANTABILITY or FITNESS FOR A
//  PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General
//  Public License version 2 along with this program; if
//  not, write to:
//
//  Free Software Foundation, Inc.
//  51 Franklin Street, Fifth Floor,
//  Boston, MA  02110-1301, USA.
//

//
//  Started by Mark Millard on 2015-04-28.
//
//  See the makefile.* files for examples of how to build.
//

#include "acpphint_kernelrunners.h"
#include "sys_cpubinding.h" // RestrictThreadToCpu

#include <stdexcept>    // runtime_error

#include <future>       // future, async, launch::async

#include <algorithm>    // std::nth_element

#include <new>          // bad_alloc

//#include <system_error> // system_error

template<typename DSIZE, typename ISIZE>
using TrialCount = typename std::vector<KernelRunnerResults<DSIZE,ISIZE>>::size_type;

template<typename DSIZE, typename ISIZE>
TrialCount<DSIZE,ISIZE> constexpr NTRIAL{21u}; // Edit as needed.

long double const approx_answer_floating_form
                {0.386'294'361'119'890'618'834'464'242'916'531'361'510'00L};
                // Unsure if ends up on the low vs. the high side,
                // or by how much.

template<typename DSIZE, typename ISIZE>
static auto serial_kernel_runner
                        ( ClkInfo                           const&  clock_info
                        , LapsCount                         const   laps
                        , ISIZE                             const   memry
                        , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
                        ) -> std::vector<KernelRunnerResults<DSIZE,ISIZE>>
{
    if (1u!=ki.nproc)
    {
        throw std::runtime_error("SerialKernelRunner used for 1!=nproc");
    }
    
    std::vector<KernelRunnerResults<DSIZE,ISIZE>>   krrs{};

    try
    {
        // Potentially: restrict the allocation(s) to a NUMA memory domain:
        auto alloc_vect= std::async
                    ( std::launch::async
                    , [memry]()
                        {
                            RestrictThreadToCpu(0u,1u); // if built for such
                            return KernelVectors<DSIZE,ISIZE>{memry};
                        }
                    );
        KernelVectors<DSIZE,ISIZE> kvs{alloc_vect.get()};

        krrs.reserve(NTRIAL<DSIZE,ISIZE>);

        // bad_alloc not expected after the kvs construction.

        auto sequential= std::async // async: used for potential cpu lock down
                ( std::launch::async
                , [&clock_info,laps,&ki,&kvs,&krrs]()
                    {
                        RestrictThreadToCpu(0u,1u); // if built for such
                        
                        for ( auto trials_left{NTRIAL<DSIZE,ISIZE>}
                            ; 0u<trials_left
                            ; --trials_left
                            )
                        {
                            auto const start{clock_info.Now()};
                
                            auto result{Kernel(0u, ki, kvs)};   // 0u for iproc
                            for ( auto lap_count_down{laps}
                                ; 1<lap_count_down
                                ; --lap_count_down
                                )
                                result= Kernel(0u, ki, kvs);    // 0u for iproc
                
                            auto const finish{clock_info.Now()};
                
                            auto const measured_laps_duration{finish-start};
                            auto const measurement_overhead
                                            {clock_info.DurationOverhead()};
                
                            if  (measured_laps_duration<measurement_overhead)
                                throw std::runtime_error
                                            ( "measured duration less than "
                                              "previously-measured overhead"
                                            );
                
                            auto const laps_duration
                                        {  measured_laps_duration
                                         - measurement_overhead
                                        };
                            auto const sec_per_lap
                                        {  typename KernelRunnerResults
                                                                <DSIZE,ISIZE>
                                             ::SecFloatingForm(laps_duration)
                                         / laps
                                        };
                
                            krrs.emplace_back   ( result
                                                , laps_duration
                                                , sec_per_lap
                                                , kvs.VectorsTotalBytes()
                                                );
                        }
                    }
                );
        sequential.wait();
    }
    catch(std::bad_alloc const& e)
    {
        // Empty krrs indicates NOMEM up front.
    }

    return krrs;
} // SerialKernelRunner

template<typename DSIZE, typename ISIZE>
static auto parallel_kernel_runner
                        ( ClkInfo                           const&  clock_info
                        , LapsCount                         const   laps
                        , ISIZE                             const   memry // per thread
                        , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
                        ) -> std::vector<KernelRunnerResults<DSIZE,ISIZE>>
{
    if (ki.nproc<1u)
    {
        throw std::runtime_error("ParallelKernelRunner used for nproc<1");
    }
    
    std::vector<KernelRunnerResults<DSIZE,ISIZE>>   krrs{};

    try
    {
        std::vector<KernelVectors<DSIZE,ISIZE>>     threads_kvs{};
    
        for (HwConcurrencyCount thread{0u}; thread<ki.nproc; ++thread)
        {
            // Distribute allocations over the potential NUMA memory domains:
            auto alloc_thread= std::async
                            ( std::launch::async
                            , [thread,memry,&ki,&threads_kvs]()
                                {
                                    RestrictThreadToCpu(thread,ki.nproc);
                                                        // if built for such
                                    threads_kvs.emplace_back
                                          (KernelVectors<DSIZE,ISIZE>{memry});
                                }
                            );
            alloc_thread.wait();
        }

        krrs.reserve(NTRIAL<DSIZE,ISIZE>);

        // bad_alloc not expected after the KernelVectors constructions.
    
        std::vector<std::future<KernelResults<DSIZE,ISIZE>>>
                                                        in_parallel(ki.nproc);
        
        for ( auto trials_left{NTRIAL<DSIZE,ISIZE>} // 0u<NTRIAL required.
            ; 0u<trials_left
            ; --trials_left
            )
        {
            auto const start{clock_info.Now()};
    
            KernelResults<DSIZE,ISIZE> result{};
            for ( auto lap_count_down{laps}
                ; 0<lap_count_down
                ; --lap_count_down
                )
            {
                for(HwConcurrencyCount thread{0u}; thread<ki.nproc; ++thread)
                {
                    in_parallel[thread]= std::async
                        ( std::launch::async
                        , [thread,&ki,&threads_kvs]()
                            {
                                RestrictThreadToCpu(thread,ki.nproc);
                                                        // if built for such
                                return Kernel<DSIZE,ISIZE>
                                                        ( thread
                                                        , ki
                                                        , threads_kvs[thread]
                                                        );
                            }
                        );
                }
                KernelResults<DSIZE,ISIZE> lap_result{};
    
                for(auto& thread : in_parallel)
                {
                    lap_result.Merge(thread.get());
                }
                
                result= lap_result; // Using the last lap's result
            }
        
            auto const finish{clock_info.Now()};
            
            auto const measured_laps_duration{finish-start};
            auto const measurement_overhead{clock_info.DurationOverhead()};
            
            if  (measured_laps_duration<measurement_overhead)
            {
                throw std::runtime_error
                ("measured duration less than previously-measured overhead");
            }
            
            auto const laps_duration{measured_laps_duration-measurement_overhead};
            auto const sec_per_lap  {   typename KernelRunnerResults<DSIZE,ISIZE>
                                        ::SecFloatingForm(laps_duration) / laps
                                    };
                                    
            std::size_t vectors_total_bytes{0u};
            for (auto const& kvs : threads_kvs)
            {
                vectors_total_bytes+= kvs.VectorsTotalBytes();
            }
        
            krrs.emplace_back   ( result
                                , laps_duration
                                , sec_per_lap
                                , vectors_total_bytes
                                );
        }
    }
    catch(std::bad_alloc const& e)
    {
        // Empty krrs indicates NOMEM.
    }
    
    return krrs;
}

template<typename DSIZE, typename ISIZE>
auto KernelRunner   ( ClkInfo                           const&  clock_info
                    , LapsCount                         const   laps
                    , ISIZE                             const   memry
                    , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
                    ) -> KernelRunnerResults<DSIZE,ISIZE>
{
    static_assert
        ( 0x1u == (NTRIAL<DSIZE,ISIZE>&0x1u)
        , "NTRIAL not odd (so no exact median position after sorting)"
        );
    
    if (laps<=0)
    {
        throw std::runtime_error("laps was not positive");
    }
    
    if (memry<2)
    {
        throw std::runtime_error("memry<2");
    }

    std::vector<KernelRunnerResults<DSIZE,ISIZE>> krrs{};
    
    if (1u==ki.nproc)
    {
        krrs= serial_kernel_runner<DSIZE,ISIZE>(clock_info,laps,memry,ki);
    }
    else
    {
        krrs= parallel_kernel_runner<DSIZE,ISIZE>(clock_info,laps,memry,ki);
    }
    
    if (krrs.empty()) // Upfront NOMEM problem.
    {
        return KernelRunnerResults<DSIZE,ISIZE>
                                    {KernelResults<DSIZE,ISIZE>::EFlag::NOMEM};
    }

    auto median_duration_pos{krrs.begin()+krrs.size()/2};
                                            // For odd NTRIAL [==krrs.size()]

    std::nth_element( krrs.begin(), median_duration_pos, krrs.end()
                    , [](auto const& left, auto const& right)
                        { return  left.median_mean_sec_per_lap
                                < right.median_mean_sec_per_lap;
                        }
                    );
    
    if  ( median_duration_pos->kernel_result.result_bounds.HI
        <= median_duration_pos->kernel_result.result_bounds.LO
        )
    {
        throw std::runtime_error("result_bounds.HI <= result_bounds.LO");
    }
                    
    auto approx_answer_on_LO_HI_scale
                                {(approx_answer_floating_form*ki.scx)*ki.scy};
            // Unsure if this under or over estimates relative to higher
            // accuracy.
    
    // Crude check on the result's LO
    if  ( 1.001L*approx_answer_on_LO_HI_scale
        < median_duration_pos->kernel_result.result_bounds.LO
        )
    {
        throw std::runtime_error("result_bounds.LO appearently to large");
    }
     
    // Crude check on the result's HI
    if  ( median_duration_pos->kernel_result.result_bounds.HI
        < 0.999L*approx_answer_on_LO_HI_scale
        )
    {
        throw std::runtime_error("result_bounds.HI appearently to small");
    }
   
    return *median_duration_pos;
}


// Edit as needed to add more alternatives (or disable some):

// DSIZE=short:

template
auto KernelRunner<short,short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<short,short>      const   ki
                    ) -> KernelRunnerResults<short,short>;

template
auto KernelRunner<short,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<short,unsigned short>;

// DSIZE=unsigned short:

template
auto KernelRunner<unsigned short,short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,short>;

template
auto KernelRunner<unsigned short,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,unsigned short>;

// DSIZE=unsigned int:

template
auto KernelRunner<unsigned int,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<unsigned int,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned int,unsigned int>;

// DSIZE=unsigned long:

template
auto KernelRunner<unsigned long,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<unsigned long,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned long,unsigned long>;

// DSIZE=unsigned long long:

template
auto KernelRunner<unsigned long long,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs   < unsigned long long
                                            , unsigned long long
                                            >               const   ki
                    ) -> KernelRunnerResults< unsigned long long
                                            , unsigned long long
                                            >;

// DSIZE=float:

template
auto KernelRunner<float,short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<float,short>      const   ki
                    ) -> KernelRunnerResults<float,short>;

template
auto KernelRunner<float,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned short>;

template
auto KernelRunner<float,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned int>;

// DSIZE=double:
            
template
auto KernelRunner<double,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned int>;

template
auto KernelRunner<double,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long>;

template
auto KernelRunner<double,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long long>;

// DSIZE=long double:

template
auto KernelRunner<long double,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long>;

template
auto KernelRunner<long double,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<long double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long long>;


char copyright_and_license_for_acpphint_kernelrunners[]
{
    "Context for this Copyright: acpphint_kernelrunners\n"
    "\n"
    "Copyright (c) 2015-2020 Mark Millard\n"
    "Copyright (C) 1994 by Iowa State University Research Foundation, Inc.\n"
    "\n"
    "Note: Any acpphint*.{h,cpp} code  or makefile code\n"
    "      that is insufficiently original comes under\n"
    "      the Iowa State University Research Foundation,\n"
    "      Inc. copyright.\n"
    "\n"
    "      This does not apply to cpp_*.{h,cpp} code or\n"
    "      sys_*.{h,cpp} code: that is strictly mine.\n"
    "\n"
    "This program is free software; you can redistribute it\n"
    "and/or modify it under the terms of the GNU General\n"
    "version 2 Public License as published by the Free\n"
    "Software Foundation.\n"
    "\n"
    "This program is distributed in the hope that it will\n"
    "be useful, but WITHOUT ANY WARRANTY; without even the\n"
    "implied warranty of MERCHANTABILITY or FITNESS FOR A\n"
    "PARTICULAR PURPOSE.  See the GNU General Public\n"
    "License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General\n"
    "Public License version 2 along with this program; if\n"
    "not, write to:\n"
    "\n"
    "Free Software Foundation, Inc.\n"
    "51 Franklin Street, Fifth Floor,\n"
    "Boston, MA  02110-1301, USA.\n"
};
