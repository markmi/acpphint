//
//  acpphint_kernels.cpp
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

#include "acpphint_kernels.h"
#include <algorithm>    // for std::max
#include <climits>      // for ULONG_MAX, UINT_MAX, ULLONG_MAX
#include <cmath>        // for trunc
#include <stdexcept>    // for std::runtime_error
#include <type_traits>  // for std::is_floating_point
#include <utility>      // for std::cmp_less_equal
#include <limits>       // for std::numeric_limits
#include <cstddef>      // for std::size_t

// NOTE: Various identifiers track the use in the original HINT benchmark code.
// NOLINTBEGIN(readability-identifier-length,readability-function-cognitive-complexity)
template<typename DSIZE, typename ISIZE>
PrimaryKernelInputs<DSIZE,ISIZE>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads)
    : nproc{threads}
{
    // Unlike the old pthread HINT code, work to have the scx value be a
    // power of 2 times the scatter unit.

    auto nchunk= NCHUNK<ISIZE>(nproc);

    ISIZE ISIZE_nproc= nproc;

    ISIZE scatter_unit= ISIZE_nproc*nchunk;
    DSIZE DSIZE_scatter_unit= scatter_unit;

    if constexpr (std::is_floating_point<DSIZE>::value)
    {
        // Find the largest associative DSIZE whole number, dmax.
        DSIZE tm{1U};

        DSIZE volatile tm2= tm + tm; // try to prevent floating register use.
        DSIZE volatile tm1= tm2 + 1; // try to prevent floating register use.
        while (DSIZE{1U}==tm1-tm2)
        {
            tm=     tm2;
            tm2=    tm2+tm2;
            tm1=    tm2+1;
        }

        // Use a grid of dmax + 1 squares, but this might overflow,
        // so back off 1.
        // Indexing may later limit this to a smaller value.
        dmax = tm + (tm - 1); // Maximum such associative whole number

        DSIZE low{1U};
        DSIZE high{dmax};
        while (1<high-low)
        {
            DSIZE mid=low + std::trunc((high-low)/2);
            DSIZE quot= dmax/mid;
            if      (mid<quot)  { low= mid; }
            else if (quot<mid)  { high= mid; }
            else                { low= high= mid; }
        }

        initial_dx= std::trunc(high/DSIZE_scatter_unit);
        DSIZE p2{1U};
        while (2*p2<=initial_dx) { p2*=2; }
        initial_dx= p2;
        // initial_dx is now an integral power of 2.
        scx= initial_dx * DSIZE_scatter_unit;
        scy= high; // Can make rectangular grid cells.

        dmax= (scy-1)*scx + (scx-1);
    }
    else // if constexpr (std::is_integral<DSIZE>::value)
    {
        dmax= std::numeric_limits<DSIZE>::max();

        DSIZE low{1U};
        DSIZE high{dmax};
        while (1<high-low)
        {
            DSIZE mid=low + (high-low)/2;
            DSIZE quot= dmax/mid;
            if      (mid<quot)  { low= mid; }
            else if (quot<mid)  { high= mid; }
            else                { low= high= mid; }
        }

        initial_dx= high/DSIZE_scatter_unit;
        DSIZE next_dx= initial_dx & (initial_dx-1); // remove least signficant 1 bit // NOLINT(hicpp-signed-bitwise)
        while (next_dx!=0)
        {
            initial_dx= next_dx;
            next_dx= initial_dx & (initial_dx-1);   // remove least signficant 1 bit // NOLINT(hicpp-signed-bitwise)
        }
        // initial_dx is now an integral power of 2.
        scx=  initial_dx*DSIZE_scatter_unit;
        scy=  high; // Can make rectangular grid cells.

        dmax= (scy-1)*scx + (scx-1);
    }

    ISIZE imax;

    if constexpr ( std::cmp_less_equal( std::numeric_limits<ISIZE>::max()
                                      , std::numeric_limits<std::size_t>::max()
                                      )
                 )
        { imax= std::numeric_limits<ISIZE>::max(); }
    else
        { imax= std::numeric_limits<std::size_t>::max(); }

    if (imax<=std::numeric_limits<DSIZE>::max())
    {
        DSIZE DSIZE_imax= imax;

        if (DSIZE_imax<=dmax)
        {

            ISIZE low{1U};
            ISIZE high{imax};
            while (1<high-low)
            {
                ISIZE mid=low + (high-low)/2;
                ISIZE quot= imax/mid;
                if      (mid<quot)  { low= mid; }
                else if (quot<mid)  { high= mid; }
                else                { low= high= mid; }
            }

            ISIZE ISIZE_initial_dx= high/scatter_unit;
            ISIZE next_dx= ISIZE_initial_dx & (ISIZE_initial_dx-1); // remove least signficant 1 bit // NOLINT(hicpp-signed-bitwise)
            while (next_dx!=0)
            {
                ISIZE_initial_dx= next_dx;
                next_dx= ISIZE_initial_dx & (ISIZE_initial_dx-1); // remove least signficant 1 bit // NOLINT(hicpp-signed-bitwise)
            }
            initial_dx= ISIZE_initial_dx;
            // initial_dx is now an integral power of 2.
            scx= initial_dx * DSIZE_scatter_unit;
            scy= high; // Can make rectangular grid cells.

            dmax= (scy-1)*scx + (scx-1);
        }
    }
}

