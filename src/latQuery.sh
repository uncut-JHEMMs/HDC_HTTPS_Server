#!/bin/bash
#sends the current unix time in milliseconds to the server
TIME=$(date +%s%N | cut -b1-13)
echo "sent timestamp is $TIME"
curl -k --digest --user myuser:mypass https://localhost:8080/latency?time=$(date +%s%N | cut -b1-13) > latResponse.txt

