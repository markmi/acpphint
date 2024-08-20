//
//  acpphint_main.cpp
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
// acpphint_main is the benchmark source but writes out
// files for gnuplot for mulltiple DSIZE/ISIZE data type
// pairs. Executable names may encode what compiler/library
// combination was used and likely drop the _main suffix.
//
// Adjustments are made to the source files for the .o
// files that are linked in with accphint_main.
//
// Also large thread_counts can not handle smaller DSIZE
// or ISIZE types like unsigned short or short, given
// NCHUNK: scx=initial_dx*nproc*NCHUCK with initial_dx
// being a integral (non-negative) power of 2. It just
// does not have enough room for nscout<initial_dx to fit.
// Thus some combinations may have no data to plot.
//

#include <algorithm>                 // for std::max
#include <cerrno>                    // for ERANGE, errno
#include <climits>                   // for ULONG_MAX, UINT_MAX, ULLONG_MAX
#include <cstdlib>                   // for strtoul, std::strtoul
#include <exception>                 // for std::exception
#include <iostream>                  // for std::__1::operator<<, std::basi...
#include <fstream>                   // open and related.
#include <limits>                    // for std::numeric_limits
#include <stdexcept>                 // for std::runtime_error
#include <string>                    // for std::char_traits, std::allocator
#include <span>                      // for std::span
#include "acpphint_kernels.h"        // for PrimaryKernelInputs, KernelResults
#include "acpphint_kernelrunners.h"  // for TIME_PARALLEL_THREAD_CREATION_TOO
#include "acpphint_kernelsamplers.h" // for KernelSampler, (indirectly) dsize_all_isize_all
#include "../other_src_used/cpp_clockinfo.h"           // for HwConcurrencyCount, ClkInfo
#include "../other_src_used/sys_cpubinding.h"          // for ConcurrencyCountForInDomains

namespace
{
    template<typename DSIZE,typename ISIZE, bool want_parallel_thread_creation_time_included>
    void report_samples  ( std::string                       const& filename
                         , ClkInfo                           const& clock_info
                         , PrimaryKernelInputs<DSIZE,ISIZE>  const& ki // NOLINT(readability-identifier-length)
                         )
    {
        std::cout
            << "generating: " << filename << "\n" << std::flush;

        auto const ks_result{KernelSampler<DSIZE,ISIZE,want_parallel_thread_creation_time_included>
                                 (clock_info,ki)};

        if (ks_result.empty())
        {
            std::cout
                << "empty result for "+filename+" so no file.\n" << std::flush;
        }
        else if (0 == ks_result.front().quips)
        {
            std::cout
                << "empty result for "+filename+" so no file.\n"
                << "note: ksr.kernel_result.eflag: "
                    << static_cast<int>
                                (ks_result.front().run_result.kernel_result.eflag)
                    << "\n"
                << "\n" << std::flush;
        }
        else
        {
            std::cout << ks_result.back().how_stopped_notes << std::flush;

            std::ofstream gnuplot_data(filename);

            if (gnuplot_data.fail())
            {
                throw std::runtime_error("unable to open: "+filename);
                return;
            }

            std::cout
                << "writing:    " << filename << "\n" << std::flush;

            gnuplot_data    << "# filename: " << filename << "\n"
                            << "# acpphint_main version: " ACPPHINT_VERS "\n"
                            << "\n" << std::flush;

            for (auto const& ksr : ks_result)
            {
                if (0 == ksr.quips)
                {
                    std::cout
                        << "stopped with, n:                    " << ksr.n << "\n"
                        << "ksr.kernel_result.eflag:            "
                            << static_cast<int>(ksr.run_result.kernel_result.eflag)
                            << "\n"
                        << "\n" << std::flush;
                }
                else
                {
                    gnuplot_data
                        << ksr.run_result.median_mean_sec_per_lap.count() << " "
                        << ksr.quips << " "
                        <<   ksr.run_result.median_mean_sec_per_lap.count()
                           * ksr.quips
                            << " "
                        << ksr.run_result.vectors_total_bytes << "\n";

                    if  (   KernelResults<DSIZE,ISIZE>::EFlag::DISCRET
                         == ksr.run_result.kernel_result.eflag
                        )
                    {
                        std::cout
                            << "stopped with, n:                    " << ksr.n
                            << "\n"
                            << "ksr.kernel_result.eflag:            DISCRET ("
                            << std::to_string(static_cast<int>(ksr.run_result.kernel_result.eflag))
                            << ")\n" << std::flush;

                    }
                }
            }
        }
    }

