#ifndef _MACHINE_H
#define _MACHINE_H

// Memory segments (32-bit kernel mode addresses)
#define KUSEG 0x00000000U
#define KSEG0 0x80000000U
#define KSEG1 0xA0000000U
#define KSEG2 0xC0000000U

/*
 * QEMU MMIO address definitions.
 */
#define MALTA_PCIIO_BASE 0x18000000
#define MALTA_FPGA_BASE 0x1f000000

/*
 * 16550 Serial UART device definitions.
 */
#define MALTA_SERIAL_BASE (MALTA_PCIIO_BASE + 0x3f8)
#define MALTA_SERIAL_DATA (MALTA_SERIAL_BASE + 0x0)
#define MALTA_SERIAL_LSR (MALTA_SERIAL_BASE + 0x5)
#define MALTA_SERIAL_DATA_READY 0x1

/*
 * Intel PIIX4 IDE Controller device definitions.
 * Hardware documentation available at
 * https://www.intel.com/Assets/PDF/datasheet/290562.pdf
 */
#define MALTA_IDE_BASE (MALTA_PCIIO_BASE + 0x01f0)
#define MALTA_IDE_DATA (MALTA_IDE_BASE + 0x00)
#define MALTA_IDE_ERR (MALTA_IDE_BASE + 0x01)
#define MALTA_IDE_NSECT (MALTA_IDE_BASE + 0x02)
#define MALTA_IDE_LBAL (MALTA_IDE_BASE + 0x03)
#define MALTA_IDE_LBAM (MALTA_IDE_BASE + 0x04)
#define MALTA_IDE_LBAH (MALTA_IDE_BASE + 0x05)
#define MALTA_IDE_DEVICE (MALTA_IDE_BASE + 0x06)
#define MALTA_IDE_STATUS (MALTA_IDE_BASE + 0x07)
#define MALTA_IDE_LBA 0xE0
#define MALTA_IDE_BUSY 0x80
#define MALTA_IDE_CMD_PIO_READ 0x20  /* Read sectors with retry */
#define MALTA_IDE_CMD_PIO_WRITE 0x30 /* write sectors with retry */

/*
 * MALTA Power Management device definitions.
 */
#define MALTA_FPGA_HALT (MALTA_FPGA_BASE + 0x500)


/* IO function */
void m_putch(char ch);
char m_getch(void);
/* when call this func, gxemul will quit */
void m_halt(void);

#endif
