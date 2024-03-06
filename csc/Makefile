codeDIR=code
all: fibo

fibo: code/fibo.o code/main.o
	gcc $(LDFLAGS) -o fibo code/fibo.o code/main.o

# 调用code目录下的Makefile来构建.o文件
code/fibo.o code/main.o:
	$(MAKE) -C code

clean:
	rm -rf code/*.o