reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_10_O3lto-libstdc++-cpulockdown"
FNAMEPREFIX=     "ThreadRipper1950X-nonuma-Fedora/acpphint-ThreadRipper1950X-131072MiB-threads_32-LP64-Fedora_32-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_10_O3lto-libstdc++"
FNAME2PREFIX=    "ThreadRipper1950X-nonuma-Fedora/acpphint-ThreadRipper1950X-131072MiB-threads_32-LP64-Fedora_32-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSUPPERBOUND= 4e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  1.6
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality Improvement/sec"
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt"  using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"    using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads.txt"  using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads.txt"    using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"     using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads.txt"  using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads.txt"    using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"     using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads.txt"     using 4:2 with linespoints linecolor rgb "light-turquoise" \
#    EOF
