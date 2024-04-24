// For register-only custom instructions.
#define _BUILD_INST_PATTERN_1(__op, __rs, __rt, __rd, __vs, __vt, __vd)                            \
	do {                                                                                       \
		asm volatile("move $" #__rs ", %1\n"                                               \
			     "move $" #__rt ", %2\n"                                               \
			     ".insn\n"                                                             \
			     ".word (" #__op " | (" #__rd " << 11) | (" #__rt " << 16) | (" #__rs  \
			     " << 21))\n"                                                          \
			     "move %0, $" #__rd "\n"                                               \
			     : "=r"(__vd)                                                          \
			     : "r"(__vs), "r"(__vt)                                                \
			     : "$" #__rs, "$" #__rt, "$" #__rd);                                   \
	} while (0)

// For memory-accessing custom instructions.
#define _BUILD_INST_PATTERN_2(__op, __rs, __rt, __rd, __vs, __vt, __vd, __vr)                      \
	do {                                                                                       \
		asm volatile("move $" #__rs ", %1\n"                                               \
			     "move $" #__rt ", %2\n"                                               \
			     "move $" #__rd ", %3\n"                                               \
			     ".insn\n"                                                             \
			     ".word (" #__op " | (" #__rd " << 11) | (" #__rt " << 16) | (" #__rs  \
			     " << 21))\n"                                                          \
			     "move %0, $" #__rd "\n"                                               \
			     : "=r"(__vr)                                                          \
			     : "r"(__vs), "r"(__vt), "r"(__vd)                                     \
			     : "$" #__rs, "$" #__rt, "$" #__rd, "memory");                         \
	} while (0)

#define PMAXUB(__rs, __rt, __rd, __vs, __vt, __vd)                                                 \
	_BUILD_INST_PATTERN_1(0x3f, __rs, __rt, __rd, __vs, __vt, __vd)

#define CAS(__rs, __rt, __rd, __vs, __vt, __vd)                                                    \
	_BUILD_INST_PATTERN_2(0x3e, __rs, __rt, __rd, &(__vs), __vt, __vd, __vd)

#define UNK(__rs, __rt, __rd, __vs, __vt, __vd)                                                    \
	_BUILD_INST_PATTERN_2(0x3d, __rs, __rt, __rd, __vs, __vt, __vd, __vd)

#define BIT(x) (1 << (x))

int main() {
	unsigned int ret = 0;
	unsigned int rs, rt, rd;

	// pmaxub
	rs = 0x12345678;
	rt = 0x87654321;
	rd = 0;
	PMAXUB(8, 9, 10, rs, rt, rd);
	ret |= (rd == 0x87655678) ? 0 : BIT(0);

	// cas
	rs = 3;
	rt = 3;
	rd = 5;
	CAS(18, 19, 20, rs, rt, rd);
	ret |= (rs == 5) ? 0 : BIT(1);
	ret |= (rd == 3) ? 0 : BIT(2);

	// unknown
	rs = 0x12345678;
	rt = 0x23456789;
	rd = 0x34567891;
	UNK(8, 9, 10, rs, rt, rd);
	ret |= (rs == 0x12345678) && (rt == 0x23456789) && (rd == 0x34567891) ? 0 : BIT(3);

	return ret;
}
