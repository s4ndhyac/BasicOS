#!/bin/sh
mkdir __test_3_1
cd __test_3_1
echo "main main main\nkekes\nhi friend how you are doing? lakad matatag! Normalin, normalin!" > main.txt


ls | grep main | wc > __res

../out > __res2
sleep 1

#check result
result=`diff __res __res2`
if [[ -z $result ]]; then
	echo "Test 3_1 passed"
else
	echo "Output is different"
	echo "Expected: $(cat __res)"
	echo "Got: $(cat __res2)"
fi


cd ..
rm -rf __test_3_1
