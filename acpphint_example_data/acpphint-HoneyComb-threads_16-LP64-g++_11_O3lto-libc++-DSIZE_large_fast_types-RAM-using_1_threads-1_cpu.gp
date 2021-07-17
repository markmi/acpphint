reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_11_O3lto-libc++"
FNAMEPREFIX=     "HoneyComb-FBSD/acpphint-HoneyComb-65536MiB-threads_16-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_11_O3lto-libc++"
FNAME2PREFIX=    "MacchDblShot-FBSD/acpphint-MacchDblShot-16384MiB-threads_4-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**34
QUIPSUPPERBOUND= 4e7
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0.txt"  using 4:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0.txt"    using 4:2 with linespoints linecolor rgb "light-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_2.txt"  using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_2.txt"    using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_3.txt"  using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_3.txt"    using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0.txt"  using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0.txt"    using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_6.txt"  using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_6.txt"    using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_11.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_11.txt"   using 4:2 with linespoints linecolor rgb "dark-grey"
#    EOF
