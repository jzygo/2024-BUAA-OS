void scanf_1_check() {
	printk("Running scanf_1_check\n");
	char arr[128];
	int dec, hex;
	char ch;
	scanf("%d",  &dec);
	printk("%d",dec);
	scanf("%x",&hex);
	printk("%x",hex);
	scanf("%c",&ch);
	printk("%c",ch);
	scanf("%s",arr);
	printk("%s",arr);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	scanf_1_check();
	halt();
}
