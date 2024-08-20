//
//  cpp_clockinfo_main.cpp (c++20 currently)
//
//  Copyright (c) 2019-2021 Mark Millard
//
//  Permission to use, copy, modify, and distribute this software for any
//  purpose with or without fee is hereby granted, provided that the above
//  copyright notice and this permission notice appear in all copies.
//
//  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
//  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
//  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
//  SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
//  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
//  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
//  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//  Note:
//  This file is intended to be license-compatibile with producing, for
//  example, GPLv2-only licensed software when combined with such GPL
//  licensed software.
//

//
//  Started by Mark Millard on 2019-08-20.
//
//  Isolated test as a.out (line continuation implicit via indenting):
//      c++ -std=c++20 -pedantic -g -O3 -c cpp_thousandslocale.cpp
//      c++ -std=c++20 -pedantic -pthread -g -O3 -c cpp_clockinfo.cpp
//      c++ -std=c++20 -pedantic -pthread -g -O3 -DCPPCLOCKINFO_VERS='"test"'
//                                      cpp_thousandslocale.o cpp_clockinfo.o
//                                                      cpp_clockinfo_main.cpp
//

#include <cerrno>                 // for ERANGE, errno
#include <chrono>                 // for std::chrono::duration
#include <cstdlib>                // for strtoul, std::strtoul
#include <exception>              // for std::exception
#include <iomanip>                // for std::__1::operator<<, std::setw
#include <iostream>               // for std::__1::operator<<, std::basic_os...
#include <limits>                 // for std::numeric_limits
#include <vector>                 // for std::vector
#include <span>                   // for std::span
#include "cpp_clockinfo.h"        // for ClkInfo, ClkInfoFromThreads, HwConc...
#include "cpp_thousandslocale.h"  // for CppThousandsLocale

