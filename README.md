##### README.md for acpphint
This covers:
1.  What `acpphint` is a variation on . . .
2.  Where are the various README.md files?
3.  Licensing notes . . .
4.  Changelog . . .

##### What `acpphint` is a variation on . . .

`acpphint` is a variation on the original HINT serial and pthread benchmarks, but written in C++ and with various choices specific to my use. The benchmark program still generates files appropriate for gnuplot use, but not fully matching the original HINT benchmark's output files.

I am not trying to collect/archive reports of results.

An example gnuplot from an OverDrive 1000 is:
![OverDrive 1000, QUIPS vs. RAM](acpphint_example_data/acpphint-OverDrive_1000-threads_4-LP64-g++_10_O3lto-libc++-DSIZE_large_fast_types-RAM-using_2_threads-cpu_migration_FreeBSD.png)
QUality Improvement/sec (QUIPS) is based on QUality in a hierarchical integration problem, Quality being `1/(high_bound-low_bound)`. So higher is better. As the total size of the kernel vectors grows noticably beyond a cache size, the cache tends to become fairly ineffective. So the right hand side tends to be memory subsystem bandwidth constrained for the stopping constraints that I've used. (I used stopping-constraints that avoided using kernel vector sizes that would cause paging, for example.)

The dark/bright is for without cpu lock down code and the light is for with cpu lock down code. `_ull` for `unsigned long long`, `_ul` for `unsigned long`. (There are other data types measured/recorded, just not plotted here.) `DSIZE` is for the Data Type. `ISIZE` is for the Indexing Type (not double or other floating point types).

Another plot from the same data file is:
![OverDrive 1000, QUIPS vs. median of mean lap times](acpphint_example_data/acpphint-OverDrive_1000-threads_4-LP64-g++_10_O3lto-libc++-DSIZE_large_fast_types-time-using_2_threads-cpu_migration_FreeBSD.png)

This tends to show the latency involved for small sized kernel vectors, where single threaded does not measure any thread creation time or the like but the multi-threaded cases do.

##### Where are the various README.md files?
The various README.md files are:
```
README.md                       (this file)
acpphint_src/README.md          (the main README.md)
acpphint_example_data/README.md (.gp gnulot usage too)
other_src_used/README.md        (other source code used)
```
(I normally have  a directory called `acpphint`  as the top of the tree of the acpphint source distribution.) It is the `acpphint_src/README.md` that covers how to build and how to run for the way the examples are set up. Material about using the example data files is in: `acpphint_example_data/README.md` .

##### Licensing notes . . .
The various LICENSE.md files are:
```
LICENSE.md                (the overall license: GPLv2)
acpphint_src/LICENSE.md   (GPLv2 for HINT code variations)
acpphint_example_data/LICENSE.md (None for the example data)
other_src_used/LICENSE.md (OpenBSD-vintage ISC source)
```
Overall, each `acpphint*-NAME` program is licensed as GPLv2 (not earlier, not later). But building the `acpphint related code does build some OpenBSD-vintage-ISC licensed source files, those under `other_src_used/ instead of under `acpphint_src/. Some resulting object files are linked into the acpphint\*-NAME related programs (GPLv2 programs).

See the `acpphint_src/LICENSE.md` for the GPLv2 license text that applies to the files in `acpphint_src/` and to the binary files containing code from them. As I have the things structured, programs that do not have names that start with `acpphint do not contain code from acpphint_src/` ( just from other_src_used/` ).

See the `other_src_used/LICENSE.md` for the OpenBSD-vintage ISC license text that applies to just the source files in `other_src_used/` and to any binaries not containing code from `acpphint_src/` source files.

Note: The original HINT's copyright holder, Iowa State University Research Foundation, Inc., reported that, despite how HINT was mistakenly distributed back then, the intended license was GPLv2 without any "or later" status: exactly GPLv2. This is what sets the license for the source code in `acpphint_src/`. (I did not bother to make distinct licensing for the `acpphint_src/makefile.*` examples.) That code's license in turn sets the overall license to also be GLPv2 (no earlier, no later), despite OpenBSD_vintage ISC licensed source in `other_src_used/` also being built and linked in.
##### Changelog . . .
###### [0.1.24] 2021-Jul-06 (start) . . . 2021-???-?? (final)
Additions
- Add HoneyComb tests for at least FreeBSD.

- Add code based on C++20 --and some C++17 that is now
  available: The output of the scales vs. the known
  approximation to the result now uses std::to_chars
  for the floating point output when available.

- Pending: Start a round of updating to more modern
  FreeBSD and other OS vintages.

- Pending: Add some gnuplot .gp files for exploring
  the new data.

