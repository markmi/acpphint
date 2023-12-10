reset session
GNUTERM = "x11"
COMPILERLIBRARY2="clang++_17_O3lto-libc++-cpulockdown"
FNAME2PREFIX=    "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n266876_e183039f0882_64bit-" . COMPILERLIBRARY2
COMPILERLIBRARY= "g++_13_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n266876_e183039f0882_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSUPPERBOUND= 1100e6
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "7950X3D QUIPs vs. kernel vectors total bytes, process under cpuset, cpulockdown also avoids repeated thread migrations within cpuset\n" . \
          "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
unset logscale y
set ytics border mirror
set mytics 2
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_14.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_14.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_16_18_28_30.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_16_18_28_30.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_16_18_28_30.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_16_18_28_30.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpu_16.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpu_16.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpu_16.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpu_16.txt"   using 4:2 with linespoints linecolor rgb "light-grey"
#    EOF

