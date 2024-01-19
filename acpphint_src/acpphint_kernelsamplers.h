//
//  acpphint_kernelsamplers.h
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

#ifndef acpphint_kernelsamplers_guard
#define acpphint_kernelsamplers_guard

#include <string>                    // for std::basic_string, std::string
#include <vector>                    // for std::vector
#include "acpphint_kernelrunners.h"  // for KernelRunnerResults, TIME_PARALLEL_THREAD_CREATION_TOO
class ClkInfo;
template <typename DSIZE, typename ISIZE> struct PrimaryKernelInputs;

template<typename DSIZE, typename ISIZE>
struct KernelSamplerResults
{
    using FloatingType              = long double;
    using QualityImprovementPerSec  = FloatingType;

    // Note: quality improvement == run_result.median_mean_sec_per_lap*quips
    //       (up to floating point based approximations and such)

    KernelRunnerResults<DSIZE,ISIZE>    run_result{};

    QualityImprovementPerSec            quips{};

    ISIZE                               n{}; // size count, not an index

    std::string                         how_stopped_notes{};

        // Notes (working from the goal quips to what contributes and how) . . .

        // Rational quality_improvement_per_sec (a.k.a. QUIPS or qp) would be:
        // (for t==run_result.median_mean_sec_per_lap)
        //
        //      quips
        //  ==
        //      quality_improvement/t
        //  == { See "Where . . ." below for the substitution's justification. }
        //      (delq + 1/(HI-LO)) / t
        //  ==
        //      delq / t + 1/(HI-LO) / t

        // Where the following leads to delq's value, eventually . . .
        //
        // Note explaining dmax+1 use (and, indirectly, delq use):
        //      grid size == scx*scy == dmax+1
        //      so scx*(scy-1)+(scx-1) == dmax.
        //
        //      Using dmax helps avoid running out of bits at
        //      DSIZE or ISIZE bit limitations.
        // End note.
        //
        // Accurate change in quality would be (mathematical Rational numbers):
        //
        //      quality_improvement
        //  == { See "And . . ." below for the substitution's justification. }
        //      (dmax+1)/(HI-LO) - (dmax+1)/((dmax+1)-0)
        //  ==
        //      (dmax+1)/(HI-LO) - 1/1
        //  ==
        //      (dmax/(HI-LO) - 1/1) + 1/(HI-LO)
        //  ==
        //      delq + 1/(HI-LO)
        //
        // where delq == dmax/(HI-LO) - 1/1
        // (presuming mathematical Rational arithmetic anyway)

        // And "quality" comes from . . .
        //
        // Rationals HI/(dmax+1) and LO/(dmax+1)
        // form the bounds on 2*ln(2) - 1 for the scx by scy grid used.
        //
        // The accuracy (the value range width) would be:
        //
        //      accuracy
        //  ==
        //      HI/(dmax+1) - LO/(dmax+1)
        //  ==
        //      (HI-LO) / (dmax+1)
        //
        // The original accuracy is then: (dmax+1)/(dmax+1) - 0/(dmax+1) == 1/1.
        //
        // Quality is defined for the HINT context as the reciprical of the
        // accuracy. So:
        //
        //      quality
        // ==
        //      (dmax+1) / (HI-LO)
        //
        // For the initial accuracy: (dmax+1)/((dmax+1)-0) == 1/1.
        //
        // Thus the change in quality from the orignal accuracy is:
        //
        //      quality_improvement
        //  ==
        //      (dmax+1)/(HI-LO) - (dmax+1)/((dmax+1)-0)
        //  ==
        //      (dmax+1)/(HI-LO) - 1/1

        // All this is applied to the problem of numerically approximating the
        // relationship
        //
        // Integral from 0 to 1 of (1-x)/(1+x) dx
        //
        // (A "Real Number" name for the arbitrary-accuracy calculation
        // relationship being: 2*ln(2) - 1.)
        //
        // For a given "grid size" this is done via breaking up the containting
        // (1-0) by (1-0) box into an i by j cell grid (fixed cell shape so
        // fixed dx and dy across the cells) and using two counts of cells:
        //
        // A) The LO count: all the cells strictly <= (1/x)/(1+x) everywhere.
        //
        // B) The HI count: all the cells but those strictly > (1/x)/(1+x)
        //                  everywhere. (So it spans the LO count.)
        //
        // (Did I put equality in correctly?)
        //

    KernelSamplerResults( KernelRunnerResults<DSIZE,ISIZE>  const& results
                        , QualityImprovementPerSec                 qi_per_sec
                        , ISIZE                                    n_used
                        )
                    : run_result{results}
                    , quips     {qi_per_sec}
                    , n         {n_used}
    {}
}; // KernelSamplerResults

template<typename DSIZE, typename ISIZE>
using KernelSamplerResultsVect // sorted by strictly increasing n
        = std::vector<KernelSamplerResults<DSIZE,ISIZE>>;

template<typename DSIZE, typename ISIZE, bool want_parallel_thread_creation_time_included>
auto KernelSampler  ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<DSIZE,ISIZE>      const&  ki
                    ) -> KernelSamplerResultsVect<DSIZE,ISIZE>;
#endif // acpphint_kernelsamplers_guard
