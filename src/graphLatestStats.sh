#!/bin/bash
#this command graphs the data from the most recent run of the server (it graphs the serverStats.txt file)
#create the graph for one TPS
#need to let this take in the parameters of the run
TITLE=$(date +"%D %T")
gnuplot -e "set terminal png size 600,400; set output 'LatestStats.png';
 set title 'Request Benchmark from $TITLE';
 set key autotitle columnhead;
 unset key;
 set xlabel 'Server Running Time';
 set yrange [0:500];
 set ylabel 'Requests/Responses Per Second';
 plot 'serverStats.txt' using 1 w lines" 
