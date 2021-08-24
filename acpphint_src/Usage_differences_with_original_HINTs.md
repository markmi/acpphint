#### acpphint_src/Usage_differences_with_original_HINTs.md
This file documents differences with the original serial and
pthreads HINT benchmarks that matter for use.

The rest of this document covers:
1.  Context . . .
2.  Data provided . . .
3.  argv[?] use . . .
4.  Time handling . . .
5.  License notes . . .
##### Context . . .
I am not trying to collect/archive reports of results.

There are separate programs for reporting on some example kernel surveys and
kernel sampling, as well as time "now" usage. These other programs are/were used 
as a basis for some testing and, for the most part, are not considered here.

##### Data provided . . .
This variation on HINT does not compute or present 'Net QUIPs' (QUality
Improvement Per Second). It only provides data for making various plots (in a
format appropriate for at least gnuplot):

1.  median trail's mean time per lap in sec 
    (median from sort by mean time per lap)
2.  QUality ImProvement/sec (a.k.a QUIPS)
3.  Quality (a.k.a QUIPS * previously listed time per lap, 
    like in the original pthread HINT)
4.  kernel vectors total memory use
    (not an understated figure like the original
    serial and pthreads HINTs used)

It does not include figures that I did not use from the
original serial and pthread HINTs: n (number of elements in
the rect vector) and bandwidth.

(The historical bandwidth calculations made unverified presumptions about the general code generation results. I'm not sure that it even had a good, uniform interpretation for the figures. Also, the original serial HINT reported delq instead of QUality, so systematically slightly under-reported QUality.)

Some gnuplot commands are thus distinct for this variation
on HINT.

One run of this HINT variant by default outputs files for
several DSIZE/ISIZE/threads-use combinations. The code can
be adjusted to run and output just ones of interest or
additional non-default ones.

For DSIZE/ISIZE combinations that are big enough, one
run of this HINT variant can output files for both
serial HINT and for examples of various multi-threaded
HINT counts.

##### argv[?] use . . .
The output files have the argv[0] text as the file name
prefix and text after that identifying the specific test.
The example makefile.* files generate very long command
names that encode lots of context information.

The default filename suffix is ".txt" .

argv[1], if supplied, supplies an alternate filename suffix.

argv[2], if supplied, specifies a minimum thread count.

argv[3], if supplied, specifies a maximum thread count.

For any of those that are missing, the defaults are:

1.  ".txt" for argv[1]
2.  3 for argv[2]
3.  For argv[3]: use the hardware concurrency count from C++
    (or, for cpulockdown variants, elsewhere); also do a serial
    HINT (single thread) run as well

Other than the potential serial run, the thread counts drop by
a factor of 2 (integer division), starting at the maximum.

##### The source code adjustable-items . . .
The areas with adjustable items are labeled with the text:
```
// Edit as needed
```
They are not centrally located in the source code. Something
like:
```
grep -A5 "Edit as needed" . . .
```
can be used to get an idea where they all are for a set of
the files.

##### Time handling . . .
The original serial HINT and pthreads HINT tried to
eliminate examples of "less work took more time"
by retrying or eventually taking a value from another
example. This variation on HINT does not do that.

While still using the mean over laps for each trial,
this variation on HINT uses the median value of such
over its trials, accepting it even for "less work
took more time". Because of this, using line plots
when the x axis is time can be important to be able
to see the "more work" order for interpreting plots
that show such data.

Another consequence is that the results are less
biased toward extreme performance under optimal
conditions for such. One can explore conditions
and their consequences more readily.

The original serial and pthreads HINTs did not
do much to measure how time measurements work
and to scale target time frames for the mean
over laps based on such. This variation on
HINT does more, including calculating a figure for
what the original HINTs called RUNTM. It also
uses the C++ steady clock by default.

##### Calculating kernel vector memory use . . .
The original serial HINT did the following (lines split
for readability):
```
    rect = (RECT  *)malloc((MSIZE)(memry * sizeof(RECT)));
    errs = (DSIZE *)malloc((MSIZE)(memry
                                    * sizeof(DSIZE) * 2));
    ixes = (ISIZE *)malloc((MSIZE)(memry
                                    * sizeof(ISIZE) * 2));
```
but later miscalculated the memory use for those (missing any
'*2'):
```
    memuse = (int)(qdata[i-1].n
           * (sizeof(RECT)+sizeof(DSIZE)+sizeof(ISIZE)));
```
pthreads HINT had similar problems for sizeof(DSIZE)
and sizeof(ISIZE) in its calculations.

This variation on HINT uses the total sizes for its
equivalent of all 3, including the sizeof(std::vector<. . .>)'s.

##### Types allowed for DSIZE and ISIZE . . .
The original serial and pthread HINTs were not designed
to handle unsigned integral types for DSIZE and ISIZE.
This variation on HINT is designed to handled such. In
fact, most of the default DSIZE/ISIZE integral types used
are unsigned types.

For this variation on HINT, when ISIZE can hold bigger
values than  array indexes are allowed to be (say, ISIZE
unsigned long long on a ILP32 architecture), the larger
type is used but is treated as having the smaller maximum
value in figuring out what size of cell grid to use.

##### License notes . . .
According to the orignal HINT's copyright holder,
Iowa State University Research Foundation, Inc.,
the intended license for HINT was always GPLv2
(no earlier, no later). The actual distributions
that I ever found did not correctly set this up:
no specific GPL version was specified at all and
no copy of the intended license was included.
(Note: the original authors/distributors were not
copyright holders.)

This variation on HINT follows the intended HINT
overall licensing: GPLv2. There is some contributing
code that has a GPLv2 compatible license that ends
up linked in the variation's programs, giving a
GPLv2-only status overall.

[Note: I do not plan on any binary distributions,
just source code on my part. But who knows what
others may do.]
