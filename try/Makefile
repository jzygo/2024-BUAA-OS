all: s1.o test.c
	mips-linux-gnu-gcc -o test test.c s1.o 

s1.o: s1.S
	mips-linux-gnu-gcc -o s1.o s1.S
