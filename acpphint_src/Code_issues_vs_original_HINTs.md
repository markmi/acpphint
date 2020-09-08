#### acpphint_src/Code_issues_vs_original_HINTs.md
This file documents properties of source code
differences with the original serial and
pthreads HINT benchmarks.

This covers:
1.  General code notes . . .
2.  Memory use tracking . . .
3.  Types allowed for DSIZE and ISIZE . . .
4.  Other issues in the original HINTs that were avoided. . .
5.  Original serial HINT notes . . .
6.  Original pthreads HINT notes . . .

##### General code notes . . .

The code is C++17 code at the time this is being written.
I generally will not make detailed comments on this
aspect. The HINT kernel code has the smallest of the
textual changes.

The other code is broken up into more source files that
are separately compiled and then linked with the main
program. (The extra programs for investigating/testing
the surveys and sampling use the split out files for
linking as well.)

For this variation on HINT, RUNTM is computed instead
of being directly adjustable. PATIENCE, MXPROC, DSREFS,
ISREFS, and MSIZE are not used.

For this variation on HINT, STILL_UNKNOWN was
added to EFlag as the first value.

The original serial and pthread HINTs did not use
their ANSWER to cross check on correct execution.
This variation on HINT does simple checks.

This variation on HINT does not centrally locate all
its adjustables in one file. The areas with adjustable
items are labeled with the text:
```
// Edit as needed
```
Something like:
```
grep -A5 "Edit as needed" . . .
```
can be used to get an idea where they all are for a set
of the files.

##### Memory use tracking . . .

The original serial HINT did the following (some lines split for readability):
```
    rect = (RECT  *)malloc((MSIZE)(memry * sizeof(RECT)));
    errs = (DSIZE *)malloc((MSIZE)(memry
                                   * sizeof(DSIZE) * 2));
    ixes = (ISIZE *)malloc((MSIZE)(memry
                                   * sizeof(ISIZE) * 2));
```
but it later miscalculated the memory use for those (missing
any '*2' or tracking of the largest index actually used
in errs and in ixes):
```
    memuse = (int)(qdata[i-1].n
           * (sizeof(RECT)+sizeof(DSIZE)+sizeof(ISIZE)));
```
This understates the allocations and can understate the
high end of what is accessed.

pthreads HINT had similar problems for sizeof(DSIZE)
and sizeof(ISIZE) in its calculations.

This variation on HINT uses the total size for its
equivalent of all 3 and accounts for the
sizeof(std::vector<. . .>) overheads as well.

The hint kernel variation also explicitly keeps track of
how much its use spanned of its equivalents of rect, errs,
and ixes. (Relevant for development/testing.)

##### Types allowed for DSIZE and ISIZE . . .

The original serial and pthread HINTs were not designed
to handle unsigned integral types for DSIZE and ISIZE.
This variation on HINT is designed to handled such. In
fact, most of the default DSIZE/ISIZE integral types used
are unsigned types. Handling this involved some case
analysis in the kernel code, avoiding temporarily having
negative numbers.

For this variation on HINT, when ISIZE can hold bigger
values than vector indexes are allowed to be (say, ISIZE
unsigned long long that happens to be 64 bits on a ILP32
architecture), the larger type is used but is treated as
having the smaller maximum value in figuring out what
size of cell grid to use.

##### Other issues in the original HINTs that were avoided. . .

While this variation on Hint does not use such, I'll note
that for the original serial and pthreads HINTs depends
on the overflow behavior of signed integral types, which
C leaves undefined. Examples include (for DSIZE being
a signed, integral type):
```
    while (((tm1 - tm2) == (DSIZE)1) && (tm2 > (DSIZE)0))
    {
        dbits++;
        tm  += tm;
        tm2 += tm2;
        tm1  = tm2 + 1;
    }
```
and (for ISIZE being a signed, integral type):
```
    while (itm2 > (ISIZE)0)
    {
        ibits++;
        itm  += itm;
        itm2 += itm2;
    }
```
compilers are allowed to do many non-obvious things with
such code, including generating code to trap for the
overflow or to assume lack of overflow in the code
generation/test produced.

While this variation on Hint does not use such, I'll note
that for the original serial and pthreads HINTs the
survey code does not initialize tscout but the code
does:
```
    if (tscout == 0)
    {
        fprintf(stderr, "Data type for %s is too small\n", argv[0]);
        exit(0);
    }
```
when tscout need not have been initialized.

In the orignal HINT survey code there was:
```
        else
        {
            tscout =  t;
            nscout *= 2;
            if (nscout > scx)
            {
                nscout /= 2;
                break;
            }
        }
```
but without the = from nscout >= scx, initial dx == nscout
results in using a run result for a smaller nscout value.

