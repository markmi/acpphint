reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_11_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_11_O3lto-libc++-cpulockdown"
FNAME2PREFIX=    "MacchDblShot-FBSD/acpphint-MacchDblShot-16384MiB-threads_4-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 1e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  1.6
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
unset logscale y
set ytics border mirror
set mytics 5
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"     using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_3.txt"   using 4:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"     using 4:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"   using 4:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"     using 4:2 with linespoints linecolor rgb "light-salmon" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "violet" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"     using 4:2 with linespoints linecolor rgb "violet" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p1_kx8p3_1_3_9_11.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_2_3_8_9.txt"              using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_2_3_8_9.txt"                using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_10.txt"                 using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_10.txt"                   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt"                  using 4:2 with linespoints linecolor rgb "dark-salmon" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"                    using 4:2 with linespoints linecolor rgb "dark-salmon" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta"
#    EOF
