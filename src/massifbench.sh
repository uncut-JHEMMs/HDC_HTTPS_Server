#!/bin/bash
#this script runs the server under massif, which can't be done in the background
MASSIFOUTFN=massif_raw
PRETTYMASSIFFN=prettymassif.txt
echo "Running server under massif for heap profile data"
valgrind --tool=massif --massif-out-file=$MASSIFOUTFN ./serverStart serverconfig.cfg 
mv $MASSIFOUTFN ValgrindData
ms_print ValgrindData/$MASSIFOUTFN > ValgrindData/$PRETTYMASSIFFN
