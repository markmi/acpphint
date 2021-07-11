//
//  cpp_thousandslocale.cpp (c++20 currently)
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

#include "cpp_thousandslocale.h"

//#include <locale>       // locale, numpunct

namespace
{
    template<typename T>
    class cpp_numpunct;

    template<>
    class cpp_numpunct<char> : public std::numpunct<char>
    {
        auto do_thousands_sep() const -> char           override
        {
            return '\'';
        }

        auto do_grouping()      const -> std::string    override
        {
            return {3};
        }
    };
}

auto CppThousandsLocale() -> std::locale
{
    
    static std::locale const cpp_locale{std::locale(), new cpp_numpunct<char>{}};
    return cpp_locale;
}

char copyright_and_license_for_cpp_thousandslocale[]
{
    "Context for this Copyright: cpp_thousandslocale\n"
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
