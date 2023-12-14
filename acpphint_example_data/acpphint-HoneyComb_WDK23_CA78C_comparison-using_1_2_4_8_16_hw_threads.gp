reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_16_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n264334_215bab7924f6_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="clang++_16_O3lto_CA78C-libc++-cpulockdown"
FNAME2PREFIX=    "WDK23_CA78C-FBSD/acpphint-WDK23_CA78C-32768MiB-threads_8-LP64-FreeBSD_main_n264334_215bab7924f6_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSLOWERBOUND= 10e3
QUIPSUPPERBOUND= 1e12
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
set logscale y 10
set ytics border mirror
set mytics 10-1
RAMLABEL= "kernel vectors: total Bytes"
RAMLOGSCALE= 4
TIMELABEL= "Median for: mean seconds per lap"
TIMELOGSCALE= 10
set xlabel RAMLABEL
set logscale x RAMLOGSCALE
set xtics border mirror in RAMLOGSCALE rotate by -45
set mxtics RAMLOGSCALE-1
set grid xtics mxtics ytics mytics
plot [RAMLOWERBOUND:RAMUPPERBOUND][QUIPSLOWERBOUND:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_0_1_2_3_4_5_6_7.txt"  using 4:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_0_1_2_3_4_5_6_7.txt"    using 4:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_4_5_6_7.txt"  using 4:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_4_5_6_7.txt"    using 4:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_1_2_3.txt"  using 4:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_1_2_3.txt"    using 4:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_6_7.txt"  using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_6_7.txt"    using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_3.txt"  using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"    using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_4.txt"  using 4:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_4.txt"    using 4:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:2 with linespoints linecolor rgb "magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt"                          using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"                            using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_16_threads-cpus_kp1_0_15.txt"            using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_16_threads-cpus_kp1_0_15.txt"              using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"     using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p1_kx4p2_1_14.txt"       using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"     using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p2_kx4p3_2_15.txt"       using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_2_3_8_9.txt"              using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_10.txt"                 using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"                  using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta"
#    EOF
