void print_result(unsigned int env_id, unsigned int pc, unsigned int v0) {
	printk("env %08x reached end PC: 0x%08x, $v0 = 0x%08x\n", env_id, pc, v0);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	mips_detect_memory(ram_low_size);
	mips_vm_init();
	page_init();
	env_init();

	ENV_CREATE_PRIORITY(test_ri, 1);

	schedule(0);
}
