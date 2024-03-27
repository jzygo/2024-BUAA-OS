void scanf_1_check() {
	printk("Running scanf_1_check\n");
	char arr[128];
	char arr2[128];
	int dec, hex;
	char ch,ch2,ch3,ch4;
	scanf("%c%c%c%s%s%c",  &ch,&ch2,&ch3,arr,arr2,&ch4);
	printk("%c%c%c%s%s%c",ch,ch2,ch3,arr,arr2,ch4);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	scanf_1_check();
	halt();
}
