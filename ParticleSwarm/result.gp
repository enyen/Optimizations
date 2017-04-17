#!/usr/bin/gnuplot --persist

set terminal wxt
set title "Performance of Parents" 
set xlabel "Iteration"
set ylabel "Fitness"
set autoscale
plot "./result.txt" using 1:2 title 'GroundTrue' with lines, \
	 "./result.txt" using 1:3 title 'Prediction' with lines