template<typename DSIZE, typename ISIZE>
auto Kernel ( HwConcurrencyCount                const   iproc
            , PrimaryKernelInputs<DSIZE,ISIZE>  const&  ki
            , KernelVectors<DSIZE,ISIZE>&               kv
            ) -> KernelResults<DSIZE,ISIZE>
{
    DSIZE constexpr DSIZE_0{0U};
    DSIZE constexpr DSIZE_1{1U};
    DSIZE constexpr DSIZE_2{2U};

    DSIZE DSIZE_iproc= iproc;
    DSIZE DSIZE_nproc= ki.nproc;

    DSIZE sh{0}; // Sum of areas, high bound
    DSIZE sl{0}; // Sum of areas, low bound

    ISIZE const NCHUNK_ISIZE{NCHUNK<ISIZE>(ki.nproc)};
        // for 1==ki.nproc: 1==NCHUNK (not adjustable!)

    DSIZE const NCHUNK_DSIZE= NCHUNK_ISIZE;

    ISIZE const mcnt_bnd= kv.rect.size() - NCHUNK_ISIZE;
                                                // The original did the
                                                // analogous subtraction in the
                                                // loop.

    // Initialize the first NCHUNK intervals for the scatter decomposition
    // (or the 1==NCHUNK for serial).

    DSIZE DSIZE_nchnk{0U};
    for (ISIZE nchnk{0U}; nchnk < NCHUNK_ISIZE; ++nchnk)
    {
        auto& at_nchnk{kv.rect[nchnk]}; // .at(nchnk)

        at_nchnk.dx=    ki.initial_dx;
        at_nchnk.xl=    ((ki.scx / NCHUNK_DSIZE) * DSIZE_iproc) / DSIZE_nproc
                            + ( DSIZE_nchnk
                              * DSIZE_nproc
                              * at_nchnk.dx
                              );
        at_nchnk.xr= at_nchnk.xl + at_nchnk.dx;

        // Evaluate the function (left side).
        {
            DSIZE const tm=  ki.scx + at_nchnk.xl;
            DSIZE const tm2= ki.scy *   (ki.scx -   ((at_nchnk.xl == 0)
                                                        ? DSIZE_1
                                                        : at_nchnk.xl
                                                    )
                                        );

            at_nchnk.fll= tm2 / tm;
            at_nchnk.flh= at_nchnk.fll
                                  + ((tm * at_nchnk.fll != tm2)
                                        ? DSIZE_1
                                        : DSIZE_0
                                    );
        }

        // Evaluate the function (right side).
        {
            DSIZE const tm=  ki.scx + at_nchnk.xr;
            DSIZE const tm2= ki.scy * (ki.scx - at_nchnk.xr);
            at_nchnk.frl= tm2 / tm;

            at_nchnk.frh= at_nchnk.frl
                                  + ((tm * at_nchnk.frl != tm2)
                                        ? DSIZE_1
                                        : DSIZE_0
                                    );
        }

        // For 1U==nproc: avoid overflow: underestimate ki.scx*ki.scy by 1
        // For  1U<nproc: no overflow is possible so use normal code.
        at_nchnk.ahi= (1U==ki.nproc)
                                ? ki.dmax // like old serial HINT.
                                : at_nchnk.flh * at_nchnk.dx;

        at_nchnk.alo= at_nchnk.frl * at_nchnk.dx;

        if (at_nchnk.ahi < at_nchnk.alo)
            { throw std::runtime_error("at_nchnk's ahi<alo while processing"); }

        {
            DSIZE const tm= at_nchnk.ahi - at_nchnk.alo;

            // Need to find the correct spot for this error info to go into.
            // Original had OUT OF BOUNDS ACCESS for nchnk == 0 case!!!
            // (Or any other i==0 case of evaluating the loop test.)
            ISIZE i{nchnk};        // .at(i-1)
            for (; (i > 0) && (kv.errs[i-1] < tm); --i) // Swapped && operands!
            {
                kv.errs[i]= kv.errs[i-1];   // .at(i-1)
                kv.ixes[i]= kv.ixes[i-1];   // .at(i-1)
            }

            kv.errs[i]= tm;     // .at(i)
            kv.ixes[i]= nchnk;  // .at(i)
        }

        sh+= at_nchnk.ahi; // for 1U==nproc ahi is slight underestimate.
        sl+= at_nchnk.alo;

        if (sh < sl)
            { throw std::runtime_error("nchnk's sh<sl while processing"); }

        ++DSIZE_nchnk;
    }

    ISIZE iq= NCHUNK_ISIZE-1; // last entry in queue.

    KernelResults<DSIZE,ISIZE> kr{iq};

    ISIZE it{0U};
    for (; (it < mcnt_bnd) && (it <= iq); ++it)
    {
        ISIZE const ma{kv.ixes[it]};        // Index of parent; // .at(it)
                                            // Head of list has maximum error.
        ISIZE const io{ma};                 // Index of left child;
                                            // Head of list has maximum error.
        ISIZE const jo= it + NCHUNK_ISIZE;  // Index of right child.

        kr.RECT_maxjo= std::max(kr.RECT_maxjo,jo); // acpphint memory tracking

        auto& at_ma{kv.rect[ma]};   // .at(ma)
        auto& at_io{kv.rect[io]};   // .at(io)
        auto& at_jo{kv.rect[jo]};   // .at(jo)

        // .dx is required to be a integral (non-negative) power of 2.
        // So initial_dx needs to be set up that way, even when
        // nproc*NCHUNK (the scatter unit) is not such a power of 2.
        // Note: initial_dx==scx/(NCHUNK*nproc) .
        at_jo.dx= at_io.dx=  at_ma.dx / DSIZE_2;

        if (std::trunc(at_io.dx) != at_io.dx)
            { throw std::runtime_error( "std::trunc(at_io.dx) != at_io.dx"); }

        if (std::trunc(at_jo.dx) != at_jo.dx)
            { throw std::runtime_error( "std::trunc(at_jo.dx) != at_jo.dx"); }

        // Right child gets right boundary.
        at_jo.xr= at_ma.xr;

        // New point in about the middle is shared by child subintervals
        at_io.xr= at_io.xl + at_io.dx;
        at_jo.xl= at_io.xr;

        // Right child gets right f value upper and lower bounds
        at_jo.frl= at_ma.frl;
        at_jo.frh= at_ma.frh;

        // Note that the left child simply inherits much of its info from ma.

        // This is the function evaluation.
        {
            DSIZE const tm=  ki.scx + at_io.xr;
            DSIZE const tm2= ki.scy * (ki.scx - at_io.xr);

            at_io.frl= tm2 / tm;

            at_io.frh= at_io.frl
                              + ((tm * at_io.frl != tm2)
                                    ? DSIZE_1
                                    : DSIZE_0
                                );
            at_jo.fll= at_io.frl;
            at_jo.flh= at_io.frh;
        }

        {
            // Compute the left daughter error. Allow for DSIZE unsigned.
            DSIZE tmio{0U};
            if (at_io.frh <= at_io.fll && DSIZE_2 <= at_io.dx)
                { tmio= (at_io.fll - at_io.frh) * (at_io.dx - DSIZE_2); }
            else if (at_io.fll <= at_io.frh && at_io.dx <= DSIZE_2)
                { tmio= (at_io.frh - at_io.fll) * (DSIZE_2 - at_io.dx); }
            else
                {} // tmio==0U

            DSIZE const errio=      ( at_io.flh - at_io.frh
                                    + at_io.fll - at_io.frl
                                    )
                                  * (at_io.dx - DSIZE_1)
                                  - tmio;

            // Repeat for the right daughter. Allow for DSIZE unsigned.
            DSIZE tmjo{0U};
            if (at_jo.frh <= at_jo.fll && DSIZE_2 <= at_jo.dx)
                { tmjo= (at_jo.fll - at_jo.frh) * (at_jo.dx - DSIZE_2); }
            else if (at_jo.fll <= at_jo.frh && at_jo.dx <= DSIZE_2)
                { tmjo= (at_io.frh - at_io.fll) * (DSIZE_2 - at_jo.dx); }
            else
                {} // tmjo==0U

            DSIZE const errjo=      ( at_jo.flh - at_jo.frh
                                    + at_jo.fll - at_jo.frl
                                    )
                                  * (at_jo.dx - DSIZE_1)
                                  - tmjo;

            // Compute indices for io and jo on the queue.
            ISIZE const inc= 1 + ((errio < errjo) ?1 :0);   // 1 vs. 2
            ISIZE const jnc= 3 - inc;                       // 2 vs. 1

            // Put on both io and jo. If one is zero, it will not be used.
            kv.errs[iq+inc]= errio; // .at(iq+inc)
            kv.ixes[iq+inc]= io;    // .at(iq+inc)
            kv.errs[iq+jnc]= errjo; // .at(iq+jnc)
            kv.ixes[iq+jnc]= jo;    // .at(iq+jnc)
        }

        kr.ixes_errs_maxiq= std::max    ( kr.ixes_errs_maxiq
                                        , static_cast<ISIZE>(iq+2)
                                        ); // acpphint memory tracking

        // Decide how much to increment iq.
        iq= iq + 1 + ((kv.errs[iq+2] != DSIZE_0) ?1 :0); //.at(iq+2)
                                    // last entry in queue.

        // Remove parent sum contributions. Replace with child contributions.
        {
            DSIZE const tm{at_ma.alo}; // Remember: io==ma
            at_io.alo= at_io.frl * at_io.dx;
            at_jo.alo= at_jo.frl * at_jo.dx;
            sl-= tm;
            sl+= at_io.alo + at_jo.alo;
        }
        {
            DSIZE const tm{at_ma.ahi}; // Remember: io==ma
            at_io.ahi= at_io.flh * at_io.dx;
            at_jo.ahi= at_jo.flh * at_jo.dx;
            sh-= tm;
            sh+= at_io.ahi + at_jo.ahi;
        }

        if (sh < sl)
            { throw std::runtime_error("highbound<lowbound while processing"); }
    }

    kr.result_bounds.HI= sh;
    kr.result_bounds.LO= sl;

    kr.eflag= (it<=iq)  ? KernelResults<DSIZE,ISIZE>::EFlag::normal
                        : KernelResults<DSIZE,ISIZE>::EFlag::DISCRET;

    kr.RECT_total_used=     kr.RECT_maxjo;      ++kr.RECT_total_used;
    kr.ixes_errs_used_each= kr.ixes_errs_maxiq; ++kr.ixes_errs_used_each;

    return kr;
}
// NOLINTEND(readability-identifier-length,readability-function-cognitive-complexity)

