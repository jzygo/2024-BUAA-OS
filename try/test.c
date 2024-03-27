#include<stdio.h>

extern int add_function();

int main() {
    printf("calling MIPS");
    add_function(1,2);
    printf("Back in C code");
    return 0;
}