reset session
GNUTERM = "x11"
COMPILERLIBRARY= "g++_10_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "OverDrive_1000-FBSD-boot_s/acpphint-OverDrive_1000-8192MiB-threads_4-LP64-FreeBSD_13_r363590M_64bit-" . COMPILERLIBRARY
COMPILERLIBRARY2="g++_10_O3lto-libc++"
FNAME2PREFIX=    "OverDrive_1000-FBSD-boot_s/acpphint-OverDrive_1000-8192MiB-threads_4-LP64-FreeBSD_13_r363590M_64bit-" . COMPILERLIBRARY2
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**34
QUIPSUPPERBOUND= 6e7
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
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_1_2_3.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_1_2_3.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads-cpus_0_1_2.txt" using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads-cpus_0_1_2.txt"   using 4:2 with linespoints linecolor rgb "dark-grey" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_3.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_3.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_2.txt" using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_2.txt"   using 4:2 with linespoints linecolor rgb "dark-green" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_2.txt" using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_2.txt"   using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_3.txt" using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"   using 4:2 with linespoints linecolor rgb "dark-red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_1.txt" using 4:2 with linespoints linecolor rgb "red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_1.txt"   using 4:2 with linespoints linecolor rgb "red" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt" using 4:2 with linespoints linecolor rgb "red" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"   using 4:2 with linespoints linecolor rgb "red" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads-cpus_0_1_2_3.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads-cpus_0_1_2_3.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads-cpus_0_1_2.txt" using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads-cpus_0_1_2.txt"   using 4:2 with linespoints linecolor rgb "light-grey" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_3.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_3.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_2.txt" using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_2.txt"   using 4:2 with linespoints linecolor rgb "light-green" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_2.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_2.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_1_3.txt" using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_1_3.txt"   using 4:2 with linespoints linecolor rgb "light-blue" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_0_1.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_0_1.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads-cpus_2_3.txt" using 4:2 with linespoints linecolor rgb "light-goldenrod" \
,FNAMEPREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads-cpus_2_3.txt"   using 4:2 with linespoints linecolor rgb "light-goldenrod" \
#    EOF
