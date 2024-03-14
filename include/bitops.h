#ifndef _SYS_BITOPS_H_
#define _SYS_BITOPS_H_

#define BITS_PER_LONG 32
#define BITS_PER_LONG_LONG 64

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define GENMASK_ULL(h, l) (((~0ULL) << (l)) & (~0ULL >> (BITS_PER_LONG_LONG - 1 - (h))))

#define LOG_1(n) (((n) >= 2) ? 1 : 0)
#define LOG_2(n) (((n) >= 1 << 2) ? (2 + LOG_1((n) >> 2)) : LOG_1(n))
#define LOG_4(n) (((n) >= 1 << 4) ? (4 + LOG_2((n) >> 4)) : LOG_2(n))
#define LOG_8(n) (((n) >= 1 << 8) ? (8 + LOG_4((n) >> 8)) : LOG_4(n))
#define LOG2(n) (((n) >= 1 << 16) ? (16 + LOG_8((n) >> 16)) : LOG_8(n))

#endif
