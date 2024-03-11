//
//  sys_cpubinding.cpp (c++20 currently)
//
//  Copyright (c) 2019-2024 Mark Millard
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
//  Started by Mark Millard on 2019-09-12.
//

#include "sys_cpubinding.h"
#include <stdexcept>  // for std::runtime_error
#include <thread>     // for std::thread

#if defined(__FreeBSD__) && defined(WANT_CPU_LOCKDOWN)
#include <sys/types.h>      // for cpusetid_t, id_t
#include <sys/_domainset.h> // for domainset_t
#include <sys/domainset.h>  // for DOMAINSET_* macro-based operations and such
#include <sys/_cpuset.h>    // for cpuset_t
#include <sys/cpuset.h>     // for CPU_* macro-based operations and such

#include <vector>
#include <iostream>
#include <string>

struct cpu_and_domain_sets
{
    cpuset_t    cpu_set{};
    domainset_t domain_set{}; // always a singleton set
};

struct sys_cpubinding
{
// NOTE: Only instance is const after construction
// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    cpusetid_t cpusetid{};

    std::vector<cpu_and_domain_sets> singleton_sets{}; // Indexed by cpu_num. // NOLINT(readability-redundant-member-init)

    unsigned int domain_count{0U};
    unsigned int concurrency_count_for_in_domains{0U}; // Matching c++ type
// NOLINTEND(misc-non-private-member-variables-in-classes)

private:
// NOLINTBEGIN(readability-function-cognitive-complexity)
    auto domains_and_domain_cpu_sets(auto const& dom_set,auto const& cpu_set,auto& domain_info) -> void
    {
        for (id_t dnum{0U}; dnum<DOMAINSET_SETSIZE; ++dnum)
        {
            if (DOMAINSET_ISSET(dnum,&dom_set))
            {
                cpuset_t dcpu_set;

                if  (0 != cpuset_getaffinity( CPU_LEVEL_WHICH
                                            , CPU_WHICH_DOMAIN
                                            , dnum
                                            , sizeof(dcpu_set)
                                            , &dcpu_set
                                            )
                    )
                    { throw std::runtime_error("failed to identify an affinity"); }

                cpuset_t filtered_dcpu_set;
                CPU_COPY(&dcpu_set, &filtered_dcpu_set);
                CPU_AND(&filtered_dcpu_set, &filtered_dcpu_set, &cpu_set);

                unsigned int const domain_concurrency_count= CPU_COUNT(&filtered_dcpu_set);

                if (0U == domain_concurrency_count) { continue; }

                // Positive domain_concurrency_count . . .

                concurrency_count_for_in_domains+= domain_concurrency_count;

                std::cout << "os domain " << std::to_string(dnum) << " filtered affinity #cpus: "
                          << std::to_string(domain_concurrency_count)
                          << "\n";

                domainset_t domain_as_set{};
                DOMAINSET_SETOF(dnum,&domain_as_set);

                domain_info.emplace_back
                            (cpu_and_domain_sets{filtered_dcpu_set,domain_as_set});
            }
        }
    }

    auto prog_cpunum_to_os_cpuid(auto& domain_info) -> void
    {
        // Number cpus going across domains first:
        unsigned int domain_num{0U};
        for ( unsigned int cpu_num{0U}
            ; cpu_num<concurrency_count_for_in_domains
            ; ++cpu_num
            )
        {
            unsigned int cpu_ffs= CPU_FFS(&domain_info[domain_num].cpu_set);
            while (0U == cpu_ffs)   // Avoid assuming all domains have
            {                       // the same count of cpus.
                domain_num= (domain_num+1U)%domain_count;
                cpu_ffs=    CPU_FFS(&domain_info[domain_num].cpu_set);
            }

            unsigned int const cpu_id= cpu_ffs-1U;
            CPU_CLR(cpu_id,&domain_info[domain_num].cpu_set);

            cpuset_t cpu_as_set{};
            CPU_SETOF(cpu_id,&cpu_as_set);

            singleton_sets.emplace_back
                            ( cpu_and_domain_sets
                                {cpu_as_set,domain_info[domain_num].domain_set}
                            );

            std::cout << "Program's cpu_num: "
                      << std::to_string(cpu_num)
                      << " has operating system cpu_id: "
                      << std::to_string(cpu_id)
                      << "\n";
        }
    }
// NOLINTEND(readability-function-cognitive-complexity)

public:
    sys_cpubinding()
    {
        if (0 != cpuset(&cpusetid))
            { throw std::runtime_error("cpuset failed."); }

        domainset_t dom_set;
        int memdom_policy{};

        std::vector<cpu_and_domain_sets> domain_info{};

        cpuset_t cpu_set;

        if  (0 != cpuset_getaffinity( CPU_LEVEL_WHICH
                                    , CPU_WHICH_PID
                                    , id_t{-1} // current process
                                    , sizeof(cpu_set)
                                    , &cpu_set
                                    )
            )
            { throw std::runtime_error("failed to identify an affinity"); }

        std::cout << "os PID (not NUMA domain) affinity #cpus: "
                  << std::to_string(CPU_COUNT(&cpu_set))
                  << "\n";

        if  (0 != cpuset_getdomain  ( CPU_LEVEL_WHICH
                                    , CPU_WHICH_PID
                                    , id_t{-1} // current process
                                    , sizeof(dom_set)
                                    , &dom_set
                                    , &memdom_policy
                                    )
            )
            { throw std::runtime_error("Failed to identify memory domains"); }

        domains_and_domain_cpu_sets(dom_set,cpu_set,domain_info);

        domain_count= domain_info.size();

        if (0U == domain_count)
        {
            concurrency_count_for_in_domains= CPU_COUNT(&cpu_set);

            domainset_t domain_as_set{};
            DOMAINSET_ZERO(&domain_as_set);

            domain_info.emplace_back
                        (cpu_and_domain_sets{cpu_set,domain_as_set});
        }


        if (concurrency_count_for_in_domains < 1U)
            { concurrency_count_for_in_domains
                                        = std::thread::hardware_concurrency();
            }

        if (concurrency_count_for_in_domains < 1U)
            { concurrency_count_for_in_domains= 1U; }

        prog_cpunum_to_os_cpuid(domain_info);
    }
};

