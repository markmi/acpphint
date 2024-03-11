//  acpphint_kernelrunners.cpp
//  acpphint (a C++ variation on the old HINT benchmark)
//
//  Copyright (c) 2015-2024 Mark Millard
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

#include "acpphint_kernels.h"      // for PrimaryKernelInputs, KernelVectors
#include "acpphint_kernelrunners.h"
#include <algorithm>              // for std::nth_element
#include <climits>                // for ULONG_MAX, UINT_MAX, ULLONG_MAX
#include <barrier>                // for std::barrier
#include <future>                 // for std::packaged_task, std::future
#include <new>                    // for std::bad_alloc
#include <stdexcept>              // for std::runtime_error
#include <thread>                 // for std::thread
#include <vector>                 // for std::vector
#include <cstddef>                // for std::size_t
#include "cpp_clockinfo.h"        // for ClkInfo, HwConcurrencyCount
#include "sys_cpubinding.h"       // for RestrictThreadToCpu


template<typename DSIZE, typename ISIZE>
using TrialCount = typename std::vector<KernelRunnerResults<DSIZE,ISIZE>>::size_type;

template<typename DSIZE, typename ISIZE>
TrialCount<DSIZE,ISIZE> constexpr NTRIAL{21U}; // Edit as needed.

#if 201611L <= __cpp_lib_to_chars
long
#endif
double constexpr approx_answer_floating_form
                {0.386'294'361'119'890'618'834'464'242'916'531'361'510'00L};
                // Unsure if ends up on the low vs. the high side,
                // or by how much.

namespace
{
// NOLINTBEGIN(readability-identifier-length,readability-function-cognitive-complexity)
    template<typename DSIZE, typename ISIZE>
    auto serial_kernel_runner
            ( ClkInfo                           const&  clock_info
            , LapsCount                         const   laps
            , ISIZE                             const   memry
            , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
            ) -> std::vector<KernelRunnerResults<DSIZE,ISIZE>>
    {
        if (1U!=ki.nproc)
        {
            throw std::runtime_error("SerialKernelRunner used for 1!=nproc");
        }

        std::vector<KernelRunnerResults<DSIZE,ISIZE>>   krrs{};

        try
        {
            // Potentially: restrict the allocation(s) to a NUMA memory domain:

            // clang16 still can not do the template deduction itself.
            auto alloc_vect_code
            {[memry]()
                {
                    RestrictThreadToCpu(0U,1U); // if built for such
                    return KernelVectors<DSIZE,ISIZE>{memry};
                }
            };
            std::packaged_task alloc_vect_task{alloc_vect_code};
            auto alloc_vect_future{alloc_vect_task.get_future()};
            std::thread alloc_vect_task_thread{std::move(alloc_vect_task)};
            // Use std::jthread when clang allows it.

            KernelVectors<DSIZE,ISIZE> kvs{alloc_vect_future.get()};

            alloc_vect_task_thread.join();

            krrs.reserve(NTRIAL<DSIZE,ISIZE>);

            // bad_alloc not expected after the kvs construction.

            auto sequential_code
                {[&clock_info,laps,&ki,&kvs,&krrs]()
                        {
                            RestrictThreadToCpu(0U,1U); // if built for such

                            for ( auto trials_left{NTRIAL<DSIZE,ISIZE>}
                                ; 0U<trials_left
                                ; --trials_left
                                )
                            {
                                auto const start{clock_info.Now()};

                                auto result{Kernel(0U, ki, kvs)};    // 0U for iproc
                                for ( auto lap_count_down{laps}
                                    ; 1<lap_count_down
                                    ; --lap_count_down
                                    )
                                    { result= Kernel(0U, ki, kvs); } // 0U for iproc

                                auto const measured_laps_duration{clock_info.Now()-start};
                                auto const measurement_overhead
                                                {clock_info.DurationOverhead()};

                                if  (measured_laps_duration<measurement_overhead)
                                    { throw std::runtime_error
                                                ( "measured duration less than "
                                                  "previously-measured overhead"
                                                );
                                    }

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
                 };

             std::packaged_task sequential_task{sequential_code};  // used for potential cpu lock down
             auto sequential_task_future{sequential_task.get_future()};
             std::thread sequential_task_thread{std::move(sequential_task)};
             // Use std::jthread when clang allows it.
             sequential_task_future.wait();
             sequential_task_thread.join();
        }
        catch(std::bad_alloc const& e) // NOLINT(bugprone-empty-catch)
        { } // Empty krrs indicates NOMEM up front.

        return krrs;
    } // SerialKernelRunner

    template<typename DSIZE, typename ISIZE, bool want_thread_creation_time_included>
    auto parallel_kernel_runner
                            ( ClkInfo                           const&  clock_info
                            , LapsCount                         const   laps
                            , ISIZE                             const   memry // per thread
                            , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
                            ) -> std::vector<KernelRunnerResults<DSIZE,ISIZE>>
    {
        if (ki.nproc<1U)
        {
            throw std::runtime_error("ParallelKernelRunner used for nproc<1");
        }

        std::vector<KernelRunnerResults<DSIZE,ISIZE>>   krrs{};

        try
        {
            std::vector<KernelVectors<DSIZE,ISIZE>>     threads_kvs{};

            for (HwConcurrencyCount thread{0U}; thread<ki.nproc; ++thread)
            {
                // Possibly distribute allocations over the potential NUMA memory domains:
                auto memalloc_code
                    {[thread,memry,&ki,&threads_kvs]()
                        {
                             RestrictThreadToCpu(thread,ki.nproc); // if built for such
                             threads_kvs.emplace_back(KernelVectors<DSIZE,ISIZE>{memry});
                        }
                    };
                std::packaged_task memalloc_task{memalloc_code};
                auto memalloc_task_future{memalloc_task.get_future()};
                std::thread memalloc_task_thread{std::move(memalloc_task)};
                // Use std::jthread when clang allows it.
                memalloc_task_future.wait();
                memalloc_task_thread.join();
            }

            krrs.reserve(NTRIAL<DSIZE,ISIZE>);

            // bad_alloc not expected after the KernelVectors constructions.

            std::vector<std::future<KernelResults<DSIZE,ISIZE>>> parallel_tasks_future{};
            parallel_tasks_future.reserve(ki.nproc);

            std::vector<std::thread> parallel_tasks_thread{};
            // Use std::jthread when clang allows it.
            parallel_tasks_thread.reserve(ki.nproc);

            for ( auto trials_left{NTRIAL<DSIZE,ISIZE>} // 0U<NTRIAL required.
                ; 0U<trials_left
                ; --trials_left
                )
            {
                auto start{clock_info.Now()}; // Replaced if thread startup is to be untimed.

                auto replace_start_at_barrier_completion = [&start,&clock_info]() noexcept
                {
                    start= clock_info.Now();
                };
                std::barrier sync_then_set_start(ki.nproc,replace_start_at_barrier_completion);

                auto measured_laps_duration{start-start}; // zero of right type. // NOLINT(misc-redundant-expression)

                KernelResults<DSIZE,ISIZE> result{};
                for ( auto lap_count_down{laps}
                    ; 0<lap_count_down
                    ; --lap_count_down
                    )
                {
                    auto parallel_code
                        {[&ki,&threads_kvs,&sync_then_set_start](HwConcurrencyCount thread)
                            {
                                RestrictThreadToCpu(thread,ki.nproc); // if built for such

                                if (!want_thread_creation_time_included)
                                    { sync_then_set_start.arrive_and_wait(); }

                                return Kernel<DSIZE,ISIZE>( thread
                                                  , ki
                                                  , threads_kvs[thread]
                                                  );
                            }
                        };

                    for(auto threads_left{ki.nproc}; 0U<threads_left; --threads_left)
                    {
                        std::packaged_task parallel_task{parallel_code};
                        auto parallel_task_future{parallel_task.get_future()};

                        parallel_tasks_future.emplace_back(std::move(parallel_task_future));
                        parallel_tasks_thread.emplace_back(std::move(parallel_task),ki.nproc-threads_left);
                    }
                    KernelResults<DSIZE,ISIZE> lap_result{};

                    for(auto& thread_result : parallel_tasks_future)
                    {
                        lap_result.Merge(thread_result.get());
                    }

                    result= lap_result; // Using the last lap's result

                    if constexpr (!want_thread_creation_time_included)
                        { measured_laps_duration+= clock_info.Now()-start; }

                    for(auto& thread : parallel_tasks_thread)
                    {
                        thread.join();
                    }

                    parallel_tasks_thread.clear();
                    parallel_tasks_future.clear();
                }

                if constexpr (want_thread_creation_time_included)
                    { measured_laps_duration+= clock_info.Now()-start; }

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

                std::size_t vectors_total_bytes{0U};
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
        catch(std::bad_alloc const& e) // NOLINT(bugprone-empty-catch)
        { } // Empty krrs indicates NOMEM.

        return krrs;
    }
// NOLINTEND(readability-identifier-length,readability-function-cognitive-complexity)
};

template<typename DSIZE, typename ISIZE, bool want_parallel_thread_creation_time_included>
auto KernelRunner   ( ClkInfo                           const&  clock_info
                    , LapsCount                         const   laps
                    , ISIZE                             const   memry
                    , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki // NOLINT(readability-identifier-length)
                    ) -> KernelRunnerResults<DSIZE,ISIZE>
{
    static_assert
        ( 0x1U == (NTRIAL<DSIZE,ISIZE>&0x1U)
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

    if (1U==ki.nproc)
    {
        // Implicitly/always want_thread_creation_time_included==false :
        krrs= serial_kernel_runner<DSIZE,ISIZE>(clock_info,laps,memry,ki);
    }
    else
    {
        krrs= parallel_kernel_runner<DSIZE,ISIZE,want_parallel_thread_creation_time_included>
                  (clock_info,laps,memry,ki);
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
    constexpr auto upperbound_rescale{1.001L};
    if  ( upperbound_rescale*approx_answer_on_LO_HI_scale
        < median_duration_pos->kernel_result.result_bounds.LO
        )
    {
        throw std::runtime_error("result_bounds.LO appearently to large");
    }

    // Crude check on the result's HI
    constexpr auto lowerbound_rescale{0.999L};
    if  ( median_duration_pos->kernel_result.result_bounds.HI
        < lowerbound_rescale*approx_answer_on_LO_HI_scale
        )
    {
        throw std::runtime_error("result_bounds.HI appearently to small");
    }

    return *median_duration_pos;
}


// Edit as needed to add more alternatives (or disable some):

// DSIZE=short:

template
auto KernelRunner<short,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<short,short>      const   ki
                    ) -> KernelRunnerResults<short,short>;

template
auto KernelRunner<short,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<short,short>      const   ki
                    ) -> KernelRunnerResults<short,short>;

template
auto KernelRunner<short,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<short,unsigned short>;

template
auto KernelRunner<short,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<short,unsigned short>;

// DSIZE=unsigned short:

template
auto KernelRunner<unsigned short,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,short>;

template
auto KernelRunner<unsigned short,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,short>;

template
auto KernelRunner<unsigned short,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,unsigned short>;

template
auto KernelRunner<unsigned short,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned short,unsigned short>;

#if ULONG_MAX == UINT_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned int:

template
auto KernelRunner<unsigned int,unsigned int,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<unsigned int,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned int,unsigned int>;

template
auto KernelRunner<unsigned int,unsigned int,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<unsigned int,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned int,unsigned int>;
#endif

// DSIZE=unsigned long: // Always included

template
auto KernelRunner<unsigned long,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<unsigned long,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned long,unsigned long>;

template
auto KernelRunner<unsigned long,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<unsigned long,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<unsigned long,unsigned long>;

#if ULONG_MAX == ULLONG_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned long long:      

template 
auto KernelRunner<unsigned long long,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs   < unsigned long long
                                            , unsigned long long
                                            >               const   ki
                    ) -> KernelRunnerResults< unsigned long long
                                            , unsigned long long
                                            >;

template 
auto KernelRunner<unsigned long long,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs   < unsigned long long
                                            , unsigned long long
                                            >               const   ki
                    ) -> KernelRunnerResults< unsigned long long
                                            , unsigned long long
                                            >;
#endif

// DSIZE=float:

template
auto KernelRunner<float,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<float,short>      const   ki
                    ) -> KernelRunnerResults<float,short>;

template
auto KernelRunner<float,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , short                                 const   memry
                    , PrimaryKernelInputs<float,short>      const   ki
                    ) -> KernelRunnerResults<float,short>;

template
auto KernelRunner<float,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned short>;

template
auto KernelRunner<float,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned short                        const   memry
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned short>;

template
auto KernelRunner<float,unsigned int,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned int>;

template
auto KernelRunner<float,unsigned int,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<float,unsigned int>;

#ifdef DSIZE_ALL_ISIZE_ALL
// DSIZE=double:

template
auto KernelRunner<double,unsigned int,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned int>;

template
auto KernelRunner<double,unsigned int,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned int                          const   memry
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned int>;

template
auto KernelRunner<double,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long>;

template
auto KernelRunner<double,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long>;

template
auto KernelRunner<double,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long long>;

template
auto KernelRunner<double,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<double,unsigned long long>;

// DSIZE=long double:

template
auto KernelRunner<long double,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long>;

template
auto KernelRunner<long double,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long                         const   memry
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long>;

template
auto KernelRunner<long double,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<long double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long long>;

template
auto KernelRunner<long double,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , LapsCount                             const   laps
                    , unsigned long long                    const   memry
                    , PrimaryKernelInputs<long double,unsigned long long>
                                                            const   ki
                    ) -> KernelRunnerResults<long double,unsigned long long>;
#endif


extern "C" char const copyright_and_license_for_acpphint_kernelrunners[]
{
    "Context for this Copyright: acpphint_kernelrunners\n"
    "\n"
    "Copyright (c) 2015-2024 Mark Millard\n"
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
