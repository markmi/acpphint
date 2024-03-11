//
//  acpphint_kernelsamplers_main.cpp
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

//
// acpphint_kernelsamplers_main is used to see what kernel
// sampler results are like, both for testing and for seeing
// if any involved "Edit as needed" items need adjustment.
// (But that last likely requires adjusting the code for
// interesting issues.)
//
// Adjust thread_count at its "Edit as needed" place below.
// The default has been set to 3 when at least that many
// threads are available.
//
// The non-small tests are generally in a #if 0 . . . #endif .
//
// Also large thread_counts can not handle smaller DSIZE
// or ISIZE types like unsigned short or short, given
// NCHUNK: scx=initial_dx*nproc*NCHUCK with initial_dx
// being a integral (non-negative) power of 2. It just
// does not have enough room for nscout<initial_dx to fit.
//

#include <exception>                  // for std::exception
#include <iostream>                   // for std::__1::operator<<, std::basi...
#include <span>                       // for std::span
#include "acpphint_kernels.h"         // for PrimaryKernelInputs
#include "acpphint_kernelrunners.h"   // for TIME_PARALLEL_THREAD_CREATION_TOO
#include "acpphint_kernelsamplers.h"  // for KernelSampler, (indirectly) dsize_all_isize_all
#include "cpp_clockinfo.h"            // for ClkInfo, HwConcurrencyCount
#include "cpp_thousandslocale.h"      // for CppThousandsLocale
#include "sys_cpubinding.h"           // for ConcurrencyCountForInDomains

namespace
{
    template<typename DSIZE,typename ISIZE>
    void report_sampler(ClkInfo const& clock_info)
    {
        std::cout
        << "serial. . .\n"
        << "\n" << std::flush;

        PrimaryKernelInputs<DSIZE,ISIZE> const ki_serial(1U);

        std::cout
            << "nproc:      " << ki_serial.nproc << "\n"
            << "scx:        " << ki_serial.scx << "\n"
            << "scy:        " << ki_serial.scy << "\n"
            << "dmax:       " << ki_serial.dmax << "\n"
            << "initial_dx: " << ki_serial.initial_dx << "\n"
            << "\n" << std::flush;

        auto const ks_serial_result{KernelSampler<DSIZE,ISIZE,TIME_PARALLEL_THREAD_CREATION_TOO>
                                    (clock_info,ki_serial)};

        for (auto const& ksr : ks_serial_result)
        {
            if (0 == ksr.quips)
                { std::cout
                    << "\n"
                    << "stopped with, n:                    " << ksr.n << "\n"
                    << "ksr.kernel_result.eflag:            "
                    << static_cast<int>(ksr.run_result.kernel_result.eflag)
                    << "\n"
                    << "\n" << std::flush;
                }
            else
                { std::cout
                    << ksr.run_result.median_mean_sec_per_lap.count() << " "
                    << ksr.quips << " "
                    <<       ksr.run_result.median_mean_sec_per_lap.count()
                       * ksr.quips
                    << " "
                    << ksr.run_result.vectors_total_bytes << "\n";
            }
        }

        if (!ks_serial_result.empty())
            { std::cout
                << "\n"
                << ks_serial_result.back().how_stopped_notes << std::flush;
            }

        HwConcurrencyCount thread_count{ConcurrencyCountForInDomains()};

        if (thread_count<2U) { return; }

        if (3U<thread_count) { thread_count= 3U; }

        std::cout
            << "\n"
            << "threaded. . .\n"
            << "\n" << std::flush;

        PrimaryKernelInputs<DSIZE,ISIZE> const ki_parallel(thread_count);

        std::cout
            << "nproc:          " << ki_parallel.nproc << "\n"
            << "cx:            " << ki_parallel.scx << "\n"
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

        auto const ks_parallel_result{KernelSampler<DSIZE,ISIZE,TIME_PARALLEL_THREAD_CREATION_TOO>
                                      (clock_info,ki_parallel)};

        for (auto const& ksr : ks_parallel_result)
        {
            if (0 == ksr.quips)
                { std::cout
                    << "\n"
                    << "stopped with, n:                    " << ksr.n << "\n"
                    << "ksr.kernel_result.eflag:            "
                        << static_cast<int>(ksr.run_result.kernel_result.eflag)
                        << "\n"
                    << "\n" << std::flush;
                }
            else
                { std::cout
                    << ksr.run_result.median_mean_sec_per_lap.count() << " "
                    << ksr.quips << " "
                    <<       ksr.run_result.median_mean_sec_per_lap.count()
                           * ksr.quips
                        << " "
                    << ksr.run_result.vectors_total_bytes << "\n";
                }
        }

        if (!ks_parallel_result.empty())
            { std::cout
                << "\n"
                << ks_parallel_result.back().how_stopped_notes << std::flush;
            }
    }
};

