extern void print_result(unsigned int, unsigned int, unsigned int);

static inline void pre_env_run(struct Env *e) {
	struct Trapframe *tf = (curenv == e) ? (struct Trapframe *)KSTACKTOP - 1 : &e->env_tf;
	u_int epc = tf->cp0_epc;
	if (epc == 0x400180) {
		u_int v0 = tf->regs[2];

		if ((v0 & 0x1) != 0) {
			printk("pmaxub: wrong value for $d\n");
		}
		if ((v0 & 0x2) != 0) {
			printk("cas: wrong value for $s\n");
		}
		if ((v0 & 0x4) != 0) {
			printk("cas: wrong value for $d\n");
		}
		if ((v0 & 0x8) != 0) {
			printk("Failed to handle unknown instruction");
		}

		print_result(e->env_id, epc, v0);
		env_destroy(e);
		schedule(0);
	}
}
