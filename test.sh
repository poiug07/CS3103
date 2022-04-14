rm exec_time

for i in {1..5}
	do
		STARTTIME=$(date +%s%N)
		make run
		ENDTIME=$(date +%s%N)
	#	echo "It takes $(($ENDTIME - $STARTTIME)) nanoseconds to complete this task..."
		echo "$(($ENDTIME - $STARTTIME))" >> exec_time
	done