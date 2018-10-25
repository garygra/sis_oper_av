#!/bin/bash
### ARG1 = algorithm type: 0 => round robin, 1 => mogul

# for i in `seq 1 256`;
# do
i=200
# while [ $i -gt 0 ]; do
for i in $(seq 0.001 0.005 .3); do   # for algorithm 0
# for i in $(seq 0.001 0.0005 .05); do # FOR algorithm 1
	TOTAL_IO=5
	# z=`echo $i*4|bc -l`
	# z1=`echo $i*2|bc -l`
	# PACKET_MEANS="$z $z1"
	PACKET_MEANS=""
	for j in $(seq 1 1 $TOTAL_IO); do
		z=`echo "scale=5;($i/$j)" | bc -l`
		PACKET_MEANS="$PACKET_MEANS $z"
	done 
	echo $PACKET_MEANS
	# PACKET_MEANS="$i" 
	printf "$i "
	../build/livelock_sim --total_io=$TOTAL_IO --packet_arrival_mean="$PACKET_MEANS" --debug_level=0 --algorithm=$1 --quota=$2
	
	# i=$((i-1))
done
