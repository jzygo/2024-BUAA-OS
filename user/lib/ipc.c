// User-level IPC library routines

#include <env.h>
#include <lib.h>
#include <mmu.h>

// Send val to whom.  This function keeps trying until
// it succeeds.  It should panic() on any error other than
// -E_IPC_NOT_RECV.
//
// Hint: use syscall_yield() to be CPU-friendly.
void ipc_send(u_int whom, u_int val, const void *srcva, u_int perm) {
	int r;
	while ((r = syscall_ipc_try_send(whom, val, srcva, perm)) == -E_IPC_NOT_RECV) {
		syscall_yield();
	}
	user_assert(r == 0);
}

// Receive a value.  Return the value and store the caller's envid
// in *whom.
//
// Hint: use env to discover the value and who sent it.
u_int ipc_recv(u_int *whom, void *dstva, u_int *perm) {
	int r = syscall_ipc_recv(dstva);
	if (r != 0) {
		user_panic("syscall_ipc_recv err: %d", r);
	}

	if (whom) {
		*whom = env->env_ipc_from;
	}

	if (perm) {
		*perm = env->env_ipc_perm;
	}

	return env->env_ipc_value;
}

void sem_open(int sem_id, int n) {
	syscall_sem_open(sem_id, n);
}

int sem_wait(int sem_id) {
	int r;
	// Lab 4-1-Exam: Your code here. (1/9)
	// Implement process blocking
	while(r=syscall_sem_wait(sem_id)>0) {
		syscall_yield();
	}
	return r;
}

int sem_post(int sem_id) {
	return syscall_sem_post(sem_id);
}

int sem_kill(int sem_id) {
	return syscall_sem_kill(sem_id);
}