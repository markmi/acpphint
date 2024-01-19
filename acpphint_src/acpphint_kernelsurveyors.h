//
//  acpphint_kernelsurveyors.h
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

#ifndef acpphint_kernelsurveyors_guard
#define acpphint_kernelsurveyors_guard

#include "acpphint_kernelrunners.h"  // for KernelRunnerResults, TIME_PARALLEL_THREAD_CREATION_TOO
#include "acpphint_kernels.h"        // for NMIN
class ClkInfo;

template<typename DSIZE, typename ISIZE>
struct KernelSurveyorResults
{
    ISIZE nscout;

    KernelRunnerResults<DSIZE,ISIZE> krr;
            // krr.median_mean_sec_per_lap holds the closest thing
            // to what the original HINT benchmarks call tscout. The
            // detailed time measurement techniques are not the
            // same.

    auto tscout() const -> auto { return krr.median_mean_sec_per_lap; };
            // Mostly here to provide name from original HINT variants.

    KernelSurveyorResults(PrimaryKernelInputs<DSIZE,ISIZE> const& ki)
                    : nscout{NMIN<ISIZE>(ki.nproc)}
                    , krr   {}
    {}
}; // KernelSurveyorResults

template<typename DSIZE, typename ISIZE, bool want_parallel_thread_creation_time_included>
auto KernelSurveyor ( ClkInfo                           const&  clock_info
                    , PrimaryKernelInputs<DSIZE,ISIZE>  const&  ki
                    ) -> KernelSurveyorResults<DSIZE,ISIZE>;

#endif // acpphint_kernelsurveyors_guard
