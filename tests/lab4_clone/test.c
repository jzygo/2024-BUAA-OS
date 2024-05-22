#include <lib.h>
#include <string.h>

int flag = 0;

char buf[30];

const char input[30] = "this is what father env write";

static void os_assert(int cond, const char *err) {
	if (!cond) {
		user_halt("%s\n", err);
	}
}

void check() {
	while (!flag) {
		syscall_yield();
	}
	int r = strcmp(input, buf);
	if (r == 0) {
		debugf("They share same memory!\n");
	} else {
		os_assert(0, "They cannot share same memory!!!\n");
	}
	user_halt("child env ended\n");
}

int main() {
	u_int child_stack = 0x7f3fd800;
	syscall_clone((void *)check, (void *)child_stack);
	strcpy(buf, input);
	flag = 1;
	return 0;
}
