#!/bin/sh
#create test files
head -c 1000000 /dev/urandom > __lol.txt

#execute program
./out __lol.txt __lol.copy.txt

#check result
if [ ! -f __lol.copy.txt ]; then
    echo "File not found!"
    rm -f __lol.txt
    exit 0
fi
result=`diff __lol.txt __lol.copy.txt`
if [[ -z $result ]]; then
	echo "Test 1 passed"
else
	echo "Different files"
fi

#cleanup
rm -f __lol.txt
rm -f __lol.copy.txt