g++ findDupFiles.cpp
rm -rf tests
mkdir -p tests/1 tests/2 tests/
echo hello > tests/hello.txt
echo hello > tests/1/hello2.txt
echo hellp > tests/2/hello3.txt
cp tests/1/hello2.txt tests/hello4.txt
cp tests/2/hello3.txt tests/1/hello5.txt
cp tests/2/hello3.txt tests/hello6.txt
chmod -r tests/hello6.txt
echo world > tests/2/hello7.txt
mkdir tests/33/hello8.txt
find tests -name "hello*.txt" | ./a.out