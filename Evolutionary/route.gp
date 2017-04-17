#!/usr/bin/gnuplot --persist
	 
set terminal wxt
set title "Final Route" 
set xlabel "x"
set ylabel "y"
set autoscale
plot "./route.txt" title 'route' with lines
