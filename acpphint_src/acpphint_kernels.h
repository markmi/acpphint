//
//  acpphint_kernels.h
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
//  Started by Mark Millard on 2015-04-28.
//
//  See the makefile.* files for examples of how to build.
//

#ifndef acpphint_kernels_guard
#define acpphint_kernels_guard

#include <algorithm>  // for std::max
#include <cstddef>    // for size_t, std::size_t
#include <limits>     // for std::numeric_limits
#include <vector>     // for std::vector

using HwConcurrencyCount = unsigned int; // Matching C++

template<typename ISIZE>
constexpr auto NCHUNK(HwConcurrencyCount nproc) -> ISIZE
{
    return (1U==nproc) ?ISIZE{1U} :ISIZE{4U};
    // for 1==nproc: 1==NCHUNK (not adjustable!)
    // For 1<nproc case:
    // Number of chunks for scatter decomposition. Larger numbers
    // increase time to first result (latency) but sample domain more
    // evenly, minimum 2U.
}

template<typename ISIZE>
constexpr auto NMIN(HwConcurrencyCount nproc) -> ISIZE
{
    ISIZE nchunk{NCHUNK<ISIZE>(nproc)};
    return (ISIZE{1U}==nchunk) ?ISIZE{2U} :nchunk;
}

template<typename DSIZE,typename ISIZE>
struct PrimaryKernelInputs
{   // grid size == scx*scy == dmax+1 so scx*(scy-1)+(scx-1) == dmax.
    // Using dmax directly in places can help avoid
    // overflow/truncation/rounding.

    HwConcurrencyCount  nproc; // # threads, 1U for serial.

    DSIZE               scx;
    DSIZE               scy;

    DSIZE               dmax;   // == scx*scy - 1
                                            // helps avoid overflow
                                            // or other such issues.

    DSIZE               initial_dx; // scx/NCHUNK(nproc)/nproc

    PrimaryKernelInputs() = delete;

    PrimaryKernelInputs(HwConcurrencyCount const threads);
}; // PrimaryKernelInputs

template<typename DSIZE, typename ISIZE>
struct KernelVectors
{
    struct RECT
    {
        // grid size == scx*scy == dmax+1 so scx*(scy-1)+(scx-1) == dmax.
        // Using dmax directly can help avoid overflow/truncation/rounding.

        DSIZE   ahi, // Upper bound via rectangle areas for scx by scy breakdown
                alo, // Lower bound via rectangle areas for scx by scy breakdown
                dx,  // Interval widths, SEE NOTES BELOW.
                flh, // Function values of left  coordinates, high
                fll, // Function values of left  coordinates, low
                frh, // Function values of right coordinates, high
                frl, // Function values of right coordinates, low
                xl,  // Left  x-coordinates of subintervals
                xr;  // Right x-coordinates of subintervals

        // dx notes:
        //
        // The scatter decomposition starts with dx == scx/NCHUNK/nproc where
        // things are set up so that dx is then a power of 2. (So scx
        // constrained relative to NCHUNK*nproc.)
        //
        // Only when NCHUNK*nproc == 1 would there be a stage when an explicit,
        // single cell spans conceptually spans the whole scx*scy area --and so
        // could have overflow/truncation/rounding problems. (dmax is used
        // instead: scx*(scy-1) + (scx-1) == scx*scy-1 without overflow.)
        //
        // But the minimum NCHUNK requirement being 2 (possibly set higher)
        // means that no single cell spans scx*sxy for 1<nproc.
        //
        // The QUality ImProvement measure is (still) always based on comparing
        // to the conceptual initial approximation bounds (for this code always
        // implicit)
        //
        //      HI/(dmax+1) == (dmax+1)/(dmax+1) and LO/(dmax+1) == 0/(dmax+1)
        //
        // for high and low bounds on . . .
        //
        // Integral from 0 to 1 of (1-x)/(1+x) dx
        //
        // (A "Real Number" name for the arbitrary-accuracy calculation
        // relationship is: 2*ln(2) - 1.)
    }; // RECT

    using RECTVector = std::vector<RECT>;    
    using ErrsVector = std::vector<DSIZE>;

    using IxesVector = std::vector<ISIZE>;
            // Holds indexes into rect and errs.

    RECTVector  rect;
    ErrsVector  errs;
    IxesVector  ixes; // indexes into rect and errs.

    KernelVectors() = delete;

    KernelVectors(ISIZE memry) : rect(memry), errs(memry*2), ixes(memry*2)
    {}

    auto VectorsTotalBytes() const -> auto // Counts capacity overhead
    { return  sizeof(KernelVectors)
            + rect.size()*sizeof(RECT) 
            + errs.size()*sizeof(DSIZE) 
            + ixes.size()*sizeof(ISIZE);
    }