// Edit as needed to add more alternatives (or disable some):

// DSIZE=short:

template
PrimaryKernelInputs<short,short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);
template
auto Kernel<short,short>
            ( HwConcurrencyCount                                    const   iproc
            , PrimaryKernelInputs<short,short>                      const&  ki
            , KernelVectors<short,short>&                                   kv
            ) -> KernelResults<short,short>;

template
PrimaryKernelInputs<short,unsigned short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);
template
auto Kernel<short,unsigned short>
            ( HwConcurrencyCount                                    const   iproc
            , PrimaryKernelInputs<short,unsigned short>             const&  ki
            , KernelVectors<short,unsigned short>&                          kv
            ) -> KernelResults<short,unsigned short>;

// DSIZE=unsigned short:

template
PrimaryKernelInputs<unsigned short,short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);
template
auto Kernel<unsigned short,short>
            ( HwConcurrencyCount                                    const   iproc
            , PrimaryKernelInputs<unsigned short,short>             const&  ki
            , KernelVectors<unsigned short,short>&                          kv
            ) -> KernelResults<unsigned short,short>;

template
PrimaryKernelInputs<unsigned short,unsigned short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);
template
auto Kernel<unsigned short,unsigned short>
            ( HwConcurrencyCount                                    const   iproc
            , PrimaryKernelInputs<unsigned short,unsigned short>    const&  ki
            , KernelVectors<unsigned short,unsigned short>&                 kv
            ) -> KernelResults<unsigned short,unsigned short>;

