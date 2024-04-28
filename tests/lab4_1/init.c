void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	printk("init.c:\tmips_init() is called\n");
	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	env_init();

	envid2env_check();
	halt();
}
