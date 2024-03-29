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
TIMEUPPERBOUND=  10
set title "1950X, 1CCD 2CCX 2/4 hwthreads, process under cpuset, cpulockdown also avoids repeated thread migrations within cpuset\n" . \
          "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx4_kx4p1_0_9.txt"         using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx4_kx4p1_0_9.txt"           using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8_kx8p2_0_2_8_10.txt"    using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8_kx8p2_0_2_8_10.txt"      using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx4s_0_12.txt"             using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx4s_0_12.txt"               using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_8.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_8.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx4_kx4p1_0_9.txt"         using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx4_kx4p1_0_9.txt"           using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8_kx8p2_0_2_8_10.txt"    using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8_kx8p2_0_2_8_10.txt"      using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx4s_0_12.txt"             using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx4s_0_12.txt"               using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_8.txt"  using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_8.txt"    using 4:2 with linespoints linecolor rgb "light-magenta"
#    EOF
