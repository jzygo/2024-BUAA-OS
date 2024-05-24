#include <lib.h>
#include <malta.h>

int main() {
	debugf("devtst begin\n");
	int i = 0;
	int r;
	char buf[32] __attribute__((aligned(4))) = {0};
	char c __attribute__((aligned(4))) = 0;
	u_int cons_lsr = MALTA_SERIAL_LSR;
	u_int cons = MALTA_SERIAL_DATA;
	while (1) {
		if ((r = syscall_read_dev(&c, cons_lsr, 1)) != 0) {
			debugf("syscall_read_dev is bad\n");
		}
		if (c & MALTA_SERIAL_DATA_READY) {
			if ((r = syscall_read_dev(&c, cons, 1)) != 0) {
				debugf("syscall_read_dev is bad\n");
			}
		} else {
			c = 0;
		}
		if (c == '\r') {
			break;
		}
		if (c != 0) {
			buf[i++] = c;
		}
	}
	if (i == 14) {
		debugf("syscall_read_dev is good\n");
	}
	buf[i++] = '\n';
	for (int j = 0; j < i; j++) {
		int ch = buf[j];
		if ((r = syscall_write_dev(&ch, cons, 4)) != 0) {
			debugf("syscall_write_dev is bad\n");
		}
	}
	debugf("end of devtst\n");

	if (syscall_read_dev(&c, 0x0fffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x10000020, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x1000001f, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}

	if (syscall_read_dev(&c, 0x12ffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x13004200, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x130041ff, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}

	if (syscall_read_dev(&c, 0x14ffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x15000200, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_read_dev(&c, 0x150001ff, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}

	if (syscall_write_dev(&c, 0x0fffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x10000020, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x1000001f, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}

	if (syscall_write_dev(&c, 0x12ffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x13004200, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x130041ff, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}

	if (syscall_write_dev(&c, 0x14ffffff, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x15000200, 1) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, 0x150001ff, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_IDE_DEVICE, 4) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, MALTA_IDE_DEVICE + 0xf, 4) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, MALTA_SERIAL_BASE, 0x1f) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_SERIAL_BASE + 0x1f, 4) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}
	if (syscall_write_dev(&c, MALTA_SERIAL_BASE + 0x2f, 4) != -3) {
		user_panic("failed dev address test, maybe you can recheck your address validation "
			   "checking");
	}

	if (syscall_write_dev(&c, MALTA_IDE_BASE, 3) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_IDE_BASE, 5) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_IDE_BASE, 6) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_IDE_BASE, 7) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}
	if (syscall_write_dev(&c, MALTA_IDE_BASE, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_write_dev");
	}

	if (syscall_read_dev(&c, MALTA_IDE_BASE, 3) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}
	if (syscall_read_dev(&c, MALTA_IDE_BASE, 5) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}
	if (syscall_read_dev(&c, MALTA_IDE_BASE, 6) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}
	if (syscall_read_dev(&c, MALTA_IDE_BASE, 7) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}
	if (syscall_read_dev(&c, MALTA_IDE_BASE, 8) != -3) {
		user_panic(
		    "failed dev address test, maybe you should check the length in sys_read_dev");
	}

	debugf("dev address is ok\n");

	syscall_read_dev(&c, 0x10000010, 4);
	return 0;
}
