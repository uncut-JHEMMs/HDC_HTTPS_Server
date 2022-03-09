#!/bin/bash
#sets up the directory structure for APM, adds the init info files
INITINFODIR=InitInfo
VALGRINDDIR=ValgrindData
PERFDIR=PerformanceData

CPUIFILENAME=cpuinfo.txt
MEMIFILENAME=meminfo.txt

IFTOPFN=iftop.txt
#fio outputs, this doesn't go in apmgather
FIOJOBFILE=fiorandomread.io
FIOOUTFN=$INITINFODIR/fiooutput.txt

#put the initial info files in place
echo "adding init info..."
echo "running fio job, this takes a couple seconds..."
fio $FIOJOBFILE > $FIOOUTFN
cd $INITINFODIR
cat /proc/cpuinfo > $CPUIFILENAME
cat /proc/meminfo > $MEMIFILENAME
#add the server configuration as well
cp ../serverconfig.cfg .
cd ..
exit 0