#if ULONG_MAX == UINT_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned int:

template
PrimaryKernelInputs<unsigned int,unsigned int>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<unsigned int,unsigned int>
            ( HwConcurrencyCount                                const   iproc
            , PrimaryKernelInputs<unsigned int,unsigned int>    const&  ki
            , KernelVectors<unsigned int,unsigned int>&                 kv
            ) -> KernelResults<unsigned int,unsigned int>;
#endif

// DSIZE=unsigned long: // Always included

template
PrimaryKernelInputs<unsigned long,unsigned long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<unsigned long,unsigned long>
            ( HwConcurrencyCount                                const   iproc
            , PrimaryKernelInputs<unsigned long,unsigned long>  const&  ki
            , KernelVectors<unsigned long,unsigned long>&               kv
            ) -> KernelResults<unsigned long,unsigned long>;

#if ULONG_MAX == ULLONG_MAX || defined(DSIZE_ALL_ISIZE_ALL)
// DSIZE=unsigned long long:

template
PrimaryKernelInputs<unsigned long long,unsigned long long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<unsigned long long,unsigned long long>
            ( HwConcurrencyCount                                    const iproc
            , PrimaryKernelInputs< unsigned long long, unsigned long long>
                                                                    const& ki
            , KernelVectors<unsigned long long,unsigned long long>&        kv
            ) -> KernelResults<unsigned long long,unsigned long long>;
