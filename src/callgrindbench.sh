#!/bin/bash
#this script runs the server under callgrind, which can't be done in the background
CALLGRINDOUTFN=callgrind_raw
PRETTYCGFN=prettycallgrind.txt
echo "Running server under callgrind for heap profile data"
valgrind --tool=callgrind --callgrind-out-file=$CALLGRINDOUTFN ./serverStart serverconfig.cfg 
mv $CALLGRINDOUTFN ValgrindData
callgrind_annotate --tree=both ValgrindData/$CALLGRINDOUTFN > ValgrindData/$PRETTYCGFN