This variation on HINT includes the '=' in its analogous
code.

While this variation on Hint does not use such, I'll note
that for the original pthreads HINT:
```
    double  gs[MXPROC];
```
vs.
```
    argvec[iproc].gs = &gs[2*iproc];
```
That is a problem vs the description of MXPROC: "Maximum
number of processors to use in shared memory configuration".
With each "processor" having HI and LO entries in gs the
maximum "processor" count is MXPROC/2 in order to avoid
trashing memory and accessing unowned memory. "2*" could be
considered as missing from the gs declaration. Or the hint.h
description could be considered wrong.

While this variation on Hint does not use such, I'll note
that for the original serial and pthread HINTs:
```
		delq = (double)dmax / gamut - 1;
        quips = delq / t + 1.0 / gamut / t;
```
has the problem that double could lose bits for DSIZE
being, for example, long double or long long.

While this variation on Hint does not use such, I'll note
that for the original serial and pthreads HINTs uses
printf formats that do not track the types of the
matching expressions and also does not cast the expressions
to match the formats used. Modern compilers tend to
report such things.

While this variation on Hint does not use such, I'll note
that for the original serial and pthreads HINTs uses
`double When()`  [K&R style] instead of
`double When(void)`
to declare/define When.

While this variation on Hint does not use such, I'll note
that for the original serial and pthreads HINTs had:
```
typedef struct {
    double  t,         /* Time for a given run*/
            qp,        /* QUIPS for a given run */
			delq;      /* Change in Quality*/
    ISIZE   n;         /* Subintervals for a given run*/
	int     laps;
    }Speed;
```
but laps in Speed was never accessed or assigned.
(The comment for delq does not indicate that it is a
low-side estimate, low by 1/gamut [viewed as a rational
number]. gamut being the difference in the high vs. low bounds.)

##### Original serial HINT notes . . .

The kernel code initialized with: `rect[0].ahi = dmax`
(for when it==0).

This leads to an underestimation of scx\*scy by scy instead of
using dmax+1 == scx\*scy that would have been mathematically
accurate. This avoided overflow/truncation/rounding issues.
dmax based still leads to a proper upper bound for the specific
integration involved in HINT.

When the it==0 case was split, the split ignored
`rect[0].ahi` and assigned accurate figures. Things continued from there
(0<it).

The serial use of the kernel routine made no other use of
dmax.


While this variation on Hint does not use such, I'll note
that for the original serial HINT [gamut below is the difference between the high and low bounds]:
```
		delq = (double)dmax / gamut - 1;
        quips = delq / t + 1.0 / gamut / t;
. . .
        qdata[i].delq = delq;
. . .
    fprintf(curv,"%12.10lf %lf %lf %lf %10d %lf\n", 
            qdata[i-1].t, qdata[i-1].qp, qdata[i-1].delq,
            (double)qdata[i-1].n, memuse, bandwt);
```
writes out a underestimate of the quality, low by
1/gamut (viewed as a rational number).

While this variation on Hint does not use such, I'll note
that for the original serial HINT declares main as
returning void instead of the required int. (Presumes
wanting to avoiding implementation-defined status for
HINT's code.)

##### Original pthreads HINT notes . . .

The original pthreads HINT source only has the initial dx
(scx/NCHUNK/nproc) known to be an integral (non-negative)
power of 2 when (for k known):
```
     scx == power(2,k)*(NCHUNK*nproc)
```
with NCHUNK and nproc each also being integral (non-negative)
powers of 2 so scx was also a such a power of 2. This effectively
restricted nproc (given default NCHUNK values) in order for dx to
be an integral (non-negative) power of 2.

There were no stated requirements for the NCHUNK adjustable and
and nproc adjustable to each be powers of 2\.

This variation on HINT makes dx such a power of 2 but does not
require scx to be such a power of 2, effectively removing the
constraints on NCHUNK and nproc needing to be such powers of
1\. (There are now such things as 6 core/12 thread computers,
after all.)

The pthread code did not use dmax in its kernel code. The
initialization was very different: It was based on setting up
the scatter decomposition.

At no stage was there an explicit single cell spanning the whole
scx*scy area so no individual area calculation spanned a big
enough area to have overflow/truncation/rounding issues. Nor did
any sum of areas that was peformed.

The original pthread HINT sources used type double for gs[?+LO],
gs[?+HI] pairs. This involved potential conversions from DSIZE to
double and from double to DSIZE (for gamut) inside the timed-code.
Also double could potentially lose bits compared to DSIZE (say
long double or long long).

While this variation on Hint does not use such, I'll note
that for the original serial HINT declares main without
indicating the return type.

