#!/usr/bin/gnuplot --persist

set terminal wxt
set title "Performance of Parents" 
set xlabel "Iteration"
set ylabel "Fitness"
set autoscale
plot "./performance.txt" using 1:2 title 'Training' with lines, \
	 "./performance.txt" using 1:3 title 'Testing' with lines