Changes
- Update minor things in the sources to silence new
  warnings.

Removals
- The OverDrive 1000 and the other PowerMac11,2 "bit
  the dust". I'll keep some plots around that involve
  the OverDrive 1000, but possibly not the data files
  and certainly not the makefile.OverDrive* files.
  Similarly for the PowerMac11,2's now that I've no
  access to working systems of that kind.

- Pending: Various older example data files, makefile.*
  files, *.gp files, and the like will be removed.
  Some of the old makefile.* files may end up still
  around as examples.

###### [0.1.23] 2020-Aug-03 (start) . . . 2020-Oct-30 (final)
Additions
- Actaully get the FreeBSD and Linux cpu lock down
  code to be part of such builds and make the code
  actually not be rejected. Also have it work. (I
  do not distrute binaries at all --nor that linux
  specific source, avoiding sppending time on the
  additional licensing criteria that may be involved.
  But I've checked that such linux code is possible.)

- Add some FreeBSD head -r363590 data, g++10
  -O3 LTO based examples. (Exploring -O2, -O3, and
  -O3 -flto did not find great differences, but I
  stuck with the last that I tried.) Matching
  makefile.* files and the like are involved.
  I still founds that g++ produced more performant
  benchamrk code compared to clang for what I
  looked at.

- Add some Fedora 32 ThreadRipper 1950X data, g++10
  -O3 LTO based. Add some ubuntu 20.04.1 RPi4B data,
  g++9 -O3 LTO based.

- Add some gnuplot .gp files for exploring the new
  data.

Changes
- Update minor things in the sources to silence warnings
  after having added -Wpedantic -Wall -Wextra . Echo
  more information in the cout material, mostly explicit
  values in involved in the reported relationships.

Removals
- Older example data files, makefile.* files, *.gp files,
  and the like have mostly been removed. Some of the old
  makefile.* files are around as examples.

- The Pine64+ 2GB and PowerMac11,2 that allowed all
  memory slots to be used "bit the dust". As such I'm
  reverting to no coverage of old PowerMacs or the
  Pine64+ 2GB. (FreeBSD has 32-bit power pc messed
  up currently.)

###### [0.1.22] 2020-Jan-22
Additions
- Add FreeBSD head -r356426 data for Rock64,
  MacchDblShot, and OverDrive 1000. Add some gnuplot
  .gp files for comparing older examples with
  -r356426 based examples.

Changes
- Update various gnuplot .gp files to use FreeBSD
  head -r356426 based data.

Removals
- None.

###### [0.1.21] 2020-Jan-22
Additions
- Add FreeBSD head -r356426 powerpc64 (PowerMac11,2)
  results. This is an ELFv2 ABI context, instead of
  the ELFv1 context that the prior results were for.
  (No g++9 vs. system-libc++ ABI mismatches showed
  up, unlike for the earlier 32-bit powerpc test
  of -r356246 . 32-bit was not trying to update its
  ABI but did switch to the system being
  clang/clang++ based --a new mismatch resulted.)

Changes
- None.

Removals
- None.

###### [0.1.20] 2020-Jan-22
Additions
- Add RPi4B Ubuntu 19.10.1 server results.
  Added some ThreadRipper .gp files that had not
  not been checked in.

Changes
- Also updated to have the removal of a comment
  that for which the update had not been checked
  in.

Removals
- None.

###### [0.1.19] 2020-Jan-12
Additions
- Add dual socket PowerMac3,6 G4 data, but based on
  -maix-struct-return to match the odd(?) clang code
  generation for 32-bit powerpc. (May well have to
  replace later if clang is "corrected" (but that 
  would be antoehr ABI change).

Changes
- Fix various typos and out of date comments.

Removals
- None.

###### [0.1.18] 2019-Dec-19
Additions
- Add FreeBSD pg_ps enabled example of stopping once
  having reached 6sec+ for the median trial time on the
  ThreadRipper 1950X. The previous examples were for
  pg_ps disabled (so large page mappings disabled).
  The 6+ seconds gets to a little under 95.77 GiBytes.

Changes
- None.

Removals
- None.

###### [0.1.17] 2019-Dec-18
Additions
- Copy prior ThreadRipper 1950X data to a directory
  for it being a re-run and put in place new data
  from a just-after-boot run for comparison.

Changes
- Where the original data is found.

Removals
- None.

###### [0.1.16] 2019-Dec-18
Additions
- Add STOPTM_ALT macro for using -DSTOPTM_ALT=... to
  compile-time control the median trial time stopping
  criterian. Also add NSAMP_ALT for using -DNSAMP_ALT=...
  in case it is needed to reach the STOPTM_ALT value.
  Add more ThreadRipper1950X example material based on
  using these. Still no .gp or .png for the
  ThreadRipper additions.

Changes
- Of course, the STOPTM_ALT and NSAMP_ALT additions
  are changes to acpphint_kernelsamplers.cpp .

Removals
- None.

###### [0.1.15] 2019-Dec-17
Additions
- Add examples from a FreeBSD ThreadRipper 1950X
  with 128 GiByte of RAM. BIOS (well, Ryzen Master)
  NUMA vs. nonNUMA for the same head -r355777 based
  build, also pg_ps enabled (large page mappings
  allowed) vs. disabled. No exploration of round
  robin or other such for NUMA: just defaults.

Changes
- None.

Removals
- None.

###### [0.1.14] 2019-Dec-02
Additions
- Add FreeBSD MACCHIATObin Double Shot vs.
  OverDrive 1000 and PowerMac11,2 .png files.

Changes
- None.
  
Removals
- None.

###### [0.1.13] 2019-Dec-01
Additions
- Add FreeBSD MACCHIATObin Double Shot vs.
  PowerMac11,2 .gp file.

Changes
- Correct 0.1.12 date.
  
Removals
- None.

###### [0.1.12] 2019-Dec-01-23
Additions
- Add FreeBSD MACCHIATObin Double Shot files,
  based on running in the 0.1.11 context.

Changes
- Adjust makefile.common_part to indicate v0.1.11 .
  
Removals
- None.

###### [0.1.11] 2019-Nov-23
Additions
- None.

Changes
- Have the FreeBSD vs. ArchLinuxARM comparison .gp
  file list FreeBSD first, ArchLinuxARM second in
  the plots. With sysctl dev.cpu.0.freq=1296 FreeBSD
  is faster in some contexts. Also rename the file
  to list FreeeBSD apects before the matching
  ArchLinuxARM aspects.
- Adjust makefile.common_part to indicate v0.1.11 .
  
Removals
- None.

###### [0.1.10] 2019-Nov-23
Additions
- None.

Changes
- Update Rock64 benchmark data to be based on
  manually having set: sysctl dev.cpu.0.freq=1296
- Adjust makefile.common_part to indicate v0.1.10 .
  
Removals
- None.

###### [0.1.9] 2019-Nov-23
Additions
- Add Rock64 .gp files. Note: The benchmark data
  indicates that FreeBSD is not changing from
  600MHz for the CPU to faster, such as around
  1200MHz. This looks to be manually adjustable
  so I'll likely get new benchmark data later.

Changes
- Adjust makefile.common_part to indicate v0.1.9 .
  
Removals
- None.

###### [0.1.8] 2019-Nov-23
Additions
- Add Rock64 FreeBSD 13 files, but not yet .gp files.
  These were counted as running under v0.1.7 .

Changes
- Adjust makefile.common_part to indicate v0.1.8 .
  
Removals
- None.

###### [0.1.7] 2019-Nov-21
Additions
- Add Rock64 .gp files.

Changes
- Adjust makefile.common_part to indicate v0.1.7 .
  
Removals
- None.

###### [0.1.6] 2019-Nov-21
Additions
- Add Rock64 example's material, no .gp files yet.

Changes
- Update future compiler options for MACCHIATObin
  and RPi4B to be explicit about the cpu types to
  target. Other minor text edits in the 2 makefiles.
  Materials were not regenerated.
- Adjust makefile.common_part to indicate v0.1.6 .
  
Removals
- None.

###### [0.1.5] 2019-Nov-16
Additions
- Add a .gp file (for gnuplot use) that compares
  a MACCHIATObin Double Shot to a Raspberry Pi 4B:
  Both have an A72, but with very diifferent memory
  subsystems, as well as different cpu clock rates.

Changes
- Adjust makefile.common_part to indicate v0.1.5 .
  
Removals
- None.

###### [0.1.4] 2019-Nov-16
Additions
- Add iMac18,3 Corei5 example's material.
  (The .gp file, for gnuplot use, is untested.)

Changes
- Adjust makefile.common_part to indicate v0.1.4 .
  
Removals
- None.

###### [0.1.3] 2019-Nov-16
Additions
- Add MACCHIATObin double shot example's material.
  (The .gp file, for gnuplot use, is untested.)

Changes
- Adjust makefile.common_part to indicate v0.1.3 .
  
Removals
- None.

###### [0.1.2] 2019-Oct-12
Additions
- None beyond initial addition of files.

Changes
- Fix a typo in README.md to explore making a change.
- Adjust makefile.common_part to indicaite v0.1.2 .
  
Removals
- None.
