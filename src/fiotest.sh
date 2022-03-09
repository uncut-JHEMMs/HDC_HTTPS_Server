#!/bin/bash
#this script runs fio and gets data from a simple test. This is NOT to be called from 
#inside the loop in apmgather, it is an intensive job and should only run once
FIOJOBFN=fiorandomread.io
PERFORMANCEDIR=PerformanceData
fio $FIOJOBFN > $PERFORMANCEDIR/fiooutput.txt

