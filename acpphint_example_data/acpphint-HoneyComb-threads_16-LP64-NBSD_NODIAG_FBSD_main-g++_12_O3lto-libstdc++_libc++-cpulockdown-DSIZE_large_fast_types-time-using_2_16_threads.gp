reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_12_O3lto-libstdc++-cpulockdown"
FNAMEPREFIX=     "HoneyComb-NBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-NetBSD_10p0BETA_NODIAG_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_12_O3lto-libc++-cpulockdown"
FNAME2PREFIX=    "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n259950_5723e5ac6d76_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSUPPERBOUND= 1.8e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
unset logscale y
set ytics border mirror
set mytics 10
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt" using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"   using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads.txt"  using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads.txt"    using 1:2 with linespoints linecolor rgb "goldenrod" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt" using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"   using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads.txt"  using 1:2 with linespoints linecolor rgb "dark-plum" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads.txt"    using 1:2 with linespoints linecolor rgb "brown" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt"  using 1:2 with linespoints linecolor rgb "green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"    using 1:2 with linespoints linecolor rgb "green"
#    EOF
