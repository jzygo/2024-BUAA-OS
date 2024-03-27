#include <print.h>
#include <printk.h>
#include <trap.h>

/* Lab 1 Key Code "outputk" */
void outputk(void *data, const char *buf, size_t len) {
	for (int i = 0; i < len; i++) {
		printcharc(buf[i]);
	}
}
/* End of Key Code "outputk" */

/* Lab 1 Key Code "printk" */
void printk(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vprintfmt(outputk, NULL, fmt, ap);
	va_end(ap);
}
/* End of Key Code "printk" */

void print_tf(struct Trapframe *tf) {
	for (int i = 0; i < sizeof(tf->regs) / sizeof(tf->regs[0]); i++) {
		printk("$%2d = %08x\n", i, tf->regs[i]);
	}
	printk("HI  = %08x\n", tf->hi);
	printk("LO  = %08x\n\n", tf->lo);
	printk("CP0.SR    = %08x\n", tf->cp0_status);
	printk("CP0.BadV  = %08x\n", tf->cp0_badvaddr);
	printk("CP0.Cause = %08x\n", tf->cp0_cause);
	printk("CP0.EPC   = %08x\n", tf->cp0_epc);
}
void inputk(void *data, char *buf, size_t len) {
	for (int i = 0; i < len; i++) {
		while ((buf[i] = scancharc()) == '\0') {
		}
		if (buf[i] == '\r') {
			buf[i] = '\n';
		}
	}
}

int scanf(const char *fmt, ...) {
	// Lab 1-Extra: Your code here. (1/5)
	va_list ap;
	va_start(ap, fmt);
	vscanfmt(inputk, NULL, fmt, ap);
	va_end(ap);
}
