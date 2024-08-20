//
//  cpp_clockinfo.cpp (c++20 currently)
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
//  Just cpp_clockinfo.o (c++ may need to be some clang++*): 
//      c++ -std=c++20 -pedantic -pthread -g -O3 -c cpp_clockinfo.cpp
//

#include "cpp_clockinfo.h"
#include <algorithm>                    // for std::max, std::sort, std::min
#include <chrono>                       // for std::chrono::duration, std::c...
#include <future>                       // for std::future, std::packaged_task
#include <iostream>                     // for std::basic_ostream, std::__1:...
#include <iterator>                     // for std::next
#include <stdexcept>                    // for std::runtime_error
#include <string>                       // for std::char_traits, std::allocator
#include <system_error>                 // for std::system_error, std::errc
#include <thread>                       // for std::thread
#include <vector>                       // for std::vector
#include <sstream>                      // for std::ostringstream
#include <utility>                      // for std::move
#include "cpp_thousandslocale.h"        // for CppThousandsLocale

// Definitions for ClkInfo . . .

ClkInfo::ClkInfo(DurationsStatus durations_status) 
    : num_durations_sampled{num_durations_sampled_per_instance}
{
    static_assert
        ( !std::chrono::treat_as_floating_point<typename Duration::rep>::value
        , "The source code only designed for UsedClk::duration.count()"
          " being integral"
        );

    auto const old{std::cerr.flags()};

    auto const cleaner{ [old]() -> void {std::cerr.flags(old);} };

    struct end_scope_cleaner
    {
        explicit end_scope_cleaner(decltype(cleaner) const& clnr) : clean_with(clnr) {}
        ~end_scope_cleaner()                                          { clean_with(); }

        end_scope_cleaner()                                             = delete;
        end_scope_cleaner(end_scope_cleaner&&)                          = delete;
        auto operator=(const end_scope_cleaner&&) -> end_scope_cleaner& = delete;
        end_scope_cleaner(const end_scope_cleaner&)                     = delete;
        auto operator=(const end_scope_cleaner&)  -> end_scope_cleaner& = delete;

    private:
        decltype(cleaner) const& clean_with;
    } const end_scope_clean_with{cleaner};

    using TimePoint = typename UsedClk::time_point;

    // Start investigation with a small sampling for basic classification
    std::vector<TimePoint> back_to_back_tps
    {
        { Now()
        , Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        , Now()
        , Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        , Now()
        , Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        , Now()
        , Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        ,   Now()
        , Now()
        , Now()
        }
    };

    observed_durations.reserve(back_to_back_tps.size()-1U);

    auto const find_and_sort_deltas
    {
        [&back_to_back_tps,this](auto order_test, const char* throw_msg)
        {
            observed_durations.clear();

            auto prev_tp{back_to_back_tps.cbegin()};
            auto at_tp  {std::next(prev_tp)};
            for (
                ; at_tp != back_to_back_tps.cend()
                ; prev_tp = at_tp++
                )
            {
                if (order_test(*prev_tp,*at_tp))
                {
                    std::cerr
                        << "time out of required order:\n"
                        << "time was   : "
                        << nsFormatted((*prev_tp).time_since_epoch())
                        << " ns\n"
                        << "but became : "
                        << nsFormatted((*at_tp).time_since_epoch())
                        << " ns\n";
                    throw std::runtime_error(throw_msg);
                }

                observed_durations.emplace_back(*at_tp - *prev_tp);
            }

            std::sort
                (std::begin(observed_durations), std::end(observed_durations));
        }
    };

    auto const backwards_failure_test
    {
        [](UsedClk::time_point before, UsedClk::time_point after) -> bool
        { return after < before; }
    };

    find_and_sort_deltas
        ( backwards_failure_test
        , "now() did not produce all-non-negative preliminary back_to_back_tps deltas"
        );

    zero_duration_observed= observed_durations.at(0U) == Duration::zero();

    back_to_back_tps    .clear();
    observed_durations  .clear();
    back_to_back_tps    .reserve(num_durations_sampled_per_instance+1U);
    observed_durations  .reserve(num_durations_sampled_per_instance);

    auto const progress_failure_test
    {
        [](UsedClk::time_point before, UsedClk::time_point after) -> bool
        { return after <= before; }
    };

    duration_overhead= Duration::zero();

    if (!zero_duration_observed)
    {
        for (auto i{back_to_back_tps.capacity()}; 0<i; --i)
        {
            back_to_back_tps.emplace_back(Now());
        }

        try
        {
            find_and_sort_deltas
                ( progress_failure_test
                , "now() did not produce all-positive deltas for back_to_back_tps"
                );

            duration_overhead= observed_durations.at(0U);
        }
        catch (const std::runtime_error&)
        {
            back_to_back_tps    .clear();
            observed_durations  .clear();
            zero_duration_observed= true;
        }
    }

    // Deal with clocks that change slowly (min delta of no time difference)
    // by instead explicitly spinning until times change. Otherwise do not
    // spin to find such.
    if (zero_duration_observed)
    {
        auto prev_tp{Now()};
        for (auto i{back_to_back_tps.capacity()}; 0<i; --i)
        {            
            auto new_tp{Now()};
            while (new_tp == prev_tp)
                { new_tp= Now(); }

            back_to_back_tps.emplace_back(prev_tp);
            prev_tp = new_tp;
        }

        find_and_sort_deltas
            ( progress_failure_test
            , "now() equality-looping got non-positive deltas for back_to_back_tps"
            );

        duration_overhead= Duration::zero();
    }

    auto large_percentile_at
        {(large_percentile*(observed_durations.size()-1U))/percentile_scale};

    duration_spanning_large_percentile
                                = observed_durations.at(large_percentile_at);

    // Avoid odd 0U == small_scale_duration_variability if we can:
    while (  duration_spanning_large_percentile == duration_overhead
          && large_percentile_at < observed_durations.size()-1U
          )
        {
            large_percentile_at++;
            duration_spanning_large_percentile
                                = observed_durations.at(large_percentile_at);

        }

    small_scale_duration_variability
        = duration_spanning_large_percentile-duration_overhead;

    target_approx_min_duration
        =  scale_factor_for_activity_target
         * std::max(small_scale_duration_variability,duration_overhead);


    largest_duration= observed_durations.back();

    if (DurationsStatus::Forget == durations_status)
    {
        observed_durations.clear();
        observed_durations.shrink_to_fit();
    }
}

