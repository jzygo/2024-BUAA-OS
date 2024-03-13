mkdir test
cp -r code test/
cat code/14.c
x=0
end=1
while [ $x -ne 16 ]
do
	gcc -c test/code/${x}.c -o test/code/${x}.o
	let x=x+1
done
gcc -o test/hello test/code/*.o
./test/hello 2>> ./test/err.txt
mv test/err.txt .
chmod 755 err.txt
if [ $# -eq 2 ];
then
	n=$(($1+$2))
	sed -n '$np' err.txt>&2
elif [ $# -eq 1 ];
then
	n=$(($1+$end))	
	sed -n '$np' err.txt>&2
else
	sed -n '2p' err.txt>&2
fi
