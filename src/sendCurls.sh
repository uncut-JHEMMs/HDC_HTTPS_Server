#!/bin/bash
#this script sends a bunch of curl requests to the server. This has to be broken out becasue
#of massif and callgrind, which can't be run in the background
HOSTNAME=https://localhost:8080
ENDPOINT=hello
REQUESTS=50
OUTFILE=response.txt
echo "Sending $REQUESTS Requests"
COUNTER=0
while [ $COUNTER -le $REQUESTS ]
do
	COUNTER=$((COUNTER + 1))
	curl -k -i --digest --user myuser:mypass $HOSTNAME/$ENDPOINT > $OUTFILE &
done
