#ifndef _IO_H_
#define _IO_H_

#include <pmap.h>

/* Overview:
 *   Read a byte (8 bits) at the specified physical address `paddr`.
 *
 * Pre-Condition:
 *  `paddr` is a valid physical address.
 *
 * Parameters:
 *  paddr: The physical address to be read.
 *
 * Return the 8-bit value stored at `paddr`.
 *
 * Example:
 *  uint8_t data = ioread8(pa); // read a byte at physical address `pa`.
 */
static inline uint8_t ioread8(u_long paddr) {
	return *(volatile uint8_t *)(paddr | KSEG1);
}

/* Overview:
 *   Read half a word (16 bits) at the specified physical address `paddr`.
 *   Return the read result.
 */
static inline uint16_t ioread16(u_long paddr) {
	return *(volatile uint16_t *)(paddr | KSEG1);
}

/* Overview:
 *   Read a word (32 bits) at the specified physical address `paddr`.
 *   Return the read result.
 */
static inline uint32_t ioread32(u_long paddr) {
	return *(volatile uint32_t *)(paddr | KSEG1);
}

/* Overview:
 *   Write a byte (8 bits of data) to the specified physical address `paddr`.
 *
 * Pre-Condition:
 *  `paddr` is a valid physical address.
 *
 * Post-Condition:
 *   The byte at the `paddr` is overwritten by `data`.
 *
 * Parameters:
 *   data: The data to be written.
 *   paddr: The physical address to be written.
 *
 * Example:
 *   iowrite8(data, pa); // write `data` to physical address `pa`.
 */
static inline void iowrite8(uint8_t data, u_long paddr) {
	*(volatile uint8_t *)(paddr | KSEG1) = data;
}

/* Overview:
 *   Write half a word (16 bits of data) to the specified physical address `paddr`.
 */
static inline void iowrite16(uint16_t data, u_long paddr) {
	*(volatile uint16_t *)(paddr | KSEG1) = data;
}

/* Overview:
 *   Write a word (32 bits of data) to the specified physical address `paddr`.
 */
static inline void iowrite32(uint32_t data, u_long paddr) {
	*(volatile uint32_t *)(paddr | KSEG1) = data;
}

#endif
