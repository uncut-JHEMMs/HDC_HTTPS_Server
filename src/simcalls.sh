#!/bin/bash
#does simultaneous curl calls to simulate concurrent users

curl -s -k "https://localhost:8080/image?[1-1000]" &
pidlist="$!" 
#curl -s -k "https://localhost:8080/image?[1-1000]" &
#pidlist="$pidlist $!" 
#curl -s -k "https://localhost:8080/hello?[1-1000]" &
#pidlist="$pidlist $!" 
#curl -s -k "https://localhost:8080/hello?[1-1000]" &
#pidlist="$pidlist $!"
#curl -s -k "https://localhost:8080/hello?[1-1000]" &
#pidlist="$pidlist $!" 
FAIL=0
for job in $pidlist 
do
	echo $job
	wait $job || let "FAIL+=1"
done
#sleep 1
#curl -s -k "https://localhost:8080/hello?[1-100]" &
#pidlist="$!" 
#for job in $pidlist 
#do
#	echo $job
#	wait $job || let "FAIL+=1"
#done

if [ "$FAIL" == "0" ]; then
	echo "YAY!"
else
	echo "FAIL! ($FAIL)"
fi
