#!/bin/bash
#this script is responsible for grabbing the live apm data from free, top, etc.,
#and putting the right data into the right file so gnuplot can use it later
#this script needs the server to be running so it can get the PID for the server
#process; throws an error if it isn't running


#need to add the check that we're root up here so iotop works
#run the setup script so the directories exist
#here's all the filenames where data is going to go
APMFOLDER=PerformanceData
#Free outputs
FREEDATAFN=$APMFOLDER/freepulse.txt
FREEMEMFN=$APMFOLDER/freemem.txt
FREESWAPFN=$APMFOLDER/freeswap.txt
#top outputs
TOPDATAFN=$APMFOLDER/toppulse.txt
PERCCPUFN=$APMFOLDER/topcpu.txt #this is percent cpu/mem used by the server process
PERCMEMFN=$APMFOLDER/topmem.txt
#iotop outputs
IOTOPDATAFN=$APMFOLDER/iotoppulse.txt
DISKREADFN=$APMFOLDER/iotopread.txt
DISKWRITEFN=$APMFOLDER/iotopwrite.txt
DISKSWAPFN=$APMFOLDER/iotopswap.txt
PERCIOFN=$APMFOLDER/iotopperc.txt
#mpstat outputs
MPSTATDATAFN=$APMFOLDER/mpstatpulse.txt
MPSTATUSERFN=$APMFOLDER/mpstatusr.txt
MPSTATNICEFN=$APMFOLDER/mpstatnice.txt
MPSTATIDLEFN=$APMFOLDER/mpstatidle.txt
#vmstat outputs
VMSTATDATAFN=$APMFOLDER/vmstatpulse.txt
VMSTATMEMBUFFN=$APMFOLDER/vmstatmembuff.txt
VMSTATMEMCACHEFN=$APMFOLDER/vmstatmemcache.txt

#get the pid for the server
SERVERPID=$(ps | awk '$4 == "serverStart"{print $1}')
#echo $SERVERPID

#now that we have it, we can grab the data in an infinite loop
DATAPOINTS=0
while [ $DATAPOINTS -ge 0 ]
do
	sleep 0.5
	#free data
	free > $FREEDATAFN
	#put current memory numbers into their files
	awk 'NR==2{print $4}' < $FREEDATAFN >> $FREEMEMFN
	awk 'NR==3{print $4}' < $FREEDATAFN >> $FREESWAPFN
	#free | awk 'NR==2{print $4}' >> $FREEMEMFN
	#free | awk 'NR==3{print $4}' >> $FREESWAPFN
	
	#top data
	top -b -n 1 > $TOPDATAFN
	awk '$12 == "serverStart"{print $9}'  < $TOPDATAFN >> $PERCCPUFN
	awk '$12 == "serverStart"{print $10}' < $TOPDATAFN >> $PERCMEMFN
	#$top -b -n 1 | awk '$12 == "serverStart"{print $9}' >> $PERCCPUFN
	#top -b -n 1 | awk '$12 == "serverStart"{print $10}' >> $PERCMEMFN

	#iotop data
	iotop -b -n 1 -P > $IOTOPDATAFN
	awk '$12 =="./serverStart"{print $4}'   < $IOTOPDATAFN >> $DISKREADFN
	awk '$12 =="./serverStart"{print $6}'   < $IOTOPDATAFN >> $DISKWRITEFN
	awk '$12 =="./serverStart"{print $8}'   < $IOTOPDATAFN >> $DISKSWAPFN
	awk '$12 == "./serverStart"{print $10}' < $IOTOPDATAFN >> $PERCIOFN
	#iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $4}' >> $DISKREADFN
	#iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $6}' >> $DISKWRITEFN
	#iotop -b -n 1 -P | awk '$12 =="./serverStart"{print $8}' >> $DISKSWAPFN
	#iotop -b -n 1 -P | awk '$12 == "./serverStart"{print $10}' >> $PERCIOFN

	#fio data and iftop data are in initinfo, they only get run once

	#mpstat data
	mpstat > $MPSTATDATAFN
	awk 'NR==4{print $3}'  < $MPSTATDATAFN >> $MPSTATUSERFN
	awk 'NR==4{print $4}'  < $MPSTATDATAFN >> $MPSTATNICEFN
	awk 'NR==4{print $12}' < $MPSTATDATAFN >> $MPSTATIDLEFN

	#vmstat data
	vmstat > $VMSTATDATAFN
	awk  'NR==3{print $5}' < $VMSTATDATAFN >> $VMSTATMEMBUFFN
	awk  'NR==3{print $6}' < $VMSTATDATAFN >> $VMSTATMEMCACHEFN

done
exit