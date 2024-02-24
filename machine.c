#include <blib_macros.h>
#include <machine.h>

void m_putch(char ch) {
	*((volatile char *)(KSEG1 + MALTA_SERIAL_DATA)) = ch;
}

char m_getch(void) {
	if (*((volatile char *)(KSEG1 + MALTA_SERIAL_LSR)) & MALTA_SERIAL_DATA_READY) {
		return *((volatile char *)(KSEG1 + MALTA_SERIAL_DATA));
	}
	return 0;
}

void m_halt(void) {
	*(char *)(KSEG1 + MALTA_FPGA_HALT) = 0x42;
	putstr("machine.c:\thalt is not supported in this machine!\n");
	while (1) {
		;
	}
}
