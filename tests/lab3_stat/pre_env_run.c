static inline void pre_env_run(struct Env *e) {
	static int count = 0;
	if (count > MOS_SCHED_MAX_TICKS) {
		printk("ticks exceeded the limit %d\n", MOS_SCHED_MAX_TICKS);
		halt();
	}
	u_int pri, scheds, runs, clocks;
	env_stat(e, &pri, &scheds, &runs, &clocks);
	printk("%4d: env id %08x, pri %u, scheds %u time(s), runs %u time(s)\n", count, e->env_id,
	       pri, scheds, runs);
	printk("      total %u clock(s)\n", clocks);
	count++;
}
