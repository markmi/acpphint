reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_16_O3lto-libc++-ILP32-cpulockdown"
FNAMEPREFIX=     "HoneyComb-FBSD-ILP32/acpphint-HoneyComb-65536MiB-threads_16-ILP32-FreeBSD_main_n264334_215bab7924f6_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="clang++_16_O3lto_CA78C-libc++-ILP32-cpulockdown"
FNAME2PREFIX=    "WDK23_CA78C-FBSD-ILP32/acpphint-WDK23_CA78C-32768MiB-threads_8-ILP32-FreeBSD_main_n264334_215bab7924f6_64bit-" . COMPILERLIBRARY2
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
set xlabel TIMELABEL
set logscale x TIMELOGSCALE
set xtics border mirror in TIMELOGSCALE rotate by -45
set mxtics TIMELOGSCALE-1
set grid xtics mxtics ytics mytics
plot [TIMELOWERBOUND:TIMEUPPERBOUND][0:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_8_threads-cpus_0_1_2_3_4_5_6_7.txt"  using 1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_0_1_2_3_4_5_6_7.txt"    using 1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_4_threads-cpus_4_5_6_7.txt"  using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_4_5_6_7.txt"    using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_4_threads-cpus_0_1_2_3.txt"  using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_1_2_3.txt"    using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_2_threads-cpus_6_7.txt"  using 1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_6_7.txt"    using 1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_2_threads-cpus_1_3.txt"  using 1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"    using 1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_1_threads-cpus_4.txt"  using 1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_4.txt"    using 1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ui-ISIZE_ui-using_1_threads-cpus_1.txt"  using 1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 1:2 with linespoints linecolor rgb "magenta" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"     using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"       using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"     using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"       using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_4_threads-cpus_2_3_8_9.txt"              using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_2_threads-cpus_2_10.txt"                 using 1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_2_threads-cpus_2_3.txt"                  using 1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ui-ISIZE_ui-using_1_threads-cpus_6.txt"  using 1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 1:2 with linespoints linecolor rgb "dark-magenta"
#    EOF