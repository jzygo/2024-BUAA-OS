#include <env.h>
#include <lib.h>
#include <mmu.h>

void exit(void) {
	// After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
	close_all();
#endif

	syscall_env_destroy(0);
	user_panic("unreachable code");
}

const volatile struct Env *env;
extern int main(int, char **);

volatile int straced;
int strace_recv_sysno[STRACE_MAX_RECV], strace_recv_child[STRACE_MAX_RECV];
int strace_recv_cnt;
void recv_sysno(u_int child, int sysno) {
	strace_recv_child[strace_recv_cnt] = child;
	strace_recv_sysno[strace_recv_cnt] = sysno;
	strace_recv_cnt++;
}

void libmain(int argc, char **argv) {
	// set env to point at our env structure in envs[].
	env = &envs[ENVX(syscall_getenvid())];
	straced = 0;

	// call user main routine
	main(argc, argv);

	// exit gracefully
	exit();
}
