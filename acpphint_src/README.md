#### acpphint_src/README.md
This covers:
1.  What `acpphint` is a variation on . . .
2.  How to build . . .
3.  How to avoid building some `DSIZE`/`ISIZE` combinations . . .
4.  How to run the benchmarks via a makefile.NAME . . .
5.  Licensing notes . . .

##### What `acpphint` is a variation on . . .

`acpphint` is a variation on the original HINT serial and pthread benchmarks, but written in C++ and with various choices specific to my use. It still generates files appropriate for gnuplot use, but not fully matching the original HINT benchmark's output files.

I am not trying to collect/archive reports of results.

Overall, `acpphint` is licensed as GPLv2 (not earlier, not later). Some ISC license source that is not derived from any HINT variant is in use as well, leading to GPLv2 overall. More notes about this are at the bottom of this document and in the license texts that are in:
```
LICENSE.md                          (GPLv2)
../LICENSE.md                       (GPLv2 again)
../other_src_used/LICENSE.md        (OpenBSD-vintage ISC)
../acpphint_example_data/LICENSE.md (no licensing)
```
See also:
```
README.md (this acpphint/README.md [for my context])
../README.md
../other_src_used/README.md
../acpphint_example_data/README.md
```
##### How to build . . .
These notes are about the included examples. You may do otherwise. The examples presume use of (at the time this is being written):
```
macOS: Apple's c++
FreeBSD: devel/gcc9's g++9 
Fedora: g++ (that happened to be a V9)
```
FreeBSD could use its system-clang-8 but I got more performant results from using g++9, even forcing use of FreeBSD libraries, not libstdc++.

