[global]
bs=4K
iodepth=256
direct=1
ioengine=libaio
group_reporting
time_based
runtime=5
numjobs=4
name=raw-randread
rw=randread
size=100m
							
[job1]
filename=device name
