#!/bin/sh
mkdir __test_3_2
cd __test_3_2
echo "main main main\nkekes\nhi friend how you are doing? lakad matatag! Normalin, normalin!" > main.txt
echo "Ok, friend, thats 143a class and there is no place for jokes" > kek2.txt
echo "Never gonna give you up
Never gonna let you down
Never gonna run around and desert you
Never gonna make you cry
Never gonna say goodbye
Never gonna tell a lie and hurt you" > somemain


ls | grep main | wc > __res

../out > __res2
sleep 1

#check result
result=`diff __res __res2`
if [[ -z $result ]]; then
	echo "Test 3_2 passed"
else
	echo "Output is different: $result"
	echo "Expected: $(cat __res)"
	echo "Got: $(cat __res2)"
fi

cd ..
rm -rf __test_3_2

