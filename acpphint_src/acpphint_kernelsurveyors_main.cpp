//
//  acpphint_kernelsurveyors_main.cpp
//  acpphint (a C++ variation on the old HINT benchmark)
//
//  Copyright (c) 2015-2023 Mark Millard
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

//
// acpphint_kernelsurveyors_main is used to see what kernel
// surveyor results are like, both for testing and for seeing
// if any involved "Edit as needed" items need adjustment.
//
// Use of an environment with non-power-of-2
// ConcurrencyCountForInDomains() values can be
// appropriate. Or adjust thread_count at its
// "Edit as needed" place below.
//
// Also large thread_counts can not handle smaller DSIZE
// or ISIZE types like unsigned short or short, given
// NCHUNK: scx=initial_dx*nproc*NCHUCK with initial_dx
// being a integral (non-negative) power of 2. It just
// does not have enough room for nscout<initial_dx to fit.
//

#include <climits>                     // for ULONG_MAX, UINT_MAX, ULLONG_MAX
#include <exception>                   // for std::exception
#include <iostream>                    // for std::__1::operator<<, std::bas...
#include "acpphint_kernels.h"          // for KernelResults, KernelVectors
#include "acpphint_kernelsurveyors.h"  // for KernelSurveyor
#include "cpp_clockinfo.h"             // for ClkInfo, HwConcurrencyCount
#include "cpp_thousandslocale.h"       // for CppThousandsLocale
#include "sys_cpubinding.h"            // for ConcurrencyCountForInDomains

