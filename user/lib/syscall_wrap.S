#include <asm/asm.h>

LEAF(msyscall)
	// Just use 'syscall' instruction and return.

	/* Exercise 4.1: Your code here. */
	addiu   sp, sp, -24
	sw      a0, 4(sp)
	sw      a1, 8(sp)
	sw      a2, 12(sp)
	sw      a3, 16(sp)
	sw      ra, 20(sp)
	jal     strace_send
	lw      a0, 4(sp)
	lw      a1, 8(sp)
	lw      a2, 12(sp)
	lw      a3, 16(sp)
	lw      ra, 20(sp)
	addiu   sp, sp, 24
	syscall
	jr ra
END(msyscall)
