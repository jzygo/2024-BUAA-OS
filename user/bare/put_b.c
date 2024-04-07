void _start() {
	for (unsigned i = 0;; ++i) {
		if ((i & ((1 << 16) - 1)) == 0) {
			// Requires process running in kernel mode to work
			// May not work in real machine: need to check lsr.
			*(volatile char *)0xb80003f8 = 'b';
			*(volatile char *)0xb80003f8 = ' ';
		}
	}
}
