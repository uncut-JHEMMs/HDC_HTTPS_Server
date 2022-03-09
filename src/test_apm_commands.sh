#!/bin/bash
#run through the apm scripts in a valid ordering and make sure that they're returning expected values

#unit tests for all of the individual commands we use

#top
echo q | top 2> /dev/null

if [ ! $? ]
then
    echo "top failed"
else
    echo "top succeeded"
fi

#iftop
iftop 2> /dev/null 1>& 2
if [ ! $? ]
then 
    echo "iftop failed"
else
    echo "iftop succeeded"
fi

#vmstat
vmstat 2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "vmstat failed"
else
    echo "vmstat succeeded"
fi


#mpstat
mpstat 2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "mpstat failed"
else
    echo "mpstat succeeded"
fi

#init info
./InitInfo.sh 2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "Initinfo failed"
else   
    echo "Initinfo success"
fi


#benchmark
echo "Running benchmark, please be patient..."
./benchmark.sh 2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "benchmark failed"
else   
    echo "benchmark success"
fi

exit 0
#massifbench
echo "Running massif, ctrl-c after "
./massifbench.sh  2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "Massifbench failed"
else   
    echo "Massifbench success"
fi

#send curl
./sendCurls.sh  2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "sendCurls failed"
else   
    echo "sendCurls success"
fi

#callgrindbench
./callgrindbench.sh  2> /dev/null 1>& 2
if [ ! $? ]
then
    echo "Callgrindbench failed"
else   
    echo "Callgrindbench success"
fi
exit 0
#create graphs
./createGraphs.sh
if [ ! $? ]
then
    echo "CreateGraphs failed"
else   
    echo "CreateGraphs success"
fi

#latTest is part of all the benchmarks

#fiotest
./fiotest.sh
if [ ! $? ]
then
    echo "fiotest failed"
else   
    echo "fiotest success"
fi