template<typename DSIZE,typename ISIZE>
static void report_survey(ClkInfo const& clock_info)
{
    std::cout
        << "serial. . .\n"
        << "\n" << std::flush;

    PrimaryKernelInputs<DSIZE,ISIZE> const ki_serial(1u);
    
    std::cout
        << "nproc:      " << ki_serial.nproc << "\n"
        << "scx:        " << ki_serial.scx << "\n"
        << "scy:        " << ki_serial.scy << "\n"
        << "dmax:       " << ki_serial.dmax << "\n"
        << "initial_dx: " << ki_serial.initial_dx << "\n"
        << "\n" << std::flush;

    auto const ks_serial_result{KernelSurveyor(clock_info,ki_serial)};
    
    if  (   KernelResults<DSIZE,ISIZE>::EFlag::NOMEM
         == ks_serial_result.krr.kernel_result.eflag
        )
    {
        std::cout
            << "Got EFlag::NOMEM\n" << std::flush;
    }
    else
    {
        std::cout
            << "nscout:                                 "
                << ks_serial_result.nscout << "\n"
            << "krr.kernel_result.result_bounds.HI:     "
                << ks_serial_result.krr.kernel_result.result_bounds.HI << "\n"
            << "krr.kernel_result.result_bounds.LO:     "
                << ks_serial_result.krr.kernel_result.result_bounds.LO << "\n"
            << "krr.kernel_result.eflag:                "
                << static_cast<int>(ks_serial_result.krr.kernel_result.eflag)
                << "\n"
            << "krr.kernel_result.RECT_maxjo:           "
                << ks_serial_result.krr.kernel_result.RECT_maxjo << "\n"
            << "krr.kernel_result.ixes_errs_maxiq:      "
                << ks_serial_result.krr.kernel_result.ixes_errs_maxiq << "\n"
            << "krr.kernel_result.RECT_total_used:      "
                << ks_serial_result.krr.kernel_result.RECT_total_used << "\n"
            << "krr.kernel_result.ixes_errs_used_each:  "
                << ks_serial_result.krr.kernel_result.ixes_errs_used_each
                << "\n"
            << "krr.total_sec_for_laps_for_median:      "
                << ks_serial_result.krr.total_sec_for_laps_for_median.count()
                << "\n"
            << "krr.tscout():                           "
                << ks_serial_result.tscout().count() << "\n"
            << "krr.vectors_total_bytes:                "
                << ks_serial_result.krr.vectors_total_bytes << "\n"
            << "VectorsUsedBytes:                       "
                << KernelVectors<DSIZE,ISIZE>::VectorsUsedBytes
                    ( ks_serial_result.krr.kernel_result.RECT_total_used
                    , ks_serial_result.krr.kernel_result.ixes_errs_used_each
                    )
                << "\n"
            << "\n" << std::flush;
        std::cout
            << "approx HI normal scale:     "
                <<    static_cast<long double>
                        (ks_serial_result.krr.kernel_result.result_bounds.HI)
                    / static_cast<long double>(ki_serial.scx)
                    / static_cast<long double>(ki_serial.scy)
                << "\n"
            << "approx LO normal scale:     "
                <<    static_cast<long double>
                        (ks_serial_result.krr.kernel_result.result_bounds.LO)
                    / static_cast<long double>(ki_serial.scx)
                    / static_cast<long double>(ki_serial.scy)
                << "\n"
            << "\n" << std::flush;
    }

    HwConcurrencyCount const thread_count{ConcurrencyCountForInDomains()};
    
    if (thread_count<2u) return;
    
    // Edit as needed: thread_count for what to try.
     
    std::cout
        << "threaded. . .\n"
        << "\n" << std::flush;

    PrimaryKernelInputs<DSIZE,ISIZE> ki_parallel(thread_count);
   
    std::cout
        << "nproc:          " << ki_parallel.nproc << "\n"
        << "scx:            " << ki_parallel.scx << "\n"
        << "scy:            " << ki_parallel.scy << "\n"
        << "dmax:           " << ki_parallel.dmax << "\n"
        << "initial_dx:     " << ki_parallel.initial_dx << "\n"
        << "\n" << std::flush;

    std::cout
        << "NCHUNK(nproc):  "
            <<    static_cast<long double>(ki_parallel.scx)
                / static_cast<long double>(ki_parallel.nproc)
                / static_cast<long double>(ki_parallel.initial_dx)
            << "\n"
        << "\n" << std::flush;

    auto const ks_parallel_result{KernelSurveyor(clock_info,ki_parallel)};

    if  (   KernelResults<DSIZE,ISIZE>::EFlag::NOMEM
         == ks_parallel_result.krr.kernel_result.eflag
        )
    {
        std::cout
            << "Got EFlag::NOMEM\n" << std::flush;
    }
    else
    {
        if  (   ki_parallel.initial_dx
             <= static_cast<DSIZE>(ks_parallel_result.nscout)
            )
        {
            //    KernelResults<DSIZE,ISIZE>::EFlag::STILL_UNKNOWN
            // == ks_parallel_result.krr.kernel_result.eflag
            // as well.
            std::cout
                << "nscout<initial_dx failed, given nproc\n"
                << "nscout:                             "
                    << ks_parallel_result.nscout << "\n"
                << "initial_dx:                         "
                    << ki_parallel.initial_dx << "\n"
                << "\n" << std::flush;
        }
        else
        {
            std::cout
                << "nscout:                                 "
                    << ks_parallel_result.nscout << "\n"
                << "krr.kernel_result.result_bounds.HI:     "
                    << ks_parallel_result.krr.kernel_result.result_bounds.HI
                    << "\n"
                << "krr.kernel_result.result_bounds.LO:     "
                    << ks_parallel_result.krr.kernel_result.result_bounds.LO
                    <<  "\n"
                << "krr.kernel_result.eflag:                "
                    << static_cast<int>
                                (ks_parallel_result.krr.kernel_result.eflag)
                    << "\n"
                << "krr.kernel_result.RECT_maxjo:           "
                    << ks_parallel_result.krr.kernel_result.RECT_maxjo << "\n"
                << "krr.kernel_result.ixes_errs_maxiq:      "
                    << ks_parallel_result.krr.kernel_result.ixes_errs_maxiq
                    << "\n"
                << "krr.kernel_result.RECT_total_used:      "
                    << ks_parallel_result.krr.kernel_result.RECT_total_used
                    << "\n"
                << "krr.kernel_result.ixes_errs_used_each:  "
                    << ks_parallel_result.krr.kernel_result.ixes_errs_used_each
                    << "\n"
                << "krr.total_sec_for_laps_for_median:      "
                    << ks_parallel_result.krr.total_sec_for_laps_for_median
                                                                    .count()
                    << "\n"
                << "krr.tscout():                           "
                    << ks_parallel_result.tscout().count() << "\n"
                << "krr.vectors_total_bytes:                "
                    << ks_parallel_result.krr.vectors_total_bytes << "\n"
                << "VectorsUsedBytes:                       "
                    << KernelVectors<DSIZE,ISIZE>::VectorsUsedBytes
                        ( ks_parallel_result.krr.kernel_result.RECT_total_used
                        , ks_parallel_result.krr
                                            .kernel_result.ixes_errs_used_each
                        )
                    << "\n"
                << "\n" << std::flush;
            std::cout
                << "approx HI normal scale: "
                    <<  static_cast<long double>
                        (ks_parallel_result.krr.kernel_result.result_bounds.HI)
                      / static_cast<long double>(ki_parallel.scx)
                      / static_cast<long double>(ki_parallel.scy)
                        << "\n"
                << "approx LO normal scale: "
                    <<  static_cast<long double>
                        (ks_parallel_result.krr.kernel_result.result_bounds.LO)
                      / static_cast<long double>(ki_parallel.scx)
                      / static_cast<long double>(ki_parallel.scy)
                    << "\n"
                << "\n" << std::flush;
        }
    }
}