    static auto VectorsUsedBytes  
        ( std::size_t rect_used
        , std::size_t errs_ixes_used_each
        ) -> auto // May be < .size() allocations.
    {
        return    sizeof(KernelVectors)
                + rect_used*sizeof(RECT)
                + errs_ixes_used_each*sizeof(DSIZE) 
                + errs_ixes_used_each*sizeof(ISIZE);
    }

private:
    static void static_indexing_types_match_check()
    { // Valdiating HINT's code vs. intent may be required if any fail.
        static_assert
        ( std::numeric_limits<ISIZE>::is_integer
        , "ISIZE has numeric_limits<ISIZE>::is_integer false"
        );
        static_assert
        (   std::numeric_limits<ISIZE>::max()
         <= std::numeric_limits<typename RECTVector::size_type>::max()
        , "ISIZE would reach outside rect's size_type"
        );
        static_assert
        (   std::numeric_limits<ISIZE>::max()
         <= std::numeric_limits<typename ErrsVector::size_type>::max()
        , "ISIZE would reach outside errs' size_type"
        );
        static_assert
        (   std::numeric_limits<ISIZE>::max()
         <= std::numeric_limits<typename IxesVector::size_type>::max()
        , "ISIZE would reach outside ixes' size_type"
        );
    } // static_indexing_types_match_check
}; // KernelVectors

template<typename DSIZE, typename ISIZE>
struct KernelResults
{
    enum struct EFlag : int { STILL_UNKNOWN, normal, DISCRET, NOMEM };
                // EFlag::STILL_UNKNOWN was not in the original HINT
                // variations.
                //
                // EFlag::NOMEM is a creation/reservation failure that does
                // not happen directly in the kernel but in a KernelRunner
                // before the kernel is used. The kernel itself avoids
                // doing dynamic memory allocation.
                //
                // EFlag::DISCRET is a computation limitation at the
                // scale of the breakdown (scx*scy grid).

    struct IntegrationResultBounds
    {
        // Taken from one of the trials . . .

        // grid size == scx*scy == dmax+1 so scx*(scy-1)+(scx-1) == dmax.
        // Using dmax directly can help avoid overflow/truncation/rounding.

        DSIZE HI;
        DSIZE LO;

        // When the HI, LO pair is for "over all .dx intervals":
        //
        //      LO/(dmax+1) < 2*ln(2) - 1 < HI/(dmax+1)
        //
        // where dmax+1 == scx*scy.
        //
        // (A "Real Number" name for the arbitrary-accuracy calculation
        // relationship being: 2*ln(2) - 1.)
        //
        // Note that approx_answer_floating_form is a specific approximation
        // for 2*ln(2) - 1 and is used to cross check the calculations.
    }; // IntegrationBounds

    IntegrationResultBounds result_bounds{};
    EFlag                   eflag{EFlag::STILL_UNKNOWN};

    ISIZE                   RECT_maxjo{};       // Memory use tracking
    ISIZE                   ixes_errs_maxiq{};  // Memory use tracking
    //
    // Historical pthread HINT comparison:
    //
    // The original pthread HINT sources do not explicitly
    // track the above 2 memory usage items, or the below 2.

    // The following are element counts,  not Bytes:
    std::size_t             RECT_total_used{};  // Memory use tracking
    std::size_t             ixes_errs_used_each{};  // Mem use tracking

    KernelResults(EFlag upfront_problem) : eflag{upfront_problem}
    {} // No more activity intended, likely NOMEM context.

    KernelResults(ISIZE pos) // Do not use with Merge: serial kernel use
                                : result_bounds{1U,0U} // Replaced later
                                , RECT_maxjo{pos}
                                , ixes_errs_maxiq{pos}
                                , RECT_total_used(pos)
                                , ixes_errs_used_each(pos)
    {
        ++RECT_total_used;      // Convert last position to used count
        ++ixes_errs_used_each;  // Convert last position to used count
    }

    KernelResults() {} // Used to start combining thread info: Use with Merge

    void Merge(KernelResults const& chunk_results) // For combining thread info
    {
        result_bounds.HI+= chunk_results.result_bounds.HI;
        result_bounds.LO+= chunk_results.result_bounds.LO;

        eflag=              std::max(eflag,chunk_results.eflag);

        RECT_maxjo=         std::max(RECT_maxjo,chunk_results.RECT_maxjo);
        ixes_errs_maxiq=    std::max( ixes_errs_maxiq
                                    , chunk_results.ixes_errs_maxiq
                                    );

        RECT_total_used+=       chunk_results.RECT_total_used;
        ixes_errs_used_each+=   chunk_results.ixes_errs_used_each; 
    }
}; // KernelResults

template<typename DSIZE, typename ISIZE>
auto Kernel ( HwConcurrencyCount                const   iproc
            , PrimaryKernelInputs<DSIZE,ISIZE>  const&  ki
            , KernelVectors<DSIZE,ISIZE>&               kv
            ) -> KernelResults<DSIZE,ISIZE>;
#endif // acpphint_kernels_guard