#endif

// DSIZE=float:

template
PrimaryKernelInputs<float,short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<float,short>
            ( HwConcurrencyCount                        const   iproc
            , PrimaryKernelInputs<float,short>          const&  ki
            , KernelVectors<float,short>&                       kv
            ) -> KernelResults<float,short>;

template
PrimaryKernelInputs<float,unsigned short>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<float,unsigned short>
            ( HwConcurrencyCount                        const   iproc
            , PrimaryKernelInputs<float,unsigned short> const&  ki
            , KernelVectors<float,unsigned short>&              kv
            ) -> KernelResults<float,unsigned short>;

template
PrimaryKernelInputs<float,unsigned int>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<float,unsigned int>
            ( HwConcurrencyCount                        const   iproc
            , PrimaryKernelInputs<float,unsigned int>   const&  ki
            , KernelVectors<float,unsigned int>&                kv
            ) -> KernelResults<float,unsigned int>;

#ifdef DSIZE_ALL_ISIZE_ALL
// DSIZE=double:

template
PrimaryKernelInputs<double,unsigned int>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<double,unsigned int>
            ( HwConcurrencyCount                        const   iproc
            , PrimaryKernelInputs<double,unsigned int>  const&  ki
            , KernelVectors<double,unsigned int>&               kv
            ) -> KernelResults<double,unsigned int>;

template
PrimaryKernelInputs<double,unsigned long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<double,unsigned long>
            ( HwConcurrencyCount                        const   iproc
            , PrimaryKernelInputs<double,unsigned long> const&  ki
            , KernelVectors<double,unsigned long>&              kv
            ) -> KernelResults<double,unsigned long>;

template
PrimaryKernelInputs<double,unsigned long long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<double,unsigned long long>
            ( HwConcurrencyCount                                const   iproc
            , PrimaryKernelInputs<double,unsigned long long>    const&  ki
            , KernelVectors<double,unsigned long long>&                 kv
            ) -> KernelResults<double,unsigned long long>;

// DSIZE=long double:

template
PrimaryKernelInputs<long double,unsigned long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<long double,unsigned long>
            ( HwConcurrencyCount                                const   iproc
            , PrimaryKernelInputs<long double,unsigned long>    const&  ki
            , KernelVectors<long double,unsigned long>&                 kv
            ) -> KernelResults<long double,unsigned long>;

template
PrimaryKernelInputs<long double,unsigned long long>::PrimaryKernelInputs
                                            (HwConcurrencyCount const threads);

template
auto Kernel<long double,unsigned long long>
            ( HwConcurrencyCount                                    const iproc
            , PrimaryKernelInputs<long double,unsigned long long>   const& ki
            , KernelVectors<long double,unsigned long long>&               kv
            ) -> KernelResults<long double,unsigned long long>;
#endif


extern "C" char const copyright_and_license_for_acpphint_kernels[]
{
    "Context for this Copyright: acpphint_kernels\n"
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
