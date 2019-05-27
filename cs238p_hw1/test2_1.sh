#!/bin/sh
rm -rf y y.txt
rm -rf __res
touch y
touch y.txt
touch ls.txt
touch __res
./out
sleep 1

ls > __res

#check result
if [ ! -f y ] || [ ! -f y.txt ]; then
	rm -f __res
    echo "File not found!"
    exit 0
fi
result=`diff y __res`
if [[ -z $result ]]; then
	echo "Test 2_1 passed"
else
    result=`diff y.txt __res`
    if [[ -z $result ]]; then
	echo "Test 2_1 passed"
    else 
	result=`diff ls.txt __res`
	if [[ -z $result ]]; then
	    echo "Test 2_1 passed"
	else
	    echo "Output is different with ls > y"
	    echo "Expected: $(cat __res)"
	    echo "Got: $(cat y)"
	fi
    fi
fi

rm -f __res
