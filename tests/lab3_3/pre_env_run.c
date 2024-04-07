static inline void pre_env_run(struct Env *e) {
	static int count = 0, count_800 = 0, count_1001 = 0;
	if (count > 100) {
		printk("%4d: ticks exceeded the limit %d\n", count, 100);
		halt();
	}
	printk("%4d: %08x\n", count, e->env_id);
	count++;
	if (e->env_id == 0x800) {
		count_800++;
	} else if (e->env_id == 0x1001) {
		count_1001++;
	} else {
		printk("Not expected env_id %x\n", e->env_id);
		halt();
	}
	printk("env0 count: %d, env1 count: %d, ratio: %d\%\n", count_800, count_1001,
	       count_1001 * 100 / (count_800 == 0 ? 1 : count_800));
}
