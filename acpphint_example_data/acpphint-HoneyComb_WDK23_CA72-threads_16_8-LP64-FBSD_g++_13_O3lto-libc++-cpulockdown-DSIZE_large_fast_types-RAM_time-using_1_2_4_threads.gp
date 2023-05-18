reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_13_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n262658_b347c2284603_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_13_O3lto_CA72-libc++-cpulockdown"
FNAME2PREFIX=    "WDK23_CA72-FBSD/acpphint-WDK23_CA72-32768MiB-threads_8-LP64-FreeBSD_main_n262658_b347c2284603_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 1.8e8
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
set mztics 4
set grid xtics mxtics ytics mytics ztics mztics
plot [RAMLOWERBOUND:RAMUPPERBOUND][TIMELOWERBOUND:TIMEUPPERBOUND][0:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_4_5_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_4_5_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_1_2_3.txt"  using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_1_2_3.txt"    using 4:1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_6_7.txt"  using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_6_7.txt"    using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_3.txt"  using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"    using 4:1:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_4.txt"  using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_4.txt"    using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:1:2 with linespoints linecolor rgb "magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_2_3_8_9.txt"              using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 4:1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_10.txt"                 using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"                  using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 4:1:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 4:1:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 4:1:2 with linespoints linecolor rgb "dark-magenta"
#    EOF
