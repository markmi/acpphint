reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_12_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n259950_5723e5ac6d76_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_12_O3lto-libc++"
FNAME2PREFIX=    "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n259950_5723e5ac6d76_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 1.8e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10.0
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt"            using 1:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"              using 1:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_15_threads-cpus_kp1_1_15.txt"            using 1:2 with linespoints linecolor rgb "light-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_15_threads-cpus_kp1_1_15.txt"              using 1:2 with linespoints linecolor rgb "light-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"     using 1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"       using 1:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"     using 1:2 with linespoints linecolor rgb "light-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"       using 1:2 with linespoints linecolor rgb "light-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 1:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 1:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_2_3_8_9.txt"              using 1:2 with linespoints linecolor rgb "light-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 1:2 with linespoints linecolor rgb "light-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_10.txt"                 using 1:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 1:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"                  using 1:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 1:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 1:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 1:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt"            using 1:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"              using 1:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_15_threads-cpus_kp1_1_15.txt"            using 1:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_15_threads-cpus_kp1_1_15.txt"              using 1:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"     using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"       using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"     using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"       using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_2_3_8_9.txt"              using 1:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 1:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_10.txt"                 using 1:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 1:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"                  using 1:2 with linespoints linecolor rgb "dark-salmon" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 1:2 with linespoints linecolor rgb "dark-salmon" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 1:2 with linespoints linecolor rgb "dark-magenta"
#    EOF
