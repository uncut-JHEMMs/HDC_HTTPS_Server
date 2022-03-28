#!/bin/bash
#This runs everything else: sets up the dirs, starts monitoring, sends requests, cleans up
#####################DIR/FILE SETUP#######################################################
./InitInfo.sh
###########################STRART SERVER############################################
#start the server in uninstrumented mode
./serverStart serverconfig.cfg &
SERVERPID=$!
#wait for a bit for server to start
sleep 1s
##########################START MONITORS#############################################
./apmgather.sh &
APMGATHERPID=$!
#########################LOAD SERVER#################################################
REQUESTCOUNT=50
#curl some packets at it
#note that when we do "> responses.txt", the response is actually what gets put in the file, the result of the GET request, NOT any of the ouput from  curl. In order to
#check that, we'd need to run awk on it or something?
#what we really want to do is pipe the responses into awk, then look at the status code and increment a counter for the response codes
COUNTER=0
OKCOUNT=0
NOTFOUNDCOUNT=0
UNAUTHORIZEDCOUNT=0
OTHERCOUNT=0
echo "Running server in uninstrumented mode to gather APM data"
echo "Sending $REQUESTCOUNT Requests"
while [ $COUNTER -le $REQUESTCOUNT ]
do
	sleep 0.125
	res=$((COUNTER%4))
	if [ $res -eq 0 ]
	then
		./latQuery.sh
		echo 200 > response.txt
	elif [ $res -eq 1 ]
	then 
		curl -k -i https://localhost:8080/hello | awk 'NR == 1{print $2}' > response.txt
	elif [ $res -eq 2 ]
	then
		curl -k -i https://localhost:8080/digest | awk 'NR == 1{print $2}' > response.txt
	else
		curl -k -i https://localhost:8080/nothing | awk 'NR == 1{print $2}' > response.txt
	fi
	#get the response code using awk and store it in a variable
	RESPCODE=$(awk '{print $1}' response.txt)
	if [ $RESPCODE -eq 200 ]
	then 
		echo "saw 200"
		OKCOUNT=$((OKCOUNT+1))
	elif [ $RESPCODE -eq 401 ]
	then
		UNAUTHORIZEDCOUNT=$((UNAUTHORIZEDCOUNT + 1))
		echo "saw 401"
	elif [ $RESPCODE -eq 404 ]
	then
		NOTFOUNDCOUNT=$((NOTFOUNDCOUNT + 1))
		echo "saw 404"
	else
		echo "saw other code: $RESPCODE"
		OTHERCOUNT=$((OTHERCOUNT + 1))
	fi
	COUNTER=$((COUNTER + 1))
done
#write the response code variables to a file
RESPONSESFN=PerformanceData/responses.txt
#code 200
echo "0 200-OK $OKCOUNT" > $RESPONSESFN
#code 401
echo "1 401-Unauthorized $UNAUTHORIZEDCOUNT" >> $RESPONSESFN
#code 404
echo "2 404 $NOTFOUNDCOUNT" >> $RESPONSESFN
#others
echo "3 Other $OTHERCOUNT" >> $RESPONSESFN

#curl -k -i --digest --user myuser:mypass https://localhost:8080/hello > responses.txt
#NOTE FOR LATER: We can check the output from grep by either using -c (check the number of matches, which can be 0), or by using $? to get the actual return value for grep (0 for at least one match, non-zero for no matches)
#can also just do "if OUTPUT=$(command | grep pattern); then..."

#kill and cleanup
kill $SERVERPID
kill $APMGATHERPID

#./cleanup.sh
#create the graphs
./createGraphs.sh

#change the stuff in PerformanceInfo and InitInfo so everyone is not write-protected

exit 0
