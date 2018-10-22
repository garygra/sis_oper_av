#!/bin/bash


# for i in `seq 1 256`;
# do
i=200
# while [ $i -gt 0 ]; do
for i in $(seq 0.01 0.01 1.0); do
	TOTAL_IO=1
	# z=`echo $i*4|bc -l`
	# z1=`echo $i*2|bc -l`
	# PACKET_MEANS="$z $z1" 
	PACKET_MEANS="$i" 
	printf "$i "
	../build/livelock_sim --total_io=$TOTAL_IO --packet_arrival_mean="$PACKET_MEANS" --debug_level=0 --algorithm=1
	
	# i=$((i-1))
done
