#!/bin/bash
#this just creates all the graphs from the data we got
#create the graph for one TPS
gnuplot -e "set terminal png size 600,400; set output 'oneTPSTenConnectsTenProcesses.png';
 set title 'Request Benchmark with TPS one';
 set key autotitle columnhead;
 unset key;
 set xlabel 'Server Running Time';
 set yrange [0:500];
 set ylabel 'Requests/Responses Per Second';
 plot 'oneTPSTenConnectsTenProcesses.txt' using 1 w lines" 

#two TPS
gnuplot -e "set terminal png size 600,400; set output 'twoTPSTenConnectsTenProcesses.png';
 set title 'Request Benchmark with TPS two';
 set key autotitle columnhead;
 unset key;
 set xlabel 'Server Running Time';
 set yrange [0:500];
 set ylabel 'Requests/Responses Per Second';
 plot 'twoTPSTenConnectsTenProcesses.txt' using 1 w lines"

#three TPS
gnuplot -e "set terminal png size 600,400; set output 'threeTPSTenConnectsTenProcesses.png';
 set title 'Request Benchmark with TPS three';
 set key autotitle columnhead;
 unset key;
 set xlabel 'Server Running Time';
 set yrange [0:500];
 set ylabel 'Requests/Responses Per Second';
 plot 'ThreeTPSTenConnectsTenProcesses.txt' using 1 w lines"

#four TPS
gnuplot -e "set terminal png size 600,400; set output 'FourTPSTenConnectsTenProcesses.png';
 set title 'Request Benchmark with TPS four';
 set key autotitle columnhead;
 unset key;
 set xlabel 'Server Running Time';
 set yrange [0:500];
 set ylabel 'Requests/Responses Per Second';
 plot 'FourTPSTenConnectsTenProcesses.txt' using 1 w lines"
