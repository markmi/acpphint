reset session
GNUTERM = "x11"
COMPILERLIBRARY2="clang++_19_O3lto-libc++"
FNAME2PREFIX=    "RPi5-FBSD/acpphint-RPi5-8192MiB-threads_4-LP64-FreeBSD_main_n273201_e4ac0183a1a8_64bit-" . COMPILERLIBRARY2
COMPILERLIBRARY= "clang++_19_O3lto-libc++-cpulockdown"
FNAMEPREFIX=     "RPi5-FBSD/acpphint-RPi5-8192MiB-threads_4-LP64-FreeBSD_main_n273201_e4ac0183a1a8_64bit-" . COMPILERLIBRARY
RAMLOWERBOUND=   64
RAMUPPERBOUND=   2**37
QUIPSLOWERBOUND= 0
QUIPSUPPERBOUND= 1.2e8
TIMELOWERBOUND=  1e-8
TIMEUPPERBOUND=  10
set title "FNAME2PREFIX= " . FNAME2PREFIX . "\nFNAMEPREFIX= " . FNAMEPREFIX noenhanced
set ylabel "QUality ImProvement/sec"
#set logscale y 10
set ytics border mirror
#set mytics 10-1
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
plot [RAMLOWERBOUND:RAMUPPERBOUND][QUIPSLOWERBOUND:QUIPSUPPERBOUND] \
 FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt" using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "orange" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_1_2.txt" using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_2.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_1_3.txt" using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_3.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_2_3.txt" using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_2_3.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_1_2_3.txt" using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_3.txt"   using 4:2 with linespoints linecolor rgb "light-turquoise" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_1.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_1.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_2_3.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_2_3.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_2.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_2.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_3.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_3.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_2.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_2.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_3.txt" using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_3.txt"   using 4:2 with linespoints linecolor rgb "light-pink" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0.txt"  using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0.txt"    using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_2.txt"  using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_2.txt"    using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_3.txt"  using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_3.txt"    using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAME2PREFIX . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "light-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_4_threads.txt" using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_4_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-orange" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_1_2.txt" using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_2.txt"   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_1_3.txt" using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_3.txt"   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_0_2_3.txt" using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_2_3.txt"   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_3_threads_creation_untimed-cpus_1_2_3.txt" using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_3_threads_creation_untimed-cpus_0_1_3.txt"   using 4:2 with linespoints linecolor rgb "dark-turquoise" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_1.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_1.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_2_3.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_2_3.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_2.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_2.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_0_3.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_0_3.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_2.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_2.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_2_threads_creation_untimed-cpus_1_3.txt" using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_2_threads_creation_untimed-cpus_1_3.txt"   using 4:2 with linespoints linecolor rgb "dark-pink" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_0.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_0.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_1.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_1.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_2.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_2.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads-cpus_3.txt"  using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads-cpus_3.txt"    using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ull-ISIZE_ull-using_1_threads.txt" using 4:2 with linespoints linecolor rgb "dark-magenta" \
,FNAMEPREFIX  . "-DSIZE_ul-ISIZE_ul-using_1_threads.txt"   using 4:2 with linespoints linecolor rgb "dark-magenta"
#    EOF