void ClkInfo::Merge(const ClkInfo& OtherClkInfoExample)
{
    zero_duration_observed
        =   zero_duration_observed
         || OtherClkInfoExample.zero_duration_observed;

    duration_overhead
        = std::min  ( duration_overhead
                    , OtherClkInfoExample.duration_overhead
                    );
    duration_spanning_large_percentile
        = std::max  ( duration_spanning_large_percentile
                    , OtherClkInfoExample.duration_spanning_large_percentile
                    );
    small_scale_duration_variability
        = std::max  ( small_scale_duration_variability
                    , OtherClkInfoExample.small_scale_duration_variability
                    );
    target_approx_min_duration
        = std::max  ( target_approx_min_duration
                    , OtherClkInfoExample.target_approx_min_duration
                    );
    largest_duration
        = std::max  ( largest_duration
                    , OtherClkInfoExample.largest_duration
                    );

    num_durations_sampled+= OtherClkInfoExample.num_durations_sampled;

    // Effectively change the status to DurationsStatus::Forget.
    // In other words: ObservedDurationsSortedWeaklyIncreasing()
    // then returns an empty Durations value.
    observed_durations.clear();
    observed_durations.shrink_to_fit();
}

auto ClkInfo::nsFormatted(Duration const& dur) -> std::string
{
    std::ostringstream out{};
    out.imbue(CppThousandsLocale());

    out << std::chrono::duration_cast<std::chrono::nanoseconds>(dur).count();

    return out.str();
}

auto ClkInfoFromThreads( ClkInfo::DurationsStatus durations_status
                       , HwConcurrencyCount requested_threads // 0U: get from c++
                       ) -> std::vector<ClkInfo>
{
    if (0U == requested_threads)
        { requested_threads= std::thread::hardware_concurrency(); }

    auto parallel_code{[durations_status](){return ClkInfo{durations_status};}};

    std::vector<std::future<ClkInfo>> parallel_tasks_future{};
    parallel_tasks_future.reserve(requested_threads);

    std::vector<std::thread> parallel_tasks_thread{};
    // Use std::jthread when clang allows it.
    parallel_tasks_thread.reserve(requested_threads);

    for(auto threads_to_do{requested_threads}; 0U<threads_to_do; --threads_to_do)
    {
        try
        {
            std::packaged_task parallel_task{parallel_code};
            auto parallel_task_future{parallel_task.get_future()};

            parallel_tasks_future.emplace_back(std::move(parallel_task_future));
            parallel_tasks_thread.emplace_back(std::move(parallel_task));
        }
        catch(std::system_error& e)
        {
            if (e.code() == std::errc::resource_unavailable_try_again)
                { break; }

            for(auto& parallel_task_thread : parallel_tasks_thread)
            {
                parallel_task_thread.join();
            }
            throw;
        }
    }

    for(auto& parallel_task_future : parallel_tasks_future)
    {
        parallel_task_future.wait();
    }

    std::vector<ClkInfo> threads_clock_infos{};
    threads_clock_infos.reserve(parallel_tasks_future.size());

    for(auto& parallel_task_future : parallel_tasks_future)
    {
        threads_clock_infos.emplace_back(parallel_task_future.get());
    }

    for(auto& parallel_task_thread : parallel_tasks_thread)
    {
        parallel_task_thread.join();
    }

    return threads_clock_infos;
}

extern "C" char const copyright_and_license_for_cpp_clockinfo[]
{
    "Context for this Copyright: cpp_clockinfo\n"
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
