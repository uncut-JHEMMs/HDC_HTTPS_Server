#!/bin/bash
#This runs everything else: sets up the dirs, starts monitoring, sends requests, cleans up
#####################DIR/FILE SETUP#######################################################
./InitInfo.sh
###########################STRART SERVER############################################
#start the server
./serverStart serverconfig.cfg &
SERVERPID=$(ps | awk '$4 == "serverStart"{print $1}')
##########################START MONITORS#############################################
./apmgather.sh &
APMGATHERPID=$(ps | awk '$4 == "apmgather.sh"{print $1}')
#########################LOAD SERVER#################################################
#wait for a bit for server to start
sleep 1s
#curl some packets at it
#note that when we do "> responses.txt", the response is actually what gets put in the file, the result of the GET request, NOT any of the ouput from  curl. In order to
#check that, we'd need to run awk on it or something?

#what we really want to do is pipe the responses into awk, then look at the status code and increment a counter for the response codes
COUNTER=0
while [ $COUNTER -le 100 ]
do
	COUNTER=$((COUNTER + 1))
	curl -k -i --digest --user myuser:mypass https://localhost:8080/digest >> response.txt
done
#curl -k -i --digest --user myuser:mypass https://localhost:8080/hello > responses.txt
#NOTE FOR LATER: We can check the output from grep by either using -c (check the number of matches, which can be 0), or by using $? to get the actual return value for grep (0 for at least one match, non-zero for no matches)
#can also just do "if OUTPUT=$(command | grep pattern); then..."

#kill and cleanup
kill $SERVERPID
kill $APMGATHERPID
#./cleanup.sh
#create the graphs
./createGraphs.sh
