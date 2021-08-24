reset session
GNUTERM = "x11"
COMPILERLIBRARY2="g++_11_O3lto-libc++"
FNAME2PREFIX=    "ThreadRipper1950X-nonuma-FBSD-pg_ps/acpphint-ThreadRipper1950X-131072MiB-threads_32-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY2
COMPILERLIBRARY= "g++_11_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "ThreadRipper1950X-nonuma-FBSD-pg_ps/acpphint-ThreadRipper1950X-131072MiB-threads_32-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSUPPERBOUND= 5e8
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_1.txt"  using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_1.txt"    using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_2.txt"  using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_2.txt"    using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_4.txt"  using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_4.txt"    using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_8.txt"  using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_8.txt"    using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_16.txt" using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_16.txt"   using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_1.txt"  using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_1.txt"    using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_2.txt"  using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_2.txt"    using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_4.txt"  using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_4.txt"    using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_8.txt"  using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_8.txt"    using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_16.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_16.txt"   using 4:2 with linespoints linecolor rgb "light-blue"
#    EOF