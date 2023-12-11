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

//#include <__chrono/duration.h>    // for std::chrono::operator-, std::chrono...
//#include <__type_traits/decay.h>  // for std::__decay_t
#include <cerrno>                 // for ERANGE, errno
#include <chrono>                 // for std::chrono::duration
#include <cstdlib>                // for strtoul, std::strtoul
#include <exception>              // for std::exception
#include <iomanip>                // for std::__1::operator<<, std::setw
#include <iostream>               // for std::__1::operator<<, std::basic_os...
#include <limits>                 // for std::numeric_limits
#include <string>                 // for std::basic_string
#include <vector>                 // for std::vector
#include "cpp_clockinfo.h"        // for ClkInfo, ClkInfoFromThreads, HwConc...
#include "cpp_thousandslocale.h"  // for CppThousandsLocale

static HwConcurrencyCount requested_threads{0u}; // 0u: get from c++
                                    // controls threads used for threaded test
                                    // if optional argv[1] makes it positive.

int main(int argc, const char* argv[])
try
{
    if (0==argc || nullptr==argv[0] || '\0'==*argv[0])
    {
        std::cout
            << "argv[0] does not provide program name\n";
        return 1;
    }

    if (1<argc && nullptr!=argv[1] && '\0'!=*argv[1])
    {
        auto temp{std::strtoul(argv[1],nullptr,10)};
        if  (  ERANGE==errno
            || std::numeric_limits<HwConcurrencyCount>::max()<temp
            || 0u==temp
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
        << argv[0u] << " . . .\n"
        << "cpp_clockinfo_main version: " CPPCLOCKINFO_VERS "\n"
        << "\n"
        << "NOTE: Producing the below output did NOT involve any INTERNAL CPU-lock-down activity.\n"
        << std::flush;

    auto clock_info_report
    {
        [](const ClkInfo& test_clock_info) -> void
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

            std::cout
                << "\n"
                << "Of " << std::setw(0) << test_clock_info.NumDurationsSampled()
                << " near 'back to back now()' durations:\n"
                << "\n"
                << "minimum duration                     : "
                << ClkInfo::nsFormatted(test_clock_info.DurationOverhead())
                << " ns\n"
                << "(to be subtracted out from activity durations)\n"
                << "\n"
                << "target approx. min actvity duration  : "
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
            auto const percent99_of_end_durations {0.99*end_durations};

            if (1<end_durations)
            {
//        1         2         3         4         5         6         7
//23456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
//3456789> <23456789> <23456789 123> <23456789> <23456789 123>
    std::cout << "\n\
      list  #Examples       duration   fraction\n\
  position  of durat.           (ns)    with <=\n\n";

                auto const last_durations   {end_durations-1u};
                ClkInfo::DurationsCount distinct_value_pos{0u};
                for(ClkInfo::DurationsCount i{0u}; i<end_durations; ++i)
                {        
                    ClkInfo::DurationsCount value_count{1};
                    while(i<last_durations && durations[i]==durations[i+1])
                    {
                        ++value_count;
                        ++i;
                    }

                    std::cout
                    <<        std::setw(10) << distinct_value_pos
                    << " " << std::setw(10) << value_count
                    << " " << std::setw(14) << ClkInfo::nsFormatted(durations.at(i))
                    << " " << std::setw(10) << std::setprecision(8) << std::fixed
                           << double(i+1) / end_durations
                           << std::defaultfloat
                    << "\n";

                    if (  value_count<10
                       && percent99_of_end_durations < double(i+1)
                       && i+1 < end_durations
                       )
                    {
                        std::cout << "(" << end_durations-i-1 << " examples not summarized)\n";
                        break;
                    }

                    ++distinct_value_pos;
                 }
            }

            std::cout.flush();
        }

    };

    ClkInfo clock_info{ClkInfo::DurationsStatus::Keep}; // Initially: just main thread

    std::cout
        << "\n"
        << "is_steady:                           "
        << std::boolalpha << clock_info.IsSteady
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

char copyright_and_license_for_cpp_clockinfo_main[]
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
