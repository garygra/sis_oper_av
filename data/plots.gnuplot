# TO RUN THIS SCRIPT:
# gnuplot -c plots.gnu NAME_OF_TXT  
# sst-rrt_rand_file sst-rrt_bang_file dirt_rand_file dirt_bang_file "(x_g, y_g, th_g)"

set encoding utf8
set key left
set terminal postscript enhanced eps monochrome


NAME=ARG1

OUTPUT_NAME=NAME.".eps"
FILE_TXT=NAME.".txt"
# print FILE_TXT

set output OUTPUT_NAME
set title ""
set xlabel "Input"
set ylabel 'Output'
# set xrange [1:100000]
# set yrange [1:100000]

plot FILE_TXT using 7:9 with points title ""
	 # "live_lock_test_2.txt" using 7:9 with points title ""

# set output "mogul_1.eps"
# plot "mogul_test_1.txt" using 7:9 with points title ""