int main(int argc, const char* argv[])
try
{
    if (0==argc || nullptr==argv[0] || '\0'==*argv[0])
    {
        std::cout
            << "argv[0] does not provide program name\n";
        return 1;
    }

    std::cout
        << argv[0u] << " . . .\n"
        << "acpphint_kernelsurveyors_main version: "
           ACPPHINTKERNELSURVEYORS_VERS "\n"
        << "\n" << std::flush;


    std::cout.imbue(CppThousandsLocale());

    std::cout
        << argv[0u] << " . . .\n"
        << "\n";

    ClkInfo clock_info{};

    // Edit as needed to add more alternatives (or disable some):
    
#ifdef DSIZE_ALL_ISIZE_ALL
    std::cout
        << "DSIZE=long double, ISIZE=unsigned long long:\n"
        << "\n";
    
    report_survey<long double,unsigned long long>(clock_info);
    
    std::cout
        << "DSIZE=long double, ISIZE=unsigned long:\n"
        << "\n";
    
    report_survey<long double,unsigned long>(clock_info);
#endif

 
#if ULONG_MAX == ULLONG_MAX || defined(DSIZE_ALL_ISIZE_ALL)
    std::cout
        << "DSIZE=unsigned long long, ISIZE=unsigned long long:\n"
        << "\n";
    
    report_survey<unsigned long long,unsigned long long>(clock_info);
#endif


    // Always included
    std::cout
        << "DSIZE=unsigned long, ISIZE=unsigned long:\n"
        << "\n";
    
    report_survey<unsigned long,unsigned long>(clock_info);


#ifdef DSIZE_ALL_ISIZE_ALL
    std::cout
        << "DSIZE=double, ISIZE=unsigned long long:\n"
        << "\n";
    
    report_survey<double,unsigned long long>(clock_info);


    std::cout
        << "DSIZE=double, ISIZE=unsigned long:\n"
        << "\n";
    
    report_survey<double,unsigned long>(clock_info);


    std::cout
        << "DSIZE=double, ISIZE=unsigned int:\n"
        << "\n";
    
    report_survey<double,unsigned int>(clock_info);
#endif


#if ULONG_MAX == UINT_MAX || defined(DSIZE_ALL_ISIZE_ALL)
    std::cout
        << "DSIZE=unsigned int, ISIZE=unsigned int:\n"
        << "\n";
    
    report_survey<unsigned int,unsigned int>(clock_info);
#endif


#ifdef DSIZE_ALL_ISIZE_ALL
    std::cout
        << "DSIZE=float, ISIZE=unsigned int:\n"
        << "\n";
    
    report_survey<float,unsigned int>(clock_info);


    std::cout
        << "DSIZE=float, ISIZE=unsigned short:\n"
        << "\n";
    
    report_survey<float,unsigned short>(clock_info);


    std::cout
        << "DSIZE=float, ISIZE=short:\n"
        << "\n";
    
    report_survey<float,short>(clock_info);


    std::cout
        << "DSIZE=unsigned short, ISIZE=unsigned short:\n"
        << "\n";
    
    report_survey<unsigned short,unsigned short>(clock_info);


    std::cout
        << "DSIZE=unsigned short, ISIZE=short:\n"
        << "\n";
    
    report_survey<unsigned short,short>(clock_info);


    std::cout
        << "DSIZE=short, ISIZE=unsigned short:\n"
        << "\n";
    
    report_survey<short,unsigned short>(clock_info);


    std::cout
        << "DSIZE=short, ISIZE=short:\n"
        << "\n";
    
    report_survey<short,short>(clock_info);
#endif
}
catch(std::exception& e)
{
    std::cerr << e.what() << "\n";
    return 1;
}
catch(...)
{
    std::cerr << "not a std::exception exception\n";
    return 1;
}


char copyright_and_license_for_acpphint_kernelsurveyors_main[]
{
    "Context for this Copyright: acpphint_kernelsurveyors_main\n"
    "\n"
    "Copyright (c) 2015-2023 Mark Millard\n"
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
