#!bin/sh

project_code=$1
if [ -z $project_code ]
then
	echo "Command line arguments missing"
	exit 1
fi

if [ -f result.txt ]
then
	rm -f result.txt
fi
if [ -f ref_result.txt ]
then
	rm -f ref_result.txt
fi
if [ -f difference.txt ]
then
	rm -f difference.txt
fi

if [ "$project_code" == "smp" ]
then
	./simulate_cache_ref $project_code $2 $3 $4 $5 $6 $7 $8 > ref_result.txt
	./simulate_cache $1 $2 $3 $4 $5 $6 $7 $8 > result.txt
	diff result.txt ref_result.txt > difference.txt
elif [ "$project_code" == "dsm" ]
then
        ./simulate_cache $project_code $2 $3 $4 $5 $6 $7 $8 $9 > result.txt
	./simulate_cache_ref $1 $2 $3 $4 $5 $6 $7 $8 $9 > ref_result.txt
	diff result.txt ref_result.txt > difference.txt
fi

cat difference.txt


