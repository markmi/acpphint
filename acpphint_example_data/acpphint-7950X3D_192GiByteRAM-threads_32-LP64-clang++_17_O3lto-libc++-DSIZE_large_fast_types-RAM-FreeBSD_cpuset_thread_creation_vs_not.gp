reset session
GNUTERM = "x11"
COMPILERLIBRARY= "clang++_17_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "7950X3D-nonuma-FBSD-pg_ps/acpphint-7950X3D-196608MiB-threads_32-LP64-FreeBSD_main_n266876_e183039f0882_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**38
QUIPSUPPERBOUND= 1600e6
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
set xlabel RAMLABEL
set logscale x RAMLOGSCALE
set xtics border mirror in RAMLOGSCALE rotate by -45
set mxtics RAMLOGSCALE-1
set grid xtics mxtics ytics mytics
plot [RAMLOWERBOUND:RAMUPPERBOUND][0:QUIPSUPPERBOUND] \
 FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads_creation_untimed-cpus_kx2s_0_30.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads_creation_untimed-cpus_kx2s_0_30.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads-cpus_kx2s_0_30.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads-cpus_kx2s_0_30.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_32_threads_creation_untimed.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_32_threads_creation_untimed.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_32_threads.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_32_threads.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads_creation_untimed-cpus_kx4_kx4p1_0_29.txt" using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads_creation_untimed-cpus_kx4_kx4p1_0_29.txt"   using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_16_threads-cpus_kx4_kx4p1_0_29.txt" using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_16_threads-cpus_kx4_kx4p1_0_29.txt"   using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx2s_0_14.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_14.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_14.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx2s_16_18_28_30.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx2s_16_18_28_30.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_16_18_28_30.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_16_18_28_30.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx2s_0_6_16_22.txt" using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx2s_0_6_16_22.txt"   using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx2s_0_6_16_22.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx2s_0_6_16_22.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx4_kx4p1_0_13.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx4_kx4p1_0_13.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_0_13.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_0_13.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx4_kx4p1_16_17_28_29.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx4_kx4p1_16_17_28_29.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_16_17_28_29.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_16_17_28_29.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads_creation_untimed-cpus_kx4_kx4p1_0_5_16_21.txt" using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads_creation_untimed-cpus_kx4_kx4p1_0_5_16_21.txt"   using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_8_threads-cpus_kx4_kx4p1_0_5_16_21.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_8_threads-cpus_kx4_kx4p1_0_5_16_21.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p2_16_18_24_26.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p2_16_18_24_26.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p4_16_20_24_28.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx8p2_kx8p4_18_20_26_28.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx8p2_kx8p4_18_20_26_28.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p4_16_20_24_28.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p2_16_18_24_26.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p2_16_18_24_26.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p4_16_20_24_28.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p4_16_20_24_28.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8p2_kx8p4_18_20_26_28.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8p2_kx8p4_18_20_26_28.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p2_0_2_16_18.txt" using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p2_0_2_16_18.txt"   using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p4_0_4_16_20.txt" using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p4_0_4_16_20.txt"   using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx8s_0_24.txt"             using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx8s_0_24.txt"               using 4:2 with linespoints linecolor rgb "dark-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p2_0_2_16_18.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p2_0_2_16_18.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p4_0_4_16_20.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p4_0_4_16_20.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx8s_0_24.txt"             using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx8s_0_24.txt"               using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p1_16_17_24_25.txt" using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p1_16_17_24_25.txt"   using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p1_16_17_24_25.txt" using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p1_16_17_24_25.txt"   using 4:2 with linespoints linecolor rgb "light-red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads_creation_untimed-cpus_kx16_kx16p1_0_1_16_17.txt" using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads_creation_untimed-cpus_kx16_kx16p1_0_1_16_17.txt"   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_kx16_kx16p1_0_1_16_17.txt" using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_kx16_kx16p1_0_1_16_17.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_16_24.txt" using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_16_24.txt"   using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_16_24.txt" using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_16_24.txt"   using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_16.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_16.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_16.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_16.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_16_17.txt" using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_16_17.txt"   using 4:2 with linespoints linecolor rgb "dark-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_16_17.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_16_17.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpu_16.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpu_16.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
#    EOF

