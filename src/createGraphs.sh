#!/bin/bash
#this just creates all the graphs from the data we got
#create the topcpu graph (cpu usage by the process)
GRAPHDIR=PerformanceData
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/cpuUsage.png';
 set title 'CPU Usage By Server Process';
 set xlabel 'Server Running Time (s)';
 set yrange [0:100];
 set ylabel 'CPU Percentage use by server process';
 plot 'PerformanceData/topcpu.txt' w lines" 

 #topmem data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/topMemUsage.png';
 set title 'Memory Usage By Server Process';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Memory Percentage use by server process';
 plot 'PerformanceData/topmem.txt' w lines" 

 #freemem data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/freeMemUsage.png';
 set title 'System Memory Usage from free';
 set xlabel 'Server Running Time (s)';
 set ylabel 'System memory usage(b)';
 plot 'PerformanceData/freemem.txt' w lines" 

 #freeswap data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/freeSwapUsage.png';
 set title 'System Swap Usage from free';
 set xlabel 'Server Running Time (s)';
 set ylabel 'System swap usage(b)';
 set yrange [0:1];
 plot 'PerformanceData/freeswap.txt' w lines" 

#iotop io perc data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/iotopPercUsage.png';
 set title 'Percent IO saturation from iotop';
 set xlabel 'Server Running Time (s)';
 set ylabel 'IO use (%)';
 plot 'PerformanceData/iotopperc.txt' w lines"

#iotop disk read data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/iotopReadUsage.png';
 set title 'Process Disk Read from iotop';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Disk Read (B/s)';
 set yrange [0:1000];
 plot 'PerformanceData/iotopread.txt' w lines"
 
#iotop disk write data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/iotopWriteUsage.png';
 set title 'Process Disk Write from iotop';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Disk Write (B/s)';
 set yrange [0:1000];
 plot 'PerformanceData/iotopwrite.txt' w lines"

#iotop disk swap data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/iotopSwapUsage.png';
 set title 'Process Disk Swapin from iotop';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Disk Swapin (%)';
 set yrange [0:100];
 plot 'PerformanceData/iotopswap.txt' w lines"

#mpstat idle data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/mpstatidleUsage.png';
 set title 'CPU Idle time by mpstat';
 set xlabel 'Server Running Time (s)';
 set ylabel 'CPU Idle(%)';
 set yrange [90:100];
 plot 'PerformanceData/mpstatidle.txt' w lines"

#mpstat nice data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/mpstatniceUsage.png';
 set title 'CPU nice from mpstat';
 set xlabel 'Server Running Time (s)';
 set ylabel 'CPU Nice(%)';
 plot 'PerformanceData/mpstatnice.txt' w lines"

#vmstatmembuff data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/vmstatMemBuffUsage.png';
 set title 'Mem buff usage from vmstat';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Amount of memory used as buffers(B)';
 plot 'PerformanceData/vmstatmembuff.txt' w lines"

#vmstatmemcache data
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/vmstatMemCacheUsage.png';
 set title 'Mem cache usage from vmstat';
 set xlabel 'Server Running Time (s)';
 set ylabel 'Amount of memory used as caches(B)';
 plot 'PerformanceData/vmstatmemcache.txt' w lines"

#count of response codes
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/responseCodes.png';
 set title 'Response Codes from server during benchmark';
 set xlabel 'Response Codes';
 set ylabel 'Frequency';
 set boxwidth 0.5;
 set style fill solid;
 plot 'PerformanceData/responses.txt' using 1:3:xtic(2) with boxes"

#latency
gnuplot -e "set terminal png size 600,400; set output '$GRAPHDIR/latency.png';
 set title 'End-to-end delay between client and server';
 set xlabel 'Server Running Time (s)';
 set ylabel 'End-to-end delay (ms)';
 set yrange [0:100];
 plot 'PerformanceData/latResponse.txt' w lines"

#make the images writable
chmod 766 $GRAPHDIR/*.png
