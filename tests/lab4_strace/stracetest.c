#include <lib.h>

int greedy_check(int num, int child_ans[], int sysno_ans[]) {
	int straced_back = straced;
	straced = 0;

	if (num > strace_recv_cnt) {
		debugf("Wrong number of traced syscall\n");
		syscall_panic("Error");
	}

	int dirty = 0;
	if (child_ans != NULL || sysno_ans != NULL) {
		int i = 0;
		int j = 0;
		while (i < num && j < strace_recv_cnt) {
			if (child_ans) {
				if (child_ans[i] != strace_recv_child[j]) {
					dirty = 1;
					j++;
					continue;
				}
			}
			if (sysno_ans) {
				if (sysno_ans[i] != strace_recv_sysno[j]) {
					dirty = 1;
					j++;
					continue;
				}
			}
			i++;
			j++;
		}
		if (i != num) {
			debugf("Wrong child or sysno\n");
			syscall_panic("Error");
		}
	}

	if (dirty) {
		debugf("DIRTY\n");
	}

	straced = straced_back;

	return dirty;
}

void print_answer() {
	int straced_back = straced;

	debugf("%5s |%7s |%6s\n", "No.", "envid", "sysno");
	for (int i = 0; i < strace_recv_cnt; i++) {
		debugf("%4d  | 0x%x |%4d\n", i + 1, strace_recv_child[i], strace_recv_sysno[i]);
	}

	straced = straced_back;
}

int main() {
	if (fork()) {
		strace_recv();
		int child_ans[3] = {0x1001, 0x1001, 0x1001};
		int sysno_ans[3] = {2, 1, 4};
		print_answer();
		greedy_check(3, child_ans, sysno_ans);
		debugf("strace test passed!\n");
	} else {
		straced = 1;
		debugf("Hi, this is 0x%x!\n", syscall_getenvid());
	}

	return 0;
}
