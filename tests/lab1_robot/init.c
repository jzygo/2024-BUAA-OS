void position_check(void) {
	printk("%s%P%s", "This is a testcase: ", 2, 5, "\n");
	printk("the position is %P, size = %d\n", 1, 9, 9 - 1);
}

void mips_init(u_int argc, char **argv, char **penv, u_int ram_low_size) {
	position_check();
	halt();
}