The directory structure used is local and self-contained. I normally call the top of the local tree `acpphint` . I include listing it below, at least initially. The directory structure is then:
```
acpphint/acpphint_example_data/
acpphint/acpphint_src/
acpphint/objs/
acpphint/other_src_used/
```
The builds are done from `acpphint/acpphint_src/`, even for the one program that only uses source from `acpphint/other_src_used/` . The makefile.NAME files are in `acpphint/acpphint_src/`. (`makefile.common_part` is not an example of the below.) The intent for an example `acpphint/acpphint_src/makefile.NAME` is to use ( having already cd'd into `acpphint/acpphint_src/` ):
```
make -f makefile.NAME build
```
The source code for such a build is pure C++, not system specific code. (C++17 when this was written.) The executables are placed in files with paths matching the pattern:
```
../*-NAME
```
(i.e., `acpphint/*-NAME` in my context).

Some examples have variants named with a "-cpulockdown" suffix added after the original NAME, so:
```
make -f makefile.NAME-cpulockdown build
```
exist for the FreeBSD examples (only).

Why not Linux? Using <numa.h>, <sched.h>, and/or <pthread.h>  for Linux for `../other_src_used/sys_cpubinding.cpp` would get into yet other licensing instead of OpenBSD-vintage ISC, which I choose to avoid. So I do not provide source or a makefile.NAME-cpulockdown for Linux. I did use a `#include "sys_cpubinding_lgplv2_1_okay.cpp"` in `../other_src_used/sys_cpubinding.cpp` to provide a hook for someone that wants to provide such a file for their own use. But there is no such file in my source distribution.

The source code in `../other_src_used/sys_cpubinding.cpp` is generally system specific, except for the no-op ``#else`` case, unlike the other C++ source code. Thread stacks need not end up on the local memory domain for NUMA for the simple technique used. I generally only use the cpulockdown variants for investigating if such changes end up changing otherwise odd results.

I encode a lot into the example NAME's. So they are very long for the examples. The `acpphint-` prefixed executable name is the benchmark program. There are 3 other informational programs, mostly used for development/testing/investigating but generally built and run. So:
```
../acpphint-NAME (the benchmark program, GPLv2 only)

../cpp_clockinfo_main-NAME
(for investigating basic clock "now" behavior,
OpenBSD-vintage ISC)

../acpphint_kernelsurveyors_main-NAME
(for investigating what the acpphint survey
stage behavior is like, GLPv2 only)

../acpphint_kernelsamplers_main-NAME
(for investigating what the acpphint sampling
stage behavior is like, GPLv2 only)
```
The intermediate .o files are put in `../obj/`. There is:
```
make -f makefile.NAME clean
```
But it just cleans out the `../objs/` content and possibly other system specific extra material, such as `../*.dSYM/` directories on examples for macOS. There is also:
```
make -f makefile.NAME clean-programs
```
to remove the 4 programs for NAME from `../`.

##### How to avoid building some `DSIZE`/`ISIZE` combinations . . .
`main` in `acpphint_src/accphint_main.cpp` can be edited to disable various `DSIZE`/`ISIZE` combinations that might not be of interest, i.e., disabling some of the
```
report_varying_threading<DSIZE,ISIZE>(. . .)
```
calls, such as:
```
report_varying_threading<short,short>
                           (filename_prefix,clock_info);
```
If you do such, you may want to similarly disable compiling the supporting code, say to make builds take less time.

`acpphint_kernels.cpp` has, for example (abbreviated/line-split),
```
template
PrimaryKernelInputs<short,short>
                           ::PrimaryKernelInputs(. . .);
template
auto Kernel<short,short>(. . .)
                          -> KernelResults<short,short>;
```
that expands to code for `DSIZE=short`/`ISIZE=short` .

`acpphint_kernelrunners.cpp` has, for example,
```
template
auto KernelRunner<short,short>(. . .)
                    -> KernelRunnerResults<short,short>;
```
that depends on `PrimaryKernelInputs<short,short>` and `Kernel<short.short>` .

`acpphint_kernelsurveyors.cpp` has, for example,
```
template
auto KernelSurveyor<short,short>(. . .)
                  -> KernelSurveyorResults<short,short>;
```
That depends on `KernelRunner<short,short>` and the like.

`main` in `acpphint_kernelsurveyors_main` has, for example,
```
    std::cout
        << "DSIZE=short, ISIZE=short:\n"
        << "\n";
    
    report_survey<short,short>(clock_info);
```
which would also need to be disabled for the example, presuming that you build the extra program.

`acpphint_kernelsamplers.cpp` has, for example,
```
template
auto KernelSampler<short,short>(. . .)
               -> KernelSamplerResultsVect<short,short>;
```
that depends on `KernelSurveyor<short,short>` and the like.

`main` in `acpphint_kernelsamplers_main.cpp` has, for example,
```
    std::cout
        << "\n"
        << "DSIZE=short, ISIZE=short:\n"
        << "\n";
    
    report_sampler<short,short>(clock_info);
```
  which would also need to be disabled for the example, presuming that you build the extra program. There is a `#if 0 . . . #endif` around a bunch of the `DSIZE`/`ISIZE` `report_sampler` combinations in how I keep things. (So you could end up wanting to enable some that I've disabled.)

##### How to run the benchmarks via a makefile.NAME . . .
The example makefile.NAME's all support:
```
make -f makefile.NAME gather-info
```
This runs all 4 programs, cd'd into `../`, putting a bunch of `*NAME*.txt` files in that `../`. I manually move the output files that I want to keep for a while into the `../acpphint_example_data/` and then manually clean up `../` when I'm ready. The file name structure for the files that result is (before any moves):
```
../acpphint-NAME-*.txt (benchmark data apropriate for
                        gnuplot)
../cpp_clockinfo_main-NAME-output.txt
../acpphint_kernelsurveyors_main-NAME-output.txt
../acpphint_kernelsamplers_main-NAME-output.txt
```
I warn that `acpphint-NAME` uses argv[0] text as the
base name (prefix text) for the file names/paths for where it saves benchmark data. The suffix text added indicates the DSIZE type, the ISIZE type, and the used-thread-count combination.

Some example makefile.NAME's support:
```
make -f makefile.NAME gather-NUMA-info
```
that use things like FreeBSD's cpuset command or Fedora's numactl command to run under example of types of NUMA contexts. These
examples also show use of some of the potential command line arguments argv[1], argv[2], argv[3] .

`acpphint_src/makefile.common_part` is where release version numbers are adjusted.

`./acpphint_example_data/` has example `*.gp` files for use with gnuplot for some of the provided example data gathered during development.

See also:
```
Usage_differences_with_original_HINTs.md
Code_issues_vs_original_HINTs.md
../acpphint_example_data/README.md
```
See `../*.md` for more overall information.

##### Licensing notes . . .

Overall, each `acpphint*-NAME` is licensed as GPLv2 (not earlier, not later). But building the acpphint related code does build some OpenBSD-vintage-ISC licensed source files, those under `other_src_used/` instead of under `acpphint_src/`. Some resulting object files are linked into the acpphint*-NAME related programs.

See the `acpphint_src/LICENSE.md` for the GPLv2 license text
that applies to the files in `acpphint_src/` and to the binary files containing code from them. Programs that do not have names that start with `acpphint` do not contain code from `acpphint_src/` ( just from `../other_src_used/` ).

See the `../other_src_used/LICENSE.md` for the OpenBSD-vintage-ISC license text that applies to just the source files in other_src_used/ and to any binaries not containing code from `acpphint_src/` source files.

Note: The original HINT's copyright holder, Iowa State University Research Foundation, Inc., reported that, despite how HINT was mistakenly distributed back then, the intended license was GPLv2 without any "or later" status: exactly GPLv2. This is what sets the overall license for this derivative work.
