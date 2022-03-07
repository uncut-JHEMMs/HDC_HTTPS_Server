#!/bin/bash
#send a couple messages to the server to see if we can kill it

#start the server
./serverStart serverconfig.cfg &
#wait for a bit?
sleep 1s
#curl some packets at it
#note that when we do "> responses.txt", the response is actually what gets put in the file, the result of the GET request, NOT any of the ouput from  curl. In order to
#check that, we'd need to run awk on it or something?
curl -k -i --digest --user myuser:mypass https://localhost:8080/hello > responses.txt
curl -k -i --digest --user myuser:mypass https://localhost:8080/hello >> responses.txt

#NOTE FOR LATER: We can check the output from grep by either using -c (check the number of matches, which can be 0), or by using $? to get the actual return value for grep (0 for at least one match, non-zero for no matches)
#can also just do "if OUTPUT=$(command | grep pattern); then..."
#kill it
PID=$(ps | awk '$4 == "serverStart"{print $1}')
kill $PID
