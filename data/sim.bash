#!/bin/bash


# for i in `seq 1 256`;
# do
i=200
# while [ $i -gt 0 ]; do
for i in $(seq 0.1 0.1 5.0); do
	PACKET_MEANS="$i $i $i $i $i" 
	# echo $PACKET_MEANS
	# echo $i
	../build/livelock_sim --packet_arrival_mean="$PACKET_MEANS" --debug_level=0
	
	# i=$((i-1))
done
