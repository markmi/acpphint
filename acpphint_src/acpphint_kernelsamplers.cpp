//
//  acpphint_kernelsamplers.cpp
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

#include "acpphint_kernelsamplers.h"
#include <algorithm>                   // for std::max
#include <array>                       // for std::array
#include <charconv>                    // for std::chars_format, std::to_chars
#include <climits>                     // for ULONG_MAX, UINT_MAX, ULLONG_MAX
#include <cmath>                       // for pow, nexttoward, std::nexttoward
#include <cstddef>                     // for size_t, std::size_t
#include <limits>                      // for std::numeric_limits
#include <string>                      // for std::to_string, std::operator+
#include <string_view>                 // for std::basic_string_view, std::s...
#include <system_error>                // for std::errc
#include "acpphint_kernelrunners.h"    // for approx_answer_floating_form, TIME_PARALLEL_THREAD_CREATION_TOO
#include "acpphint_kernels.h"          // for KernelResults, NMIN
#include "acpphint_kernelsurveyors.h"  // for KernelSurveyor
#include "cpp_clockinfo.h"             // for ClkInfo

template<typename DSIZE, typename ISIZE, bool want_parallel_thread_creation_time_included>
auto KernelSampler  ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<DSIZE,ISIZE>      const&  ki
                    ) -> KernelSamplerResultsVect<DSIZE,ISIZE>
{
    using quality_improvement_per_sec
        = typename KernelSamplerResults<DSIZE,ISIZE>::QualityImprovementPerSec;

    auto constexpr NOERROR{KernelResults<DSIZE,ISIZE>::EFlag::normal};

    KernelSamplerResultsVect<DSIZE,ISIZE>   qdata{};

    auto const k_surveyor_result{KernelSurveyor<DSIZE,ISIZE,want_parallel_thread_creation_time_included>
                                     (clock_info,ki)
                                };

    if  (  NOERROR != k_surveyor_result.krr.kernel_result.eflag
        || ki.initial_dx <= static_cast<DSIZE>(k_surveyor_result.nscout)
        )
    {
        qdata.emplace_back  ( k_surveyor_result.krr
                            , quality_improvement_per_sec{0}
                            , k_surveyor_result.nscout
                            );
        return qdata;
    }

    using sec_floating_form
        = typename KernelRunnerResults<DSIZE,ISIZE>::SecFloatingForm;

    using floating_type
        = typename KernelSamplerResults<DSIZE,ISIZE>::FloatingType;

    using num_samples
        = typename KernelSamplerResultsVect<DSIZE,ISIZE>::size_type;

    // The below two liklely both change together, likely:
    // smaller ADVANCE_factor <=> larger NSAMP if one wants
    // about the same range of n values covered.
    //
    // For the 1.258925411794167L factor: 4 is the smallest n
    // for which n<trunc(ADVANCE_factor*n) so 0..4 end up
    // handled via n+1 instead, leaving 65 values of n based on
    // ADVANCE_factor directly when NSAMP==70u.
    //
    // Edit as needed, the next 2 (in original HINTs too):
#ifndef NSAMP_ALT
    num_samples         constexpr NSAMP{80u};
#else
    num_samples         constexpr NSAMP{NSAMP_ALT};
#endif
    long double         constexpr ADVANCE_factor{1.258925411794167L};
                                                // Approx.: 10th root of 10.

    // Edit as needed, the next 3 (in original HINTs too):
    sec_floating_form   const     RUNTM{clock_info.TargetApproxMinDuration()};
#ifndef STOPTM_ALT
    sec_floating_form   constexpr STOPTM{1u}; // seconds
#else
    sec_floating_form   constexpr STOPTM{STOPTM_ALT}; // seconds
#endif
#ifndef STOPRT_DROP_FACTOR
    floating_type       constexpr STOPRT{floating_type{1}/floating_type{30}};
                                    // quips/peak_quips greatest lower bound.
#else
    floating_type       constexpr STOPRT{  floating_type{1}
                                         / floating_type{STOPRT_DROP_FACTOR}
                                        };
                                    // quips/peak_quips greatest lower bound.
#endif

    // Edit as needed, the next 3 (new ones):
    ISIZE               constexpr STOPRT_GRID_SIZE_THRESHOLD{0u};
    LapsCount           constexpr LAPS_STOP
                                  {std::numeric_limits<LapsCount>::max()/2};
    std::size_t         constexpr VECTS_BYTES_LIMIT
                                  {std::numeric_limits<std::size_t>::max()/2};

    // Note: If increased, STOPRT_GRID_SIZE_THRESHOLD is used to avoid
    //       the STOPRT criteiria for smaller n. I used such to avoid
    //       stopping in a large "quips valley" with steep sides on
    //       some old hardware. (Worded in terms of the old plots.)
    //
    //       This context had a solid example of "more work takes less
    //       time" for the larger-n side of the valley as well.

    sec_floating_form   constexpr zero_sec{0};
    sec_floating_form   const     runtm_scaled{k_surveyor_result.nscout*RUNTM};
                                    // used in estimating laps for RUNTM

    floating_type       constexpr floating_type_1{1};

    quality_improvement_per_sec quips_peak{0u};
    floating_type               quips_to_peak_ratio{floating_type_1};


    ISIZE                               n{NMIN<ISIZE>(ki.nproc)};
    DSIZE                               DSIZE_n= n;

    KernelRunnerResults<DSIZE,ISIZE>    run_result{};

    qdata.reserve(NSAMP);

    while   (  qdata.size() < NSAMP
            && DSIZE_n < ki.initial_dx
            )   // STOPTM check is where median_mean_sec_per_lap is known.
                // eflag check is also later, as is the VECTS_BYTES_LIMIT
                // check and STOPRT check.
    {
        sec_floating_form   const tscout_scaled{n*k_surveyor_result.tscout()};
        floating_type       const truncated_ratio
                                    {std::trunc(runtm_scaled/tscout_scaled)};

        LapsCount laps= std::max(truncated_ratio,floating_type_1);

        run_result= KernelRunner<DSIZE,ISIZE,want_parallel_thread_creation_time_included>
                        ( clock_info
                        , laps
                        , n
                        , ki
                        );

        // The original HINTs did not rescale laps until time progressed
        // sufficiently.
        while (  run_result.total_sec_for_laps_for_median < RUNTM
              && laps < LAPS_STOP
              && NOERROR == run_result.kernel_result.eflag
              )
        {
            laps *= 2;
            run_result= KernelRunner<DSIZE,ISIZE,want_parallel_thread_creation_time_included>
                            ( clock_info
                            , laps
                            , n
                            , ki
                            );
        }

        quality_improvement_per_sec quips;

        if  (  NOERROR  == run_result.kernel_result.eflag
            && zero_sec <  run_result.median_mean_sec_per_lap
            )
        {
            DSIZE const gamut=  run_result.kernel_result.result_bounds.HI
                              - run_result.kernel_result.result_bounds.LO;
            floating_type const one_over_gamut= floating_type_1/gamut;

            // Calculate QUIPS. We must add one to dmax, but do it in steps.
            // This to avoid overflow of dmax.

            floating_type const delq
                    {static_cast<floating_type>(ki.dmax)/gamut - floating_type_1};

            // delq is an incomplete estimate of the full change
            // in the quality: there is another 1/gamut (rational
            // viewpoint) that helps avoid overflow/truncation/rounding.

            quips=          (delq/run_result.median_mean_sec_per_lap.count())
                + (one_over_gamut/run_result.median_mean_sec_per_lap.count());
        }
        else 
        {
            quips= 0;
        }

        qdata.emplace_back(run_result,quips,n);

        if (NOERROR != run_result.kernel_result.eflag)
            break;

        quips_peak=             std::max(quips_peak,quips);
        quips_to_peak_ratio=    quips/quips_peak;

        if (STOPTM <= run_result.median_mean_sec_per_lap)
            break;

        if (VECTS_BYTES_LIMIT <= run_result.vectors_total_bytes)
            break;

        if  (  STOPRT_GRID_SIZE_THRESHOLD<n // Threshold not in original HINTs
            && quips_to_peak_ratio<STOPRT
            )
            break;

        ISIZE const potential_n_by_increment=   n+1;
        ISIZE const potential_n_by_factor=      ADVANCE_factor*n;

        n=          std::max(potential_n_by_increment,potential_n_by_factor);
        DSIZE_n=    n;
    }

    // The original HINTs attempted to eliminate reporting
    // examples of less work taking more time. This variation
    // reports instead such results when the median_mean_sec_per_lap
    // works out that way for the quips value. (The resulting
    // linespoints graph need not be of a [single-valued] function
    // when the x-axis is for median_mean_sec_per_lap .)

    if (qdata.empty()) return qdata;

    if (STOPTM <= run_result.median_mean_sec_per_lap)
        qdata.back().how_stopped_notes
            +=  " median_mean_sec_per_lap:  stopped for "
              + std::to_string(STOPTM.count()) + " == STOPTM"
              + " <= "
              + "median_mean_sec_per_lap == "
                + std::to_string(run_result.median_mean_sec_per_lap.count())
              + "\n";

    if (VECTS_BYTES_LIMIT <= run_result.vectors_total_bytes)
        qdata.back().how_stopped_notes
            +=  " vectors_total_bytes:      stopped for "
              + std::to_string(VECTS_BYTES_LIMIT) + " == VECTS_BYTES_LIMIT"
              + " <= "
              + "vectors_total_bytes == "
                + std::to_string(run_result.vectors_total_bytes)
              + "\n";

    if  (quips_to_peak_ratio < STOPRT)
        qdata.back().how_stopped_notes
            +=  " quips_to_peak_ratio:      stopped for "
              + std::to_string(quips_to_peak_ratio)
              + " == " + std::to_string(qdata.back().quips) + "/" + std::to_string(quips_peak)
              + " == quips/peak_quips"
              + " < "
              + "STOPRT == " + std::to_string(STOPRT)
              + "\n";

    if (ki.initial_dx <= DSIZE_n) // DSIZE_n here is after last n used.
        qdata.back().how_stopped_notes
            +=  " n:                        stopped for "
              + std::to_string(ki.initial_dx) + " == initial_dx"
              + " <= "
              + "(next n) == " + std::to_string(DSIZE_n)
              + "\n";

    if (NSAMP <= qdata.size())
        qdata.back().how_stopped_notes
            +=  " qdata.size():             stopped for "
              + std::to_string(NSAMP) + " == NSAMP"
              + " <= "
              + " qdata.size() == " + std::to_string(qdata.size())
              + "\n";

    if (NOERROR != run_result.kernel_result.eflag)
        qdata.back().how_stopped_notes
            +=  " eflag:                    "
                "stopped for/with eflag not normal ("
              + std::to_string(static_cast<int>(run_result.kernel_result.eflag))
              + ")\n";
    else
        qdata.back().how_stopped_notes
            +=  " eflag:                    "
                "stopped with eflag normal ("
              + std::to_string(static_cast<int>(run_result.kernel_result.eflag))
              + ")\n";

    if (run_result.median_mean_sec_per_lap < STOPTM)
        qdata.back().how_stopped_notes
            +=  " median_mean_sec_per_lap:  stopped with "
              + std::to_string(run_result.median_mean_sec_per_lap.count())
                + " == median_mean_sec_per_lap"
              + " < "
              + "STOPTM == " + std::to_string(STOPTM.count())
              + "\n";

    if (run_result.vectors_total_bytes < VECTS_BYTES_LIMIT)
        qdata.back().how_stopped_notes
            +=  " vectors_total_bytes:      stopped with "
              + std::to_string(run_result.vectors_total_bytes)
                + " == vectors_total_bytes"
              + " < "
              + "VECTS_BYTES_LIMIT == " + std::to_string(VECTS_BYTES_LIMIT)
              + "\n";

    if  (STOPRT <= quips_to_peak_ratio)
        qdata.back().how_stopped_notes
            +=  " quips_to_peak_ratio:      stopped with "
              + std::to_string(STOPRT) + " == STOPRT"
              + " <= "
              + "quips/peak_quips"
              + " == " + std::to_string(qdata.back().quips) + "/" + std::to_string(quips_peak)
              + " == " + std::to_string(quips_to_peak_ratio)
              + "\n";

    if (DSIZE_n < ki.initial_dx) // DSIZE_n here is after last n used.
        qdata.back().how_stopped_notes
            +=  " n:                        stopped with "
              + std::to_string(DSIZE_n) + " == (next n)"
              + " < "
              + "initial_dx == " + std::to_string(ki.initial_dx)
              + "\n";

    if (qdata.size() < NSAMP)
        qdata.back().how_stopped_notes
            +=  " qdata.size():             stopped with "
              + std::to_string(qdata.size()) + " == qdata.size()"
              + " < "
              + "NSAMP == " + std::to_string(NSAMP)
              + "\n";

    qdata.back().how_stopped_notes
        +=  " vectors_total_bytes/median_mean_sec_per_lap == "
          + std::to_string(  run_result.vectors_total_bytes
                           / run_result.median_mean_sec_per_lap.count())
          + "\n";

    auto constexpr dig10      {std::numeric_limits<decltype(approx_answer_floating_form)>::digits10};
    auto const     delta      {std::pow(static_cast<decltype(approx_answer_floating_form)>(1.0L/10.0L),dig10)};
    auto const     more_delta {std::nexttoward(delta,static_cast<decltype(delta)>(1.0))};

    auto const answer_lower_bound_floating_form {approx_answer_floating_form - more_delta};

    auto const answer_upper_bound_floating_form {approx_answer_floating_form + more_delta};

    auto const scale_factor_for_hi // an upper bound on scale factor
        ( ( static_cast<decltype(approx_answer_floating_form)>(run_result.kernel_result.result_bounds.HI)
              / static_cast<decltype(approx_answer_floating_form)>(ki.scx)
              / static_cast<decltype(approx_answer_floating_form)>(ki.scy)
          ) 
         / answer_lower_bound_floating_form
        );
    auto const scale_factor_for_lo // a lower bound on scale factor
        ( ( static_cast<decltype(approx_answer_floating_form)>(run_result.kernel_result.result_bounds.LO)
              / static_cast<decltype(approx_answer_floating_form)>(ki.scx)
              / static_cast<decltype(approx_answer_floating_form)>(ki.scy)
          )
         / answer_upper_bound_floating_form
        );

    std::array<char, 128> lower_approx;
    auto const [after_lower_approx_chars, lower_approx_err_code]
        ( std::to_chars( lower_approx.data(), lower_approx.data() + lower_approx.size()
                       , answer_lower_bound_floating_form, std::chars_format::fixed
                       )
        );
    std::array<char, 128> upper_approx;
    auto const [after_upper_approx_chars, upper_approx_err_code]
        ( std::to_chars( upper_approx.data(), upper_approx.data() + upper_approx.size()
                       , answer_upper_bound_floating_form, std::chars_format::fixed
                       )
        );

    std::array<char, 128> scale_factor; // For HI's context then reused for LO's.

    qdata.back().how_stopped_notes += " HI/(scx*scy) == ? * approx. lower bound: ";
    auto const [after_scale_hi_chars, scale_hi_err_code]
        ( std::to_chars( scale_factor.data(), scale_factor.data() + scale_factor.size()
                       , scale_factor_for_hi, std::chars_format::fixed
                       )
        );
    if (scale_hi_err_code == std::errc() && lower_approx_err_code == std::errc())
    {
        qdata.back().how_stopped_notes
            +=  std::string_view(scale_factor.data(), after_scale_hi_chars);
        qdata.back().how_stopped_notes += " * ";
        qdata.back().how_stopped_notes
            += std::string_view(lower_approx.data(), after_lower_approx_chars);
    }
    else
        qdata.back().how_stopped_notes
            +=  std::to_string(scale_factor_for_hi)
              + " * " + std::to_string(answer_lower_bound_floating_form);
    qdata.back().how_stopped_notes += "\n";

    qdata.back().how_stopped_notes += " LO/(scx*scy) == ? * approx. upper bound: ";
    auto [after_scale_lo_chars, scale_lo_err_code]
        ( std::to_chars( scale_factor.data(), scale_factor.data() + scale_factor.size()
                       , scale_factor_for_lo, std::chars_format::fixed
                       )
        );
    if (scale_lo_err_code == std::errc() && upper_approx_err_code == std::errc())
    {
        qdata.back().how_stopped_notes
            +=  std::string_view(scale_factor.data(), after_scale_lo_chars);
        qdata.back().how_stopped_notes += " * ";
        qdata.back().how_stopped_notes
            += std::string_view(upper_approx.data(), after_upper_approx_chars);
    }
    else
        qdata.back().how_stopped_notes
            +=  std::to_string(scale_factor_for_lo)
              + " * " + std::to_string(answer_upper_bound_floating_form);
    qdata.back().how_stopped_notes += "\n";

    return qdata;
}