auto main(int argc, char const* argv[]) -> int
try
{
    std::span const args_span(argv,static_cast<unsigned int>(argc));
    if (args_span.empty() || nullptr==args_span[0U] || '\0'==*args_span[0U])
    {
        std::cout
            << "argv[0] does not provide program name\n";
        return 1;
    }

    HwConcurrencyCount requested_threads{0U}; // 0U means: get value from c++.
                                    // Controls threads used for threaded test
                                    // if optional argv[1] makes it positive.

    if (1U<args_span.size() && nullptr!=args_span[1U] && '\0'!=*args_span[1U])
    {
        constexpr int base_10{10};
        auto temp{std::strtoul(args_span[1U],nullptr,base_10)};
        if  (  ERANGE==errno
            || std::numeric_limits<HwConcurrencyCount>::max()<temp
            || 0U==temp
            )
        {
            std::cout
                << "argv[1] could not be converted to positive thread count\n";
            return 1;
        }

        requested_threads= temp;    // controls threads used for threaded test
                                    // if optional argv[1] makes it positive.
    }

    std::cout.imbue(CppThousandsLocale());

    std::cout
        << args_span[0U] << " . . .\n"
        << "cpp_clockinfo_main version: " CPPCLOCKINFO_VERS "\n"
        << "\n"
        << "NOTE: Producing the below output did NOT involve any INTERNAL CPU-lock-down activity.\n"
        << std::flush;

    ClkInfo clock_info{ClkInfo::DurationsStatus::Keep}; // Initially: just main thread

    std::cout
        << "\n"
        << "is_steady:                           "
        << std::boolalpha << ClkInfo::IsSteady
        << "\n"
        << "period 'resolution' : "
        << ClkInfo::nsFormatted(ClkInfo::Duration{1})
        << " ns\n"
        << "duration max        : "
        << ClkInfo::nsFormatted(ClkInfo::Duration::max())
        << " ns\n"
        << "now (from epoch)    : "
        << ClkInfo::nsFormatted(clock_info.Now().time_since_epoch())
        << " ns\n"
        << "\n"
        << "\n"
        << "main thread by itself:\n";

    auto const durations_report
    {
        []( auto const durations
          , auto const end_durations
          , double const percent99_of_end_durations
          ) -> void
        {
            auto const last_durations   {end_durations-1U};
            ClkInfo::DurationsCount distinct_value_pos{0U};
            ClkInfo::DurationsCount idx{0U};
            while (idx<end_durations)
            {
                ClkInfo::DurationsCount value_count{1};
                while(idx<last_durations && durations[idx]==durations[idx+1])
                {
                    ++value_count;
                    ++idx;
                }

                constexpr int value_pos_or_count_width{10};
                constexpr int nsFormatted_width       {14};
                constexpr int duration_width          {10};
                constexpr int duration_precision       {8};
                std::cout
                <<        std::setw(value_pos_or_count_width) << distinct_value_pos
                << " " << std::setw(value_pos_or_count_width) << value_count
                << " " << std::setw(nsFormatted_width) << ClkInfo::nsFormatted(durations.at(idx))
                << " " << std::setw(duration_width)
                       << std::setprecision(duration_precision) << std::fixed
                       << double(idx+1) / static_cast<double>(end_durations)
                      << std::defaultfloat
                << "\n";

                constexpr ClkInfo::DurationsCount value_count_threashold{10};
                if (  value_count<value_count_threashold
                   && percent99_of_end_durations < double(idx+1)
                   && idx+1 < end_durations
                   )
                {
                    std::cout << "(" << end_durations-idx-1 << " examples not summarized)\n";
                    break;
                }

                ++distinct_value_pos;
                ++idx;
            }
        }
    };

    auto const clock_info_report
    {
        [&durations_report](const ClkInfo& test_clock_info) -> void
        {
            std::cout
                << "\n";

            if (test_clock_info.ZeroDurationObserved())
            {
                std::cout
                    << "Clock tick is slow relative to execution,\n"
                    << "avoiding zero durations by spinning.\n";
            }
            else
            {
                std::cout
                    << "Clock tick is not slow relative to execution:\n"
                    << "no zero durations observed.\n";
            }

            constexpr int durations_width{0};
            std::cout
                << "\n" << std::setw(durations_width)
                << "Of " << test_clock_info.NumDurationsSampled()
                << " near 'back to back now()' durations:\n"
                << "\n"
                << "minimum duration                     : "
                << ClkInfo::nsFormatted(test_clock_info.DurationOverhead())
                << " ns\n"
                << "(to be subtracted out from activity durations)\n"
                << "\n"
                << "target approx. min activity duration  : "
                << ClkInfo::nsFormatted
                        (test_clock_info.TargetApproxMinDuration())
                << " ns\n"
                << "(with min duration still included)\n"
                << "\n"
                << "target with min duration subtracted  : "
                << ClkInfo::nsFormatted
                        (  test_clock_info.TargetApproxMinDuration()
                         - test_clock_info.DurationOverhead()
                        )
                << " ns\n"
                << "\n"
                << "Other information for reference:\n"
                << "\n"
                << "small scale duration variability     : "
                << ClkInfo::nsFormatted
                        (test_clock_info.SmallScaleDurationVariability())
                << " ns\n"
                << "\n"
                << "largest observed duration            : "
                << ClkInfo::nsFormatted(test_clock_info.LargestDuration())
                << " ns\n"
                << "(with min duration still included)\n"
                << "\n";

            auto const durations
                {test_clock_info.ObservedDurationsSortedWeaklyIncreasing()};
            auto const end_durations    {durations.size()};
            auto const percent99_of_end_durations {0.99*static_cast<double>(end_durations)};

            if (1<end_durations)
            {
//        1         2         3         4         5         6         7
//23456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
//3456789> <23456789> <23456789 123> <23456789> <23456789 123>
    std::cout << "\n\
      list  #Examples       duration   fraction\n\
  position  of durat.           (ns)    with <=\n\n";

                durations_report(durations, end_durations, percent99_of_end_durations);
            }

            std::cout.flush();
        }

    };

    clock_info_report(clock_info); // Main thread

    std::cout   << "\n"
                << "Other threads (parallel, if any):\n";

    auto threads_clock_infos
                { ClkInfoFromThreads( ClkInfo::DurationsStatus::Keep
                                    , requested_threads
                                    )
                };

    for (auto& thread_clock_info : threads_clock_infos)
    {
        clock_info_report(thread_clock_info);

        clock_info.Merge(thread_clock_info);
    }

    std::cout   << "\n"
                << "Overall:\n";

    clock_info_report(clock_info);
}
catch(std::exception& e)
{
    std::cerr << e.what() << "\n";
    return 1;
}
catch(...)
{
    std::cerr << "not a std::excpetion exception\n";
    return 1;
}

extern "C" char const copyright_and_license_for_cpp_clockinfo_main[]
{
    "Context for this Copyright: cpp_clockinfo_main\n"
    "\n"
    "Copyright (c) 2019-2021 Mark Millard\n"
    "\n"
    "Permission to use, copy, modify, and distribute this software for any\n"
    "purpose with or without fee is hereby granted, provided that the above\n"
    "copyright notice and this permission notice appear in all copies.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\n"
    "WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n"
    "MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY\n"
    "SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n"
    "WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION\n"
    "OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN\n"
    "CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n"
};