static struct sys_cpubinding const cpus_info{}; // NOLINT(cert-err58-cpp)
// NOTE: The unlikely throws need not be caught for constructing a static.

auto ConcurrencyCountForInDomains() -> unsigned int // matching c++ type
{
    return cpus_info.concurrency_count_for_in_domains;
}

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpus_used)
{
    unsigned int const num_cpus= cpus_info.singleton_sets.size();

    if (num_cpus < num_cpus_used)
        { throw   std::runtime_error(std::string("#cpus vs. #cpus_used: ")
                + std::to_string(num_cpus) + " == #cpus"
                + " < "
                + "#cpu_used == " + std::to_string(num_cpus_used));
        }

    if (num_cpus_used <= cpu_num)
        { throw   std::runtime_error("#cpus_used vs. cpu#: "
                + std::to_string(num_cpus_used) + " == #cpus_used"
                + " <= "
                + "cpu# == " + std::to_string(cpu_num));
        }

    unsigned int const c_at{cpu_num*(num_cpus/num_cpus_used)};
        // 'c_at' skips cpus when a fraction are in use.
        // This can tend to avoid hyperthreads or other
        // such with extra shared-resource constraints.
        //
        // This does make the code somewhat dependent
        // on the uniformity of the NUMA memory domains
        // as far as local-cpus go.

    if  (0 != cpuset_setaffinity( CPU_LEVEL_WHICH
                                , CPU_WHICH_TID
                                , id_t{-1} // current thread
                                , sizeof(cpus_info.singleton_sets.at(c_at).cpu_set)
                                , &cpus_info.singleton_sets.at(c_at).cpu_set
                                )
        )
        { throw std::runtime_error("failed to set cpu"); }

    // Has the thread migrated by here?

    if (cpus_info.domain_count < 2U) { return; } // Nothing more to do

    if  (0 != cpuset_setdomain  ( CPU_LEVEL_WHICH
                                , CPU_WHICH_TID
                                , id_t{-1} // current thread
                                , sizeof(cpus_info.singleton_sets.at(c_at).domain_set)
                                , &cpus_info.singleton_sets.at(c_at).domain_set
                                , DOMAINSET_POLICY_INTERLEAVE
                                )
        )
        { throw std::runtime_error("Failed to set numa memory domain"); }
}

#elif defined(__NetBSD__) && defined(WANT_CPU_LOCKDOWN)
#include <pthread.h>
#include <sched.h>

#include <vector>
#include <iostream>

struct sys_cpubinding
{
    cpuset_t* cpu_set{cpuset_create()};

    std::vector<cpuset_t*> singleton_sets{}; // Indexed by cpu_num.

    unsigned int concurrency_count{0U}; // Matching c++ type

