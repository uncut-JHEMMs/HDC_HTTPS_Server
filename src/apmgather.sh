#!/bin/bash
#this script is responsible for grabbing the live apm data from free, top, etc.,
#and putting the right data into the right file so gnuplot can use it later
#this script needs the server to be running so it can get the PID for the server
#process; throws an error if it isn't running


#need to add the check that we're root up here so iotop works
#run the setup script so the directories exist
#./InitInfo.sh
#here's all the filenames where data is going to go
APMFOLDER=PerformanceData
FREEMEMFN=$APMFOLDER/freemem.txt
#USEDMEMFN=$APMFOLDER/usedmem.txt
FREESWAPFN=$APMFOLDER/freeswap.txt
#USEDSWAPFN=$APMFOLDER/usedswap.txt
PERCCPUFN=$APMFOLDER/topcpu.txt #this is percent cpu/mem used by the server process
PERCMEMFN=$APMFOLDER/topmem.txt
DISKREADFN=$APMFOLDER/iotopread.txt
DISKWRITEFN=$APMFOLDER/iotopwrite.txt
DISKSWAPFN=$APMFOLDER/iotopswap.txt
PERCIOFN=$APMFOLDER/iotopperc.txt
#get the pid for the server
SERVERPID=$(ps | awk '$4 == "serverStart"{print $1}')
#echo $SERVERPID

#now that we have it, we can grab the data in an infinite loop
DATAPOINTS=0
while [ $DATAPOINTS -ge 0 ]
do
	sleep 0.25
	#put current memory numbers into their files
	#free | awk 'NR==2{print $3}' >> $USEDMEMFN
	free | awk 'NR==2{print $4}' >> $FREEMEMFN
	#free | awk 'NR==3{print $3}' >> $USEDSWAPFN
	free | awk 'NR==3{print $4}' >> $FREESWAPFN
	
	#put the top data in its file
	top -b -n 1 | awk '$12 == "serverStart"{print $9}' >> $PERCCPUFN
	top -b -n 1 | awk '$12 == "serverStart"{print $10}' >> $PERCMEMFN

	#put iotop data in its file
	iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $4}' >> $DISKREADFN
	iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $6}' >> $DISKWRITEFN
	iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $8}' >> $DISKSWAPFN
	iotop -b -n 1 -P | awk '$12 == "./serverStart"{print $10}' >> $PERCIOFN 
done
