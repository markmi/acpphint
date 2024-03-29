reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_13_O3lto-libc++"
FNAMEPREFIX=     "HoneyComb-OBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-OpenBSD_7p2STABLE_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="clang++_13_O3lto-libc++"
FNAME2PREFIX=    "HoneyComb-OBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-OpenBSD_7p2STABLE_NODIAG_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 1.8e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
unset logscale y
set ytics border mirror
set mytics 4
RAMLABEL= "kernel vectors: total Bytes"
RAMLOGSCALE= 4
TIMELABEL= "Median for: mean seconds per lap"
TIMELOGSCALE= 10
set xlabel RAMLABEL
set logscale x RAMLOGSCALE
set xtics border mirror in RAMLOGSCALE rotate by -45
set mxtics RAMLOGSCALE-1
set grid xtics mxtics ytics mytics
plot [RAMLOWERBOUND:RAMUPPERBOUND][0:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads-cpus_0_15.txt" using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads-cpus_0_15.txt"   using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_12_threads-cpus_0_15.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_12_threads-cpus_0_15.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_6_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_6_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_16_threads-cpus_0_15.txt" using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_16_threads-cpus_0_15.txt"   using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_12_threads-cpus_0_15.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_12_threads-cpus_0_15.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_6_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_6_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0_15.txt"  using 4:2 with linespoints linecolor rgb "magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0_15.txt"    using 4:2 with linespoints linecolor rgb "magenta" \
#    EOF
