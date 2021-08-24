#### acpphint_example_data/README.md
In general these notes are as of when they were written.

This covers:
1.  Licensing for generated-data-files and `.gp` files . . .
2.  The directory structure . . .
3.  Example gnuplot usage . . .
4.  gnuplot `.gp` file details . . .
5.  Benchmark data file details . . .
6.  Other data file details . . .

##### Licensing for generated-data-files, `.gp` files, and plot images . . .
There is no intent for any of the generated data files, the example `.gp` files
for use with gnuplot, or the plot images to have any licensing restrictions,
unlike, for example, the C++ source code and the associated binary files.

##### The directory structure . . .
`acpphint_example_data/` contains subdirectories for holding
the example data, such as:
```
HoneyComb-FBSD/
HoneyComb-Fedora/
HoneyComb-STOPTM_6/
MacchDblShot-FBSD/
MacchDblShot-Fedora/
OPi+2E-FBSD/
OverDrive_1000-FBSD/
RPi4B-FBSD/
Rock64-FBSD/
ThreadRipper1950X-nonuma-FreeBSD-pg_ps/
```
(I do not distribute binaries. But I also do not distribute the
source sys_cpubinding_lgplv2_1_okay.cpp that I used to prove
that a Linux was supportable for WANT_CPU_LOCKDOWN builds.
I just wanted to avoid covering yet more licensing criteria.)

`acpphint_example_data/` also contains some `*.gp`
files for example comparisons via a plot (using \* to shorten the
example name):
```
acpphint-OverDrive_1000-*_large_fast_types-RAM-using_2_threads-FreeBSD.gp
```
##### Example gnuplot usage (happens to be from macOS) . . .
Using load to make plots via `*.gp` files:
```
$ /usr/local/bin/gnuplot
. . .
gnuplot> cd '/Users/markmi/acpphint/acpphint_example_data/'
gnuplot> load 'acpphint-OverDrive_1000-threads_4-LP64-g++_10_O3lto-libc++-DSIZE_large_fast_types-RAM-using_2_threads-FreeBSD.gp'
```
##### gnuplot `*.gp` file details . .  .
The example `*.gp` files are for `GNUTERM = "x11"`.

The example `*.gp` files derive from a saved plot and then hand adjusting
(mostly) the tail end of copies the file(s). Overall: `*.gp` files are manually
created.
##### Benchmark data file details . . .
The following suggests what the data files look like but is based on a file no longer present:
```
# filename: ./acpphint-RPi4B-4096MiB-threads_4-LP64-Gentoo_2.6_4.19.67_v8-g++_9_O3-libstdc++-DSIZE_ull-ISIZE_ull-using_4_threads.txt
# acpphint_main version: 0.7.0
0.000137535 109063 15 1952
0.000136368 139329 19 2368
0.000136553 168433 23 2784
. . .
0.812229 7.62004e+06 6.18922e+06 645748768
1.01777 7.64108e+06 7.77683e+06 812949152
```
(The original example files predate the version number line being generated so I 
manually added the line here.)

The fields are:

1.  median trial's mean time per lap in sec
    (median from sort by mean time per lap)
2.  QUality ImProvement/sec (a.k.a QUIPS)
3.  Quality (a.k.a QUIPS * previously listed time per lap, 
    like in the original pthread HINT)
4.  kernel vectors total memory use

The default data file names fit the pattern:
```
acpphint-*-DSIZE_*-ISIZE_*-using_*_threads*.txt
```
The following is based on what I've picked as defaults for
what has code in place:

After `DSIZE_` is an abbreviation of the Data Type involved:
1.  s for short
2.  us for unsigned short
3.  f for float
4.  ui for unsigned int
5.  d for double
6.  ul for unsigned long
7.  ull for unsigned long long
8.  ld for long double

After `ISIZE_` is an abbreviation of the indexing type involved:
1.  s for short
2.  us for unsigned short
3.  ui for unsigned int
4.  ul for unsigned long
5.  ull for unsigned long long

Between `using_` and  `_threads` is the number of threads used.

After `_threads` but before `.txt` may be text indicating the specific
cpuset/taskset/. . . in use for the process involved or other such
identifying information. Some files are from using a benchmark command
line that replaced the `.txt` with other text (that happened to also end
in `.txt`).
###### Other data file details . . .
The benchmark's std::cout reports what DSIZE/ISIZE/#threads combinations could
not be run (too little to do). For those combinations that are run, it report
what stopped the run (such as the mead mean time per lap reaching a bound) and
also what stopping constraints were not reached.

At this point I'm not documenting the files from the other 3 programs,
files with names matching the patterns:
```
cpp_clockinfo_main-*-output.txt
acpphint_kernelsurveyors_main-*-output.txt
acpphint_kernelsamplers_main-*-output.txt
```
