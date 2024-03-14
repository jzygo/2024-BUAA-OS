#include <malta.h>
#include <mmu.h>
#include <printk.h>

/* Lab 1 Key Code "printcharc" */
/* Overview:
 *   Send a character to the console. If Transmitter Holding Register is currently occupied by
 *   some data, wait until the register becomes available.
 *
 * Pre-Condition:
 *   'ch' is the character to be sent.
 */
void printcharc(char ch) {
	if (ch == '\n') {
		printcharc('\r');
	}
	while (!(*((volatile uint8_t *)(KSEG1 + MALTA_SERIAL_LSR)) & MALTA_SERIAL_THR_EMPTY)) {
	}
	*((volatile uint8_t *)(KSEG1 + MALTA_SERIAL_DATA)) = ch;
}
/* End of Key Code "printcharc" */

/* Overview:
 *   Read a character from the console.
 *
 * Post-Condition:
 *   If the input character data is ready, read and return the character.
 *   Otherwise, i.e. there's no input data available, 0 is returned immediately.
 */
int scancharc(void) {
	if (*((volatile uint8_t *)(KSEG1 + MALTA_SERIAL_LSR)) & MALTA_SERIAL_DATA_READY) {
		return *((volatile uint8_t *)(KSEG1 + MALTA_SERIAL_DATA));
	}
	return 0;
}

/* Overview:
 *   Halt/Reset the whole system. Write the magic value GORESET(0x42) to SOFTRES register of the
 *   FPGA on the Malta board, initiating a board reset. In QEMU emulator, emulation will stop
 *   instead of rebooting with the parameter '-no-reboot'.
 *
 * Post-Condition:
 *   If the current device doesn't support such halt method, print a warning message and enter
 *   infinite loop.
 */
void halt(void) {
	*(volatile uint8_t *)(KSEG1 + MALTA_FPGA_HALT) = 0x42;
	printk("machine.c:\thalt is not supported in this machine!\n");
	while (1) {
	}
}