    // NOLINTBEGIN(clang-diagnostic-unused-const-variable)
    template<typename DSIZE>
    char const * const DSIZE_text{"DSIZE_unknown"};

    // Edit as needed (add more?):
    template<> char const * const DSIZE_text<short>                {"DSIZE_s"};
    template<> char const * const DSIZE_text<unsigned short>       {"DSIZE_us"};
    template<> char const * const DSIZE_text<int>                  {"DSIZE_i"};
    template<> char const * const DSIZE_text<unsigned int>         {"DSIZE_ui"};
    template<> char const * const DSIZE_text<long>                 {"DSIZE_l"};
    template<> char const * const DSIZE_text<unsigned long>        {"DSIZE_ul"};
    template<> char const * const DSIZE_text<long long>            {"DSIZE_ll"};
    template<> char const * const DSIZE_text<unsigned long long>   {"DSIZE_ull"};
    template<> char const * const DSIZE_text<float>                {"DSIZE_f"};
    template<> char const * const DSIZE_text<double>               {"DSIZE_d"};
    template<> char const * const DSIZE_text<long double>          {"DSIZE_ld"};

    template<typename ISIZE>
    char const * const ISIZE_text{"ISIZE_unknown"};

    // Edit as needed (add more?):
    template<> char const * const ISIZE_text<short>                {"ISIZE_s"};
    template<> char const * const ISIZE_text<unsigned short>       {"ISIZE_us"};
    template<> char const * const ISIZE_text<int>                  {"ISIZE_i"};
    template<> char const * const ISIZE_text<unsigned int>         {"ISIZE_ui"};
    template<> char const * const ISIZE_text<long>                 {"ISIZE_l"};
    template<> char const * const ISIZE_text<unsigned long>        {"ISIZE_ul"};
    template<> char const * const ISIZE_text<long long>            {"ISIZE_ll"};
    template<> char const * const ISIZE_text<unsigned long long>   {"ISIZE_ull"};
    // NOLINTEND(clang-diagnostic-unused-const-variable)

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    std::string filename_suffix{".txt"}; // NOLINT(cert-err58-cpp)
                                         // controls file name suffix.
                                         // optional argv[1] can change it.
    // NOTE: The unlikely throws need not be caught for constructing it.

    HwConcurrencyCount min_requested_threads{0U};
                                        // controls report_varying_threading
                                        // if optional argv[2] makes it positive.

