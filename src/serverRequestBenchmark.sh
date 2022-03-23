#!/bin/bash
#this script sends a bunch of curl requests to the server. This has to be broken out becasue
#of massif and callgrind, which can't be run in the background

#start upt the server in the background?
./serverStart serverconfig.cfg &
#get the pid of the server process, $! is the pid of the most recently executed
#background pipeline
SERVERPID=$!
#sleep for 1 second just in case the server needs time to warm up
sleep 1
HOSTNAME=https://localhost:8080
ENDPOINT=hello
OUTFILE=response.txt
PROCESSES=10
REQUESTS=1000
sleep 1
echo "Sending $REQUESTS Requests, $PROCESSES at a time"  > $OUTFILE
#1000 requests 10 at a time 
xargs -I % -P $PROCESSES curl -s -k -i https://localhost:8080/hello < <(printf '%s\n' {1..1000}) >> $OUTFILE
echo " " >> $OUTFILE
sleep 1
kill -s 2 $SERVERPID

#graph the data we got
./graphLatestStats.sh

#do a bunch of shit that results in a gnuplot graph of requests per second
