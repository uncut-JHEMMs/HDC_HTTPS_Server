#!/bin/bash
#sends the current unix time in milliseconds to the server
OUTFILE=PerformanceData/latResponse.txt
TIME=$(date +%s%N | cut -b1-13)
#echo "sent timestamp is $TIME"
curl -k https://localhost:8080/latency?time=$(date +%s%N | cut -b1-13) >> $OUTFILE 
echo  "" >> $OUTFILE

