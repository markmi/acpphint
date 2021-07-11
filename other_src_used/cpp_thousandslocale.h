//
//  cpp_thousandslocale.h (c++20 currently)
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
//  Just cpp_thousandslocale.o: 
//      c++ -std=c++20 -pedantic -g -O3 -c cpp_thousandslocale.cpp
//

#ifndef cpp_thousandslocale_guard
#define cpp_thousandslocale_guard

#include <locale>       // locale

// Can be used to cause C++ like use of ' as a separator:
auto CppThousandsLocale() -> std::locale;

#endif // cpp_thousandslocale_guard
