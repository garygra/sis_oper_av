# TO RUN THIS SCRIPT:
# gnuplot -c plots.gnu dir_to_save 
# sst-rrt_rand_file sst-rrt_bang_file dirt_rand_file dirt_bang_file "(x_g, y_g, th_g)"

set encoding utf8
set key left
set terminal postscript enhanced eps monochrome

set output "receive_livelock.eps"
set title ""
set xlabel "Input"
set ylabel 'Output'
# set xrange [1:100000]
# set yrange [1:100000]

plot "live_lock_test_1.txt" using 7:9 with points title ""
	 # "live_lock_test_2.txt" using 7:9 with points title ""

# set output "receive_livelock_2.eps"
# plot "live_lock_test_2.txt" using 7:9 with points title ""