    HwConcurrencyCount max_requested_threads{0U};
                                        // controls report_varying_threading
                                        // if optional argv[3] makes it no less
                                        // than min_requested_threads.
                                        // Defaults to min_requested_threads
                                        // when that is positive.
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    template<typename DSIZE,typename ISIZE>
    void report_varying_threading
                                ( std::string const&    filename_prefix
                                , ClkInfo const&        clock_info
                                )
    {
        std::string const longer_filename_prefix
                {filename_prefix+"-"+DSIZE_text<DSIZE>+"-"+ISIZE_text<ISIZE>+"-"};

        std::string filename    {  longer_filename_prefix
                                 + "using_1_threads"
                                 + filename_suffix
                                };

        if (min_requested_threads<2U) // includes 0U (for unspecified)
        { // Deliberately deal with single-thread first.
            PrimaryKernelInputs<DSIZE,ISIZE> const ki_serial(1U);

            report_samples<DSIZE,ISIZE,!TIME_PARALLEL_THREAD_CREATION_TOO>(filename,clock_info,ki_serial);
        }

        HwConcurrencyCount thread_count{ConcurrencyCountForInDomains()};

        if (thread_count<1U) { return; }

        HwConcurrencyCount min_threads{3U};
        if (0U!=min_requested_threads)
            { min_threads= std::max(2U,min_requested_threads); } // 1U already handled.

        if (0U!=max_requested_threads)
            { thread_count= max_requested_threads; }

        while (min_threads<=thread_count) {
            PrimaryKernelInputs<DSIZE,ISIZE> const ki_parallel(thread_count);

            filename=  longer_filename_prefix;
            filename+= "using_"+std::to_string(thread_count)+"_threads";
            filename+= filename_suffix;

            report_samples<DSIZE,ISIZE,TIME_PARALLEL_THREAD_CREATION_TOO>(filename,clock_info,ki_parallel);

            filename=  longer_filename_prefix;
            filename+= "using_"+std::to_string(thread_count)+"_threads_creation_untimed";
            filename+= filename_suffix;

            report_samples<DSIZE,ISIZE,!TIME_PARALLEL_THREAD_CREATION_TOO>(filename,clock_info,ki_parallel);

            thread_count/= 2U; // No alternate policy for this.
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
            << "argv[0] did not provide a program name\n";
        return 1;
    }

    std::cout
        << args_span[0U] << " . . .\n"
        << "acpphint_main version: " ACPPHINT_VERS "\n"
        << "\n" << std::flush;

    if (1<args_span.size() && nullptr!=args_span[1U])
    {
        filename_suffix= args_span[1U]; // replaces ".txt"
    }

    constexpr int requested_threads_base{10};

    if (2<args_span.size() && nullptr!=args_span[2U] && '\0'!=*args_span[2U])
    {
        auto temp{std::strtoul(args_span[2U],nullptr,requested_threads_base)};
        if  (  ERANGE==errno
            || std::numeric_limits<HwConcurrencyCount>::max()<temp
            || 0U==temp
            )
        {
            std::cout
                << "argv[2] could not be converted to positive thread count\n";
            return 1;
        }

        min_requested_threads= temp;    // controls report_varying_threading
                                        // if argv[2] makes it positive.
        max_requested_threads= min_requested_threads; // default
    }

    if (3<args_span.size() && nullptr!=args_span[3U] && '\0'!=*args_span[3U])
    {
        auto temp{std::strtoul(args_span[3U],nullptr,requested_threads_base)};
        if  (  ERANGE==errno
            || std::numeric_limits<HwConcurrencyCount>::max()<temp
            || 0U==temp
            )
        {
            std::cout
                << "argv[3] could not be converted to positive thread count\n";
            return 1;
        }

        max_requested_threads= temp;    // controls report_varying_threading
                                        // if argv[3] makes it positive.

        if (max_requested_threads<min_requested_threads)
        {
            std::cout
                << "max requested threads < min requested threads\n";
            return 1;
        }
    }

    std::cout
        << args_span[0U]
            << " [alternate-filename-suffix [min threads [max threads]]]\n"
        << "\n";

    std::string const filename_prefix{args_span[0U]};

    ClkInfo clock_info{};

    // Edit as needed to add more alternatives (or disable some):

    if constexpr (dsize_all_isize_all) {
        report_varying_threading<short,short>         (filename_prefix,clock_info);
        report_varying_threading<short,unsigned short>(filename_prefix,clock_info);

        report_varying_threading<unsigned short,short>(filename_prefix,clock_info);
        report_varying_threading<unsigned short,unsigned short>
                                                      (filename_prefix,clock_info);

        report_varying_threading<float,short>         (filename_prefix,clock_info);
        report_varying_threading<float,unsigned short>(filename_prefix,clock_info);
        report_varying_threading<float,unsigned int>  (filename_prefix,clock_info);
    }

    if constexpr (ULONG_MAX == UINT_MAX || dsize_all_isize_all) {
        report_varying_threading<unsigned int,unsigned int>
                                                  (filename_prefix,clock_info);
    }

    if constexpr (dsize_all_isize_all) {
        report_varying_threading<double,unsigned int> (filename_prefix,clock_info);
        report_varying_threading<double,unsigned long>(filename_prefix,clock_info);
        report_varying_threading<double,unsigned long long>
                                                      (filename_prefix,clock_info);
    }

    // Always included
    report_varying_threading<unsigned long,unsigned long>
                                                  (filename_prefix,clock_info);

        report_varying_threading<unsigned long long,unsigned long long>
                                                  (filename_prefix,clock_info);

    if constexpr (dsize_all_isize_all) {
        report_varying_threading<long double,unsigned long>
                                                  (filename_prefix,clock_info);
        report_varying_threading<long double,unsigned long long>
                                                  (filename_prefix,clock_info);
    }
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


extern "C" char const copyright_and_license_for_acpphint_main[]
{
    "Context for this Copyright: acpphint_main\n"
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
