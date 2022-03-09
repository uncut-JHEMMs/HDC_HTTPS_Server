#!/bin/bash
#this just creates all the graphs from the data we got
#create the topcpu graph (cpu usage by the process)
gnuplot -e "set terminal png size 600,400; set output 'cpuUsage.png';
 set title 'CPU Usage By Server Process';
 set xlabel 'Server Running Time (s)';
 set ylabel 'CPU Percentage use by server process';
 plot 'PerformanceData/topcpu.txt' w lines"