// Edit as needed to add more alternatives (or disable some):

// DSIZE=short:

template
auto KernelSampler<short,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,short>      const&  ki
                    ) -> KernelSamplerResultsVect<short,short>;

template
auto KernelSampler<short,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,short>      const&  ki
                    ) -> KernelSamplerResultsVect<short,short>;

template
auto KernelSampler<short,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<short,unsigned short>;

template
auto KernelSampler<short,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<short,unsigned short>;

// DSIZE=unsigned short:

template
auto KernelSampler<unsigned short,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect
                                            <unsigned short,short>;

template
auto KernelSampler<unsigned short,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect
                                            <unsigned short,short>;

template
auto KernelSampler<unsigned short,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect
                                            <unsigned short,unsigned short>;

template
auto KernelSampler<unsigned short,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect
                                            <unsigned short,unsigned short>;

#if ULONG_MAX == UINT_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned int:

template
auto KernelSampler<unsigned int,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned int,unsigned int>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<unsigned int,unsigned int>;
#endif

// DSIZE=unsigned long: // Always included

template
auto KernelSampler<unsigned long,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned long,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<unsigned long,unsigned long>;

template
auto KernelSampler<unsigned long,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned long,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<unsigned long,unsigned long>;

#if ULONG_MAX == ULLONG_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned long long:

