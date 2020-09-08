//
//  sys_cpubinding.h (c++17 currently)
//
//  Copyright (c) 2019 Mark Millard
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

#ifndef sys_cpubinding_guard
#define sys_cpubinding_guard

#include <stdexcept>    // runtime_error

// No claim that this interface could be used to deal well with
// variable numbers of "cpus" per memory domain. But also there
// is no hook for detecting such conditions either.

auto ConcurrencyCountForInDomains() -> unsigned int; // matching c++ type
// This is a total across NUMA memory domains,
// thinking of non-NUMA as 1 memory domain.

void RestrictThreadToCpu(unsigned int cpu_num, unsigned int num_cpu_used);
// Includes assigning a "interleave policy" to try to
// memory in a way allowing more bandwidth.
//
// cpu_num: 0u <= cpu_num < #cpus is a caller side
// numbering, not necessarily a system numbering.

#endif // sys_cpubinding_guard