    sys_cpubinding()
    {
        if (!cpu_set)
            { throw std::runtime_error("cpuset_create failed."); }

        if  (0 != pthread_getaffinity_np( pthread_self()
                                        , cpuset_size(cpu_set)
                                        , cpu_set
                                        )
            )
            { throw std::runtime_error("failed to identify an affinity"); }

        cpuid_t upper_bound_cpus{8U*cpuset_size(cpu_set)};
        cpuid_t cpu_id{0};
        for (; cpu_id < upper_bound_cpus; ++cpu_id) {
            int const cpu_status{cpuset_isset(cpu_id, cpu_set)};

            if      (-1 == cpu_status) break;
            else if ( 0 == cpu_status) continue;

            ++concurrency_count;
        }

        upper_bound_cpus= cpu_id;

        if (concurrency_count < 1U) {
            concurrency_count = std::thread::hardware_concurrency();
            cpu_id = upper_bound_cpus = concurrency_count;
            while (0<cpu_id)
                cpuset_set(--cpu_id, cpu_set);
        }

        cpu_id= 0U;
        for (; cpu_id<upper_bound_cpus; ++cpu_id)
        {
            int const cpu_status{cpuset_isset(cpu_id, cpu_set)};

            if      (-1 == cpu_status) break;
            else if ( 0 == cpu_status) continue;

            cpuset_t* cpu_as_set{cpuset_create()};
            if (!cpu_as_set)
                { throw std::runtime_error("cpuset_create failed."); }

            cpuset_set(cpu_id, cpu_as_set);

            singleton_sets.emplace_back(cpu_as_set);

            std::cout << "Program's cpu_num: "
                      << std::to_string(singleton_sets.size()-1U)
                      << " has operating system cpu_id: "
                      << std::to_string(cpu_id)
                      << "\n";
        }

        if (0U == singleton_sets.size())
            { throw std::runtime_error("failed to identify any cpus"); }
    }

    ~sys_cpubinding()
    {
        for(auto& singleton : singleton_sets) {
            cpuset_destroy(singleton);
            singleton= nullptr;
        }

        cpuset_destroy(cpu_set);
        cpu_set= nullptr;
    }
};

static struct sys_cpubinding const cpus_info{};

auto ConcurrencyCountForInDomains() -> unsigned int // matching c++ type
{
    return cpus_info.concurrency_count;
}

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpus_used)
{
    unsigned int num_cpus= cpus_info.singleton_sets.size();

    if (num_cpus < num_cpus_used)
        { throw   std::runtime_error(std::string("#cpus vs. #cpus_used: ")
                + std::to_string(num_cpus) + " == #cpus"
                + " < "
                + "#cpu_used == " + std::to_string(num_cpus_used));
        }

    if (num_cpus_used <= cpu_num)
        { throw   std::runtime_error("#cpus_used vs. cpu#: "
                + std::to_string(num_cpus_used) + " == #cpus_used"
                + " <= "
                + "cpu# == " + std::to_string(cpu_num));
        }

    unsigned int c_at{cpu_num*(num_cpus/num_cpus_used)};
        // 'c_at' skips cpus when a fraction are in use.
        // This can tend to avoid hyperthreads or other
        // such with extra shared-resource constraints.

    if  (0 != pthread_setaffinity_np( pthread_self()
                                    , cpuset_size(cpus_info.singleton_sets.at(c_at))
                                    , cpus_info.singleton_sets.at(c_at)
                                    )
        )
        { throw std::runtime_error("failed to set cpu"); }

    // Has the thread migrated by here?
}

#elif defined(LGPLv2_1_OKAY) && defined(__linux__) && defined(WANT_CPU_LOCKDOWN)
// <numa.h>, <sched.h>, and <pthread.h> are each licensed via
// LesserGPLv2.1 and so use would get into detailed rules for if a
// program using them is a derivative work and, if it is, if it
// comes under the unrestricted-use criteria vs. needing to meet
// other criteria, such as various prominent notices.
//
// So I avoid distributing such source code.

#include "sys_cpubinding_lgplv2_1_okay.cpp"

#else
// so   !defined(WANT_CPU_LOCKDOWN)
//  ||  (   !defined(__FreeBSD__)
//      &&  !defined(. . .)
//      &&  . . .
//      )

auto ConcurrencyCountForInDomains() -> unsigned int // matching c++ type
{
    unsigned int concurrency{std::thread::hardware_concurrency()};
    return (concurrency<1U) ? 1U : concurrency;
}

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpus_used)
{
    auto const num_cpus= ConcurrencyCountForInDomains();

    if (num_cpus < num_cpus_used)
        { throw std::runtime_error("#cpu_used: #cpus<#cpu_used"); }

    if (num_cpus_used <= cpu_num)
        { throw std::runtime_error("cpu_num: #cpu_used<=cpu_num"); }

    // Nothing to do
}

#endif // of alternatives for handling WANT_CPU_LOCKDOWN

extern "C" char const copyright_and_license_for_sys_cpubinding[]
{
    "Context for this Copyright: sys_cpubinding\n"
    "\n"
    "Copyright (c) 2019-2024 Mark Millard\n"
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
