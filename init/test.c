#include<asm/asm.h>
#include<mmu.h>
#include<printk.h>

int main() {
	int a=1;
	int b=2;
	int c= testMIPS(a,b);
	printk("%d\n",c);
	return 0;
}