template
auto KernelSampler<unsigned long long,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs       < unsigned long long
                                                , unsigned long long
                                                >           const&  ki
                    ) -> KernelSamplerResultsVect   < unsigned long long
                                                    , unsigned long long
                                                    >;

template
auto KernelSampler<unsigned long long,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs       < unsigned long long
                                                , unsigned long long
                                                >           const&  ki
                    ) -> KernelSamplerResultsVect   < unsigned long long
                                                    , unsigned long long
                                                    >;
#endif

// DSIZE=float:

template
auto KernelSampler<float,short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,short>      const&  ki
                    ) -> KernelSamplerResultsVect<float,short>;

template
auto KernelSampler<float,short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,short>      const&  ki
                    ) -> KernelSamplerResultsVect<float,short>;

template
auto KernelSampler<float,unsigned short,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<float,unsigned short>;

template
auto KernelSampler<float,unsigned short,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<float,unsigned short>;

template
auto KernelSampler<float,unsigned int,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<float,unsigned int>;

template
auto KernelSampler<float,unsigned int,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<float,unsigned int>;

#ifdef DSIZE_ALL_ISIZE_ALL
// DSIZE=double:

template
auto KernelSampler<double,unsigned int,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned int>;

template
auto KernelSampler<double,unsigned int,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned int>;

template
auto KernelSampler<double,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned long>;

template
auto KernelSampler<double,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned long>;

template
auto KernelSampler<double,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned long long>;

template
auto KernelSampler<double,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<double,unsigned long long>;

// DSIZE=long double:

template
auto KernelSampler<long double,unsigned long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<long double,unsigned long>;

template
auto KernelSampler<long double,unsigned long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const&  ki
                    ) -> KernelSamplerResultsVect<long double,unsigned long>;

template
auto KernelSampler<long double,unsigned long long,TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs       < long double
                                                , unsigned long long
                                                >           const&  ki
                    ) -> KernelSamplerResultsVect   < long double
                                                    , unsigned long long
                                                    >;

template
auto KernelSampler<long double,unsigned long long,!TIME_PARALLEL_THREAD_CREATION_TOO>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs       < long double
                                                , unsigned long long
                                                >           const&  ki
                    ) -> KernelSamplerResultsVect   < long double
                                                    , unsigned long long
                                                    >;
#endif

char copyright_and_license_for_acpphint_kernelsamplers[]
{
    "Context for this Copyright: acpphint_kernelsamplers\n"
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
