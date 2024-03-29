reset session
GNUTERM = "x11"
COMPILERLIBRARY3="g++_11_O3lto-libc++-cpulockdown"
FNAME3PREFIX=    "ThreadRipper1950X-nonuma-FBSD-pg_ps/acpphint-ThreadRipper1950X-131072MiB-threads_32-LP64-FreeBSD_main_n247756_348c41d1815d_64bit-" . COMPILERLIBRARY3
COMPILERLIBRARY2="clang++_17_O3lto-libc++-cpulockdown"
FNAME2PREFIX=    "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n266876_e183039f0882_64bit-" . COMPILERLIBRARY2
COMPILERLIBRARY= "g++_13_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n266876_e183039f0882_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSLOWERBOUND= 10e3
QUIPSUPPERBOUND= 1e12
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "Compared QUIPs vs. kernel vectors total bytes, process under cpuset, cpulockdown also avoids repeated thread migrations within cpuset\n" . \
          "FNAME3PREFIX= " . FNAME3PREFIX . "\nFNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
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
 FNAME3PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME3PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_14.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME3PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_6_16_22.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME3PREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_6_16_22.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAME3PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAME3PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
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

