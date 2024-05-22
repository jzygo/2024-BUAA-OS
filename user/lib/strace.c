#include <lib.h>

void strace_barrier(u_int env_id) {
	int straced_bak = straced;
	straced = 0;
	while (envs[ENVX(env_id)].env_status == ENV_RUNNABLE) {
		syscall_yield();
	}
	straced = straced_bak;
}

void strace_send(int sysno) {
	if (!((SYS_putchar <= sysno && sysno <= SYS_set_tlb_mod_entry) ||
	      (SYS_exofork <= sysno && sysno <= SYS_panic)) ||
	    sysno == SYS_set_trapframe) {
		return;
	}

	// Your code here. (1/2)
	if (straced==0) {
		return;
	}
	int temp = straced;
	straced = 0;
	ipc_send((envs + ENVX(syscall_getenvid()))->env_parent_id,sysno,0,0);
	syscall_set_env_status(0,ENV_NOT_RUNNABLE);
	straced = temp;
	schedule(0);
}

void strace_recv() {
	// Your code here. (2/2)
	u_int whom,perm;
	int result;
	while((result = ipc_recv(&whom, 0, 0))!=SYS_env_destroy) {
		strace_barrier(whom);
		recv_sysno(whom, result);
		syscall_set_env_status(whom,ENV_NOT_RUNNABLE);
	}
}
//make test lab=4_strace && make dbg