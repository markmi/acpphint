//
//  acpphint_kernelrunners.h
//  acpphint (a C++ variation on the old HINT benchmark)
//
//  Started by Mark Millard on 2015-04-28.
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

#ifndef acpphint_kernelrunners_guard
#define acpphint_kernelrunners_guard

#include <chrono>                 // for std::chrono::duration, std::chrono:...
#include <cstddef>                // for size_t, std::size_t
#include "acpphint_kernels.h"     // for KernelResults
class ClkInfo;

constexpr bool dsize_all_isize_all{
#ifdef DSIZE_ALL_ISIZE_ALL
                                      true
#else
                                      false
#endif
                                  };

extern 
#if 201611L <= __cpp_lib_to_chars
long
#endif
double const approx_answer_floating_form;

template<typename DSIZE, typename ISIZE>
struct KernelRunnerResults
{
    KernelResults<DSIZE,ISIZE>  kernel_result{};
                                // From median_mean_sec_per_lap example
                                // if successfully found such.

    using SecFloatingForm
            = std::chrono::duration<long double,std::chrono::seconds::period>;

    SecFloatingForm             total_sec_for_laps_for_median{};
    SecFloatingForm             median_mean_sec_per_lap{};

    std::size_t                 vectors_total_bytes{};

    KernelRunnerResults (typename KernelResults<DSIZE,ISIZE>::EFlag eflag)
                        : kernel_result{eflag}
    {} // Likely NOMEM up front problem.

    KernelRunnerResults() {}

    KernelRunnerResults ( KernelResults<DSIZE,ISIZE> const& kr
                        , SecFloatingForm                   total_duration
                        , SecFloatingForm                   mean_duration
                        , std::size_t                       vects_total_bytes
                        )   : kernel_result                 {kr}
                            , total_sec_for_laps_for_median {total_duration}
                            , median_mean_sec_per_lap       {mean_duration}
                            , vectors_total_bytes           {vects_total_bytes}
    {
        static_assert
            ( std::chrono::treat_as_floating_point<SecFloatingForm::rep>
                                ::value
            , "SecFloatingForm not of floating point form"
            );
    }
}; // KernelRunnerResults

using LapsCount = int;  // Deliberately signed for allowing signed/signed
                        // without forced conversion of operands to unsigned.
                        // int for minimizing overhead while still having a
                        // large range of non-negative values.

constexpr bool TIME_PARALLEL_THREAD_CREATION_TOO{true}; // Also avoid later join and such.

template<typename DSIZE, typename ISIZE, bool want_parallel_thread_creation_time_included>
auto KernelRunner   ( ClkInfo                           const&  clock_info
                    , LapsCount                         const   laps
                    , ISIZE                             const   memry
                    , PrimaryKernelInputs<DSIZE,ISIZE>  const   ki
                    ) -> KernelRunnerResults<DSIZE,ISIZE>;

#endif // acpphint_kernelrunners_guard
