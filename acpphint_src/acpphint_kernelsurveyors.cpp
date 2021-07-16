//
//  acpphint_kernelsurveyors.cpp
//  acpphint (a C++ variation on the old HINT benchmark)
//
//  Copyright (c) 2015-2019 Mark Millard
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

#include "acpphint_kernelsurveyors.h"

#include <climits>     // ULONG_MAX, UINT_MAX, ULLONG_MAX

template<typename DSIZE, typename ISIZE>
auto KernelSurveyor ( ClkInfo                           const&  clock_info
                    , PrimaryKernelInputs<DSIZE,ISIZE>  const&  ki
                    ) -> KernelSurveyorResults<DSIZE,ISIZE>
{
    // Edit as needed, VALUE in laps{VALUE} below:
    LapsCount constexpr laps{3u};

    auto constexpr NOERROR{KernelResults<DSIZE,ISIZE>::EFlag::normal};

    KernelSurveyorResults<DSIZE,ISIZE> survey_result{ki};
    
    while (static_cast<DSIZE>(survey_result.nscout)<ki.initial_dx)
    {
        auto const run_result
            {KernelRunner<DSIZE,ISIZE>  ( clock_info
                                        , laps
                                        , survey_result.nscout
                                        , ki
                                        )
            };

        if (NOERROR!=run_result.kernel_result.eflag)
        {
            // Prior run_result value is used, possibly STILL_UNKNOWN.
            survey_result.nscout/= 2;
            break;
        }

        survey_result.krr= run_result; // No longer STILL_UNKNOWN.
        
        if  (  clock_info.TargetApproxMinDuration()
             < run_result.median_mean_sec_per_lap
            )
            break; // Directly good result.
        
        survey_result.nscout*= 2;
        
        // Without the = below, ki.initial_dx==survey_result.nscout
        // results in using a run result for a smaller
        // survey_result.nscout value. The original pthread HINT
        // does not have the = .
        if (ki.initial_dx<=static_cast<DSIZE>(survey_result.nscout))
        {
            // Unable to take the target time for
            // smaller than the biggest dx. Report
            // the last tried size and its estimated
            // duration (run_result.median_mean_sec_per_lap).
            survey_result.nscout/= 2;
            break;
        }
    }

    return survey_result;
}


// Edit as needed to add more alternatives (or disable some):

// DSIZE=short: // Always included

template
auto KernelSurveyor<short,short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,short>      const&  ki
                    ) -> KernelSurveyorResults<short,short>;

template
auto KernelSurveyor<short,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<short,unsigned short>
                                                            const&  ki
                    ) -> KernelSurveyorResults<short,unsigned short>;

// DSIZE=unsigned short: // Always included

template
auto KernelSurveyor<unsigned short,short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,short>
                                                            const&  ki
                    ) -> KernelSurveyorResults<unsigned short,short>;

template
auto KernelSurveyor<unsigned short,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned short,unsigned short>
                                                            const&  ki
                    ) -> KernelSurveyorResults<unsigned short,unsigned short>;

#if ULONG_MAX == UINT_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned int:

template
auto KernelSurveyor<unsigned int,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<unsigned int,unsigned int>
                                                            const&  ki
                    ) -> KernelSurveyorResults<unsigned int,unsigned int>;
#endif
                    
// DSIZE=unsigned long: // Always included

template
auto KernelSurveyor<unsigned long,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs   < unsigned long
                                            , unsigned long
                                            >               const&  ki
                    ) -> KernelSurveyorResults  < unsigned long
                                                , unsigned long
                                                >;

#if ULONG_MAX == ULLONG_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned long long:

template
auto KernelSurveyor<unsigned long long,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs   < unsigned long long
                                            , unsigned long long
                                            >               const&  ki
                    ) -> KernelSurveyorResults  < unsigned long long
                                                , unsigned long long
                                                >;
#endif

// DSIZE=float:

template
auto KernelSurveyor<float,short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,short>      const&  ki
                    ) -> KernelSurveyorResults<float,short>;

template
auto KernelSurveyor<float,unsigned short>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned short>
                                                            const&  ki
                    ) -> KernelSurveyorResults<float,unsigned short>;
                    
template
auto KernelSurveyor<float,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<float,unsigned int>
                                                            const&  ki
                    ) -> KernelSurveyorResults<float,unsigned int>;

#ifdef DSIZE_ALL_ISIZE_ALL
// DSIZE=double:
            
template
auto KernelSurveyor<double,unsigned int>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned int>
                                                            const&  ki
                    ) -> KernelSurveyorResults<double,unsigned int>;

template
auto KernelSurveyor<double,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long>
                                                            const&  ki
                    ) -> KernelSurveyorResults<double,unsigned long>;

template
auto KernelSurveyor<double,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<double,unsigned long long>
                                                            const&  ki
                    ) -> KernelSurveyorResults<double,unsigned long long>;

// DSIZE=long double:

template
auto KernelSurveyor<long double,unsigned long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<long double,unsigned long>
                                                            const&  ki
                    ) -> KernelSurveyorResults<long double,unsigned long>;

template
auto KernelSurveyor<long double,unsigned long long>
                    ( ClkInfo                               const&  clock_info
                    , PrimaryKernelInputs<long double,unsigned long long>
                                                            const&  ki
                    ) -> KernelSurveyorResults<long double,unsigned long long>;
#endif


char copyright_and_license_for_acpphint_kernelsurveyors[]
{
    "Context for this Copyright: acpphint_kernelsurveyors\n"
    "\n"
    "Copyright (c) 2015-2019 Mark Millard\n"
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
