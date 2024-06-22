#include <lib.h>

void usage(void) {
	printf("usage: mkdir dir.\n");
	exit();
}

int main(int argc, char *argv[]) {
	int r;
	int rt = 0;
	struct Stat stat_buf;
	if (argc < 2) {
		usage();
	}
    if (argc == 2) {

    }

	return rt;
}