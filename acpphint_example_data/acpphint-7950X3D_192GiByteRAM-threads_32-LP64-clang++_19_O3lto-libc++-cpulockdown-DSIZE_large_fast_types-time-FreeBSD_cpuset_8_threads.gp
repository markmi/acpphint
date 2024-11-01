reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_19_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n273201_e4ac0183a1a8_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSUPPERBOUND= 1400e6
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "7950X3D QUIPs vs. kernel vectors total bytes, process under cpuset, cpulockdown also avoids repeated thread migrations within cpuset\n" . \
          "FNAMEPREFIX= " . FNAMEPREFIX noenhanced
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
 FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_16_18_28_30.txt" using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_16_18_28_30.txt"   using 1:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_14.txt"   using 1:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_6_16_22.txt" using 1:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_6_16_22.txt"   using 1:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4p1s_1_29.txt" using 1:2 with linespoints linecolor rgb "grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4p1s_1_29.txt"   using 1:2 with linespoints linecolor rgb "grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_16_17_28_29.txt" using 1:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_16_17_28_29.txt"   using 1:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_0_13.txt" using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_0_13.txt"   using 1:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_0_5_16_21.txt" using 1:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_0_5_16_21.txt" using 1:2 with linespoints linecolor rgb "dark-turquoise"
#    EOF

