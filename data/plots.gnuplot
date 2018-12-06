# TO RUN THIS SCRIPT:
# gnuplot -c plots.gnu NAME_OF_TXT  
# sst-rrt_rand_file sst-rrt_bang_file dirt_rand_file dirt_bang_file "(x_g, y_g, th_g)"

set encoding utf8
set key right center
# set terminal postscript enhanced eps monochrome
set terminal pngcairo mono

NAME=ARG1

OUTPUT_NAME=NAME.".png"
FILE_TXT=NAME.".txt"
# print FILE_TXT



set output OUTPUT_NAME
set title ""
set xlabel "Input packet rate"
set ylabel 'Output packet rate'
set y2label 'Average Queue'
set ytics nomirror
set y2tics
set xrange [1:50000]
# set yrange [1:100000]
set y2range [0:]

set xtics rotate # by 45 offset -5,-2

# stats FILE_TXT using 9 name "gral"
# show variables all
# set arrow from 0,0 to 0,gral_max nohead
# print gral_index_max # 1:(($2-last)/(first-last))
# FILE_ELEMENT="liar_test_1.txt"(7,11)
# print FILE_TXT using 7[gral_index_max]
# set yr [GPVAL_DATA_Y_MIN:GPVAL_DATA_Y_MAX]
plot FILE_TXT using 7:9 with points pt 2 title "Output", FILE_TXT using 7:10 with points pt 1 axes x1y2 title "Queue Avg"

set y2label 'Cycles'
set output NAME."_2.png"
plot FILE_TXT using 7:9 with points pt 2 title "Output", FILE_TXT using 7:3 with points pt 1 axes x1y2 title "User cycles"

# LIAR PLOT
# set y2range [0:100]
# set y2label '% time I/Os disable'
# set output NAME."_3.png"
# plot FILE_TXT using 7:9 with points pt 2 title "Output", FILE_TXT using 7:($11 * 100) with points pt 1 axes x1y2 title "% time"

set y2range [0:100]
set y2label '% useful polling'
set output NAME."_3.png"
plot FILE_TXT using 7:9 with points pt 2 title "Output", FILE_TXT using 7:($12 * 100) with points pt 1 axes x1y2 title "% polling"


