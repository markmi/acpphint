reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_19_O3lto_CA76-libc++-cpulockdown"
FNAMEPREFIX=     "WDK23_CA78C-FBSD/acpphint-WDK23_CA78C-32768MiB-threads_8-LP64-FreeBSD_main_n273201_e4ac0183a1a8_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_14_O3lto_CA76-libstdc++-cpulockdown"
FNAME2PREFIX=    "WDK23_CA78C-FBSD/acpphint-WDK23_CA78C-32768MiB-threads_8-LP64-FreeBSD_main_n273201_e4ac0183a1a8_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 3.0e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
RAMLABEL= "kernel vectors: total Bytes"
RAMLOGSCALE= 4
TIMELABEL= "Median for: mean seconds per lap"
TIMELOGSCALE= 10
set xlabel RAMLABEL
set logscale x RAMLOGSCALE
set xtics border mirror in RAMLOGSCALE rotate by -45
set mxtics RAMLOGSCALE-1
set ylabel TIMELABEL
set logscale y TIMELOGSCALE
set ytics border mirror rotate by -45
set mytics TIMELOGSCALE-1
set zlabel "QUality ImProvement/sec"
unset logscale z
set ztics border mirror
set mztics 5
set grid xtics mxtics ytics mytics ztics mztics
splot [RAMLOWERBOUND:RAMUPPERBOUND][TIMELOWERBOUND:TIMEUPPERBOUND][0:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_0_1_2_3_4_5_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_0_1_2_3_4_5_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_4_5_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_4_5_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_1_3_4_6.txt"  using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_1_3_4_6.txt"    using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_0_1_2_3.txt"  using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_0_1_2_3.txt"    using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_2_6.txt"  using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_2_6.txt"    using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_3.txt"  using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_3.txt"    using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_4.txt"  using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_4.txt"    using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_0_1_2_3_4_5_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_0_1_2_3_4_5_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_4_5_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_4_5_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_1_3_4_6.txt"  using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_1_3_4_6.txt"    using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_0_1_2_3.txt"  using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_0_1_2_3.txt"    using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_2_6.txt"  using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_2_6.txt"    using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_3.txt"  using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_3.txt"    using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_4.txt"  using 4:1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_4.txt"    using 4:1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:1:2 with linespoints linecolor rgb "dark-magenta"
#    EOF