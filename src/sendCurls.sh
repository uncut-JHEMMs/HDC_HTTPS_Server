#!/bin/bash
#this script sends a bunch of curl requests to the server. This has to be broken out becasue
#of massif and callgrind, which can't be run in the background
HOSTNAME=https://localhost:8080
ENDPOINT=hello
REQUESTS=1
OUTFILE=response.txt
echo "Sending $REQUESTS Requests"
COUNTER=0
echo " " > $OUTFILE
while [ $COUNTER -le $REQUESTS ]
do
	COUNTER=$((COUNTER + 1))
	xargs -I % -P 1000 curl -s -k -i https://localhost:8080/hello < <(printf '%s\n' {1..1000}) >> $OUTFILE
done
echo " " >> $OUTFILE 
