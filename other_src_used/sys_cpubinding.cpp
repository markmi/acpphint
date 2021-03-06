//
//  sys_cpubinding.cpp (c++20 currently)
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
//  Started by Mark Millard on 2019-09-12.
//

#include "sys_cpubinding.h"

#include <thread>       // thread::hardware_concurrency

#if defined(__FreeBSD__) && defined(WANT_CPU_LOCKDOWN)
#include <sys/param.h>
#include <sys/domainset.h>
#include <strings.h> // for ffsl and such that cpuset.h causes to be involved.
#include <sys/cpuset.h>

#include <vector>
#include <iostream>

struct cpu_and_domain_sets
{
    cpuset_t    cpu_set{};
    domainset_t domain_set{}; // always a singleton set
};

struct sys_cpubinding
{
    cpusetid_t cpusetid{};

    std::vector<cpu_and_domain_sets> singleton_sets{}; // Indexed by cpu_num.

    unsigned int domain_count{0u};
    unsigned int concurrency_count_for_in_domains{0u}; // Matching c++ type
    
    sys_cpubinding()
    {
        if (0 != cpuset(&cpusetid))
            throw std::runtime_error("cpuset failed.");

        domainset_t dom_set;
        int memdom_policy;

        std::vector<cpu_and_domain_sets> domain_info{};
        
        if  (0 != cpuset_getdomain  ( CPU_LEVEL_WHICH
                                    , CPU_WHICH_PID
                                    , id_t{-1} // current process
                                    , sizeof(domainset_t)
                                    , &dom_set
                                    , &memdom_policy
                                    )
            )
            throw std::runtime_error("Failed to identify memory domains");
        
        for (id_t d{0u}; d<DOMAINSET_SETSIZE; ++d)
        {
            if (DOMAINSET_ISSET(d,&dom_set))
            {
                cpuset_t cpu_set;

                if  (0 != cpuset_getaffinity( CPU_LEVEL_WHICH
                                            , CPU_WHICH_DOMAIN
                                            , d
                                            , sizeof(cpuset_t)
                                            , &cpu_set
                                            )
                    )
                    throw std::runtime_error("failed to identify an affinity");

                unsigned int domain_concurrency_count= CPU_COUNT(&cpu_set);
                
                if (0u == domain_concurrency_count) continue;

                // Positive domain_concurrency_count . . .

                concurrency_count_for_in_domains+= domain_concurrency_count;

                std::cout << "os domain " << std::to_string(d) << " affinity #cpus: "
                          << std::to_string(domain_concurrency_count)
                          << "\n";

                domainset_t domain_as_set{};
                DOMAINSET_SETOF(d,&domain_as_set);

                domain_info.emplace_back
                            (cpu_and_domain_sets{cpu_set,domain_as_set});
            }
        }

        domain_count= domain_info.size();

        if (0u == domain_count)
        {
            cpuset_t cpu_set;

            if  (0 != cpuset_getaffinity( CPU_LEVEL_WHICH
                                        , CPU_WHICH_PID
                                        , id_t{-1} // current process
                                        , sizeof(cpuset_t)
                                        , &cpu_set
                                        )
                )
                throw std::runtime_error("failed to identify an affinity");

            concurrency_count_for_in_domains= CPU_COUNT(&cpu_set);

            std::cout << "os PID (not NUMA domain) affinity #cpus: "
                      << std::to_string(concurrency_count_for_in_domains)
                      << "\n";

            domainset_t domain_as_set{};
            DOMAINSET_ZERO(&domain_as_set);

            domain_info.emplace_back
                        (cpu_and_domain_sets{cpu_set,domain_as_set});
        }


        if (concurrency_count_for_in_domains < 1u)
            concurrency_count_for_in_domains
                                        = std::thread::hardware_concurrency();
                                            
        if (concurrency_count_for_in_domains < 1u)
            concurrency_count_for_in_domains= 1u;

        // Number cpus going across domains first:
        unsigned int domain_num{0u};
        for ( unsigned int cpu_num{0u}
            ; cpu_num<concurrency_count_for_in_domains
            ; ++cpu_num
            )
        {
            unsigned int cpu_ffs= CPU_FFS(&domain_info[domain_num].cpu_set);
            while (0u == cpu_ffs)   // Avoid assuming all domains have
            {                       // the same count of cpus.
                domain_num= (domain_num+1u)%domain_count;
                cpu_ffs=    CPU_FFS(&domain_info[domain_num].cpu_set);
            }
            
            unsigned int const cpu_id= cpu_ffs-1u;
            CPU_CLR(cpu_id,&domain_info[domain_num].cpu_set);

            cpuset_t cpu_as_set{};
            CPU_SETOF(cpu_id,&cpu_as_set);
            
            singleton_sets.emplace_back
                            ( cpu_and_domain_sets
                                {cpu_as_set,domain_info[domain_num].domain_set}
                            );
        }
    }
};

static struct sys_cpubinding const cpus_info{};

auto ConcurrencyCountForInDomains() -> unsigned int // matching c++ type
{
    return cpus_info.concurrency_count_for_in_domains;
}

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpus_used)
{
    unsigned int num_cpus= cpus_info.singleton_sets.size();

    if (num_cpus < num_cpus_used)
        throw     std::runtime_error(std::string("#cpus vs. #cpus_used: ")
                + std::to_string(num_cpus) + " == #cpus"
                + " < "
                + "#cpu_used == " + std::to_string(num_cpus_used));

    if (num_cpus_used <= cpu_num)
        throw std::runtime_error("#cpus_used vs. cpu#: "
                + std::to_string(num_cpus_used) + " == #cpus_used"
                + " <= "
                + "cpu# == " + std::to_string(cpu_num));

    unsigned int c{cpu_num*(num_cpus/num_cpus_used)};
        // 'c' skips cpus when a fraction are in use.
        // This can tend to avoid hyperthreads or other
        // such with extra shared-resource constraints.
        //
        // This does make the code somewhat dependent
        // on the uniformity of the NUMA memory domains
        // as far as local-cpus go.

    if  (0 != cpuset_setaffinity( CPU_LEVEL_WHICH
                                , CPU_WHICH_TID
                                , id_t{-1} // current thread
                                , sizeof(cpuset_t)
                                , &cpus_info.singleton_sets.at(c).cpu_set
                                )
        )
        throw std::runtime_error("failed to set cpu");

    // Has the thread migrated by here?

    if (cpus_info.domain_count < 2u) return; // Nothing more to do

    if  (0 != cpuset_setdomain  ( CPU_LEVEL_WHICH
                                , CPU_WHICH_TID
                                , id_t{-1} // current thread
                                , sizeof(domainset_t)
                                , &cpus_info.singleton_sets.at(c).domain_set
                                , DOMAINSET_POLICY_INTERLEAVE
                                )
        )
        throw std::runtime_error("Failed to set numa memory domain");
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
    return (concurrency<1u) ? 1u : concurrency;
}

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpus_used)
{
    auto const num_cpus= ConcurrencyCountForInDomains();

    if (num_cpus < num_cpus_used)
        throw std::runtime_error("#cpu_used: #cpus<#cpu_used");

    if (num_cpus_used <= cpu_num)
        throw std::runtime_error("cpu_num: #cpu_used<=cpu_num");

    // Nothing to do
}

#endif // of alternatives for handling WANT_CPU_LOCKDOWN

char copyright_and_license_for_sys_cpubinding[]
{
    "Context for this Copyright: sys_cpubinding\n"
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