auto main(int argc, const char* argv[]) -> int
try
{
    std::span const args_span(argv,argc);
    if (args_span.empty() || nullptr==args_span[0U] || '\0'==*args_span[0U])
    {
        std::cout
            << "argv[0] does not provide program name\n";
        return 1;
    }

    std::cout
        << args_span[0U] << " . . .\n"
        << "acpphint_kernelsamplers_main version: "
           ACPPHINTKERNELSAMPLERS_VERS "\n"
        << "\n" << std::flush;


    std::cout.imbue(CppThousandsLocale());

    std::cout
        << args_span[0U] << " . . .\n"
        << "\n";

    ClkInfo clock_info{};

// Edit as needed to add more alternatives (or disable some):
    if constexpr (false) // NOLINT(readability-simplify-boolean-expr)
    {
        if constexpr (dsize_all_isize_all)
        {
            std::cout
                << "\n"
                << "DSIZE=long double, ISIZE=unsigned long long:\n"
                << "\n";

            report_sampler<long double,unsigned long long>(clock_info);

            std::cout
                << "\n"
                << "DSIZE=long double, ISIZE=unsigned long:\n"
                << "\n";

            report_sampler<long double,unsigned long>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=unsigned long long, ISIZE=unsigned long long:\n"
                << "\n";

            report_sampler<unsigned long long,unsigned long long>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=unsigned long, ISIZE=unsigned long:\n"
                << "\n";

            report_sampler<unsigned long,unsigned long>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=double, ISIZE=unsigned long long:\n"
                << "\n";

            report_sampler<double,unsigned long long>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=double, ISIZE=unsigned long:\n"
                << "\n";

            report_sampler<double,unsigned long>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=double, ISIZE=unsigned int:\n"
                << "\n";

            report_sampler<double,unsigned int>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=unsigned int, ISIZE=unsigned int:\n"
                << "\n";

            report_sampler<unsigned int,unsigned int>(clock_info);


            std::cout
                << "\n"
                << "DSIZE=float, ISIZE=unsigned int:\n"
                << "\n";

            report_sampler<float,unsigned int>(clock_info);
        }
    }

    std::cout
        << "\n"
        << "DSIZE=float, ISIZE=unsigned short:\n"
        << "\n";

    report_sampler<float,unsigned short>(clock_info);


    std::cout
        << "\n"
        << "DSIZE=float, ISIZE=short:\n"
        << "\n";

    report_sampler<float,short>(clock_info);


    std::cout
        << "\n"
        << "DSIZE=unsigned short, ISIZE=unsigned short:\n"
        << "\n";

    report_sampler<unsigned short,unsigned short>(clock_info);


    std::cout
        << "\n"
        << "DSIZE=unsigned short, ISIZE=short:\n"
        << "\n";

    report_sampler<unsigned short,short>(clock_info);


    std::cout
        << "\n"
        << "DSIZE=short, ISIZE=unsigned short:\n"
        << "\n";

    report_sampler<short,unsigned short>(clock_info);


    std::cout
        << "\n"
        << "DSIZE=short, ISIZE=short:\n"
        << "\n";

    report_sampler<short,short>(clock_info);
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


extern "C" char const copyright_and_license_for_acpphintkernelsurveyors_main[]
{
    "Context for this Copyright: acpphint_kernelsurveyors_main\n"
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
