#!/bin/sh
rm -rf __test_2
mkdir __test_2
cd __test_2

rm -rf y y.txt ls.txt
rm -rf __res
touch y
touch y.txt
touch ls.txt
touch __res

touch dota2
touch lol
touch overwatch

ls > __res

../out
sleep 1

#check result
if [ ! -f y ] || [ ! -f y.txt ]; then
    echo "File not found!"
    cd ..
	rm -rf __test_2
    exit 0
fi
echo "TEST $(cat y)"
result=`diff y __res`
if [[ -z $result ]]; then
    echo "Test 2_2 passed"
else
    result=`diff y.txt __res`
    if [[ -z $result ]]; then
	echo "Test 2_2 passed"
    else
	result=`diff ls.txt __res`
	if [[ -z $result ]]; then
	    echo "Test 2_2 passed"
	else
	    echo "Output is different with ls > y: '$result'"
	    echo "Expected: $(cat __res)"
	    echo "Got: $(cat y) $(cat y.txt) $(cat ls.txt)"
	fi
    fi
fi

cd ..
rm -rf __test_2
