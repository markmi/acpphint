//
//  cpp_clockinfo.h (c++20 currently)
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
//  Isolated test as a.out (line continuations implicit via indenting):
//      c++ -std=c++20 -pedantic -g -O3 -c cpp_thousandslocale.cpp
//      c++ -std=c++20 -pedantic -pthread -g -O3 -c cpp_clockinfo.cpp
//      c++ -std=c++20 -pedantic -pthread -g -O3 -DCPPCLOCKINFO_VERS='"test"'
//                                      cpp_thousandslocale.o cpp_clockinfo.o
//                                                      cpp_clockinfo_main.cpp
//  Just cpp_clockinfo.o (c++ may need to be some clang++*): 
//      c++ -std=c++20 -pedantic -pthread -g -O3 -c cpp_clockinfo.cpp
//

#ifndef cpp_clockinfo_guard
#define cpp_clockinfo_guard

#include <ratio>
#include <chrono>       // {steady,system,high_resolution}_clock's and related,
                        // duration. (All prior in std::chrono::)

#include <vector>       // vector and related
#include <string>       // string and related

#include <iostream>     // cerr and related

#include <system_error> // system_error, code, errc
#include <stdexcept>    // runtime_error
#include <new>          // bad_alloc

class ClkInfo
{
private:
    using SteadyClk     = std::chrono::steady_clock;
    using SystemClk     = std::chrono::system_clock;
    using HighResClk    = std::chrono::high_resolution_clock;
    // std::chrono::high_resolution_clock may be a synonym for one of
    // the other two.

public:
    enum struct DurationsStatus : unsigned {Forget, Keep};

    explicit ClkInfo(DurationsStatus status= DurationsStatus::Forget);
                    // Can throw runtime_error bad_alloc system_error . . .?
    
    ~ClkInfo()                                  = default;
    
    ClkInfo(ClkInfo const&)                     = delete;
    ClkInfo(ClkInfo&&)                          = default;
    
    auto operator=(ClkInfo const&) -> ClkInfo&  = delete;
    auto operator=(ClkInfo&&)      -> ClkInfo&  = delete;


    using UsedClk   = SteadyClk; // Edit as needed.
    
        
    auto Now() const -> auto { return UsedClk::now(); }

    
    using Duration  = typename UsedClk::duration;
    
    auto ZeroDurationObserved() const -> bool
                            { return zero_duration_observed; }
    // If observed, such zeros are avoided by spinning.
    
    auto DurationOverhead() const -> Duration
                            { return duration_overhead; }
    // Zero only if ZeroDurationObserved(), otherwise positive.
                            
    auto TargetApproxMinDuration() const -> Duration
                            { return target_approx_min_duration; }

    // Other information for the curious:
    
    auto SmallScaleDurationVariability() const -> Duration
                            { return small_scale_duration_variability; }
                            
    auto LargestDuration() const -> Duration
                            { return largest_duration; }

    using Durations         = std::vector<Duration>;
    using DurationsCount    = typename Durations::size_type;
                            
    auto NumDurationsSampled() const -> DurationsCount
                            { return num_durations_sampled; }
                            
    void Merge(const ClkInfo& OtherClkInfoExample);
        // Merge is partial:
        // Effectively changes the status to DurationsStatus::Forget.
        // In other words: ObservedDurationsSortedWeaklyIncreasing()
        // then returns an empty Durations.

    auto ObservedDurationsSortedWeaklyIncreasing() const -> Durations
                            { return observed_durations; }
                            // Empty unless DurationsStatus::Keep was used
                            // and there was no Merge use.
                            
    static auto constexpr IsSteady{UsedClk::is_steady};
    
    static auto nsFormatted(Duration const& d) -> std::string;
                           
private:
    Durations observed_durations;

    bool zero_duration_observed;

    Duration duration_overhead;
    Duration duration_spanning_large_percentile;
    Duration small_scale_duration_variability;
    Duration target_approx_min_duration;

    Duration largest_duration;
    
    DurationsCount num_durations_sampled;

    // Edit as needed, the 4 following:
    static auto constexpr percentile_scale{10u}; // Indicates 100%.
    static auto constexpr large_percentile{9u};  // So 90% covered.
    static auto constexpr scale_factor_for_activity_target  {100};

    // The general structucture is:
    //
    //    duration_overhead
    // == (min observed duration for [nearly] back-to-back now() calls)
    // (Possibly zero for clocks that are slow relative to execution.)
    //
    //    duration_spanning_large_percentile
    // == (observed duration at large_percentile coverage point)
    //
    //    small_scale_duration_variability
    // == duration_spanning_large_percentile - duration_overhead
    //
    //    target_approx_min_duration
    // ==   scale_factor_for_activity_target * small_scale_duration_variability

                                    // Edit as needed: Should not be small.
    static DurationsCount constexpr num_durations_sampled_per_instance{2'000'000u};
};

using HwConcurrencyCount = unsigned int; // Matching c++
extern auto ClkInfoFromThreads( ClkInfo::DurationsStatus durations_status
                              , HwConcurrencyCount requested_threads // 0u: get from c++
                              ) -> std::vector<ClkInfo>;

#endif // cpp_clockinfo_guard
