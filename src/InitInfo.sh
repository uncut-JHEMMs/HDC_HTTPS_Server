#!/bin/bash
#sets up the directory structure for APM, adds the init info files
INITINFODIR=InitInfo
VALGRINDDIR=ValgrindData
PERFDIR=PerformanceData

CPUIFILENAME=cpuinfo.txt
MEMIFILENAME=meminfo.txt

FREEFILENAME=free.txt
TOPFILENAME=topdata.txt
#make the directories
echo "creating directories..."
mkdir $INITINFODIR
mkdir $VALGRINDDIR
mkdir $PERFDIR

#put the initial info files in place
echo "adding init info..."
cd $INITINFODIR
cat /proc/cpuinfo > $CPUIFILENAME
cat /proc/meminfo > $MEMIFILENAME
cd ..
exit




