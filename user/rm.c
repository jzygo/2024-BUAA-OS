#include <lib.h>

void usage(void) {
	printf("usage: mkdir dir.\n");
	exit();
}
/*
rm <file>：若文件存在则删除 <file>，否则输出 rm: cannot remove '<file>': No such file or directory。
rm <dir>：命令行输出: rm: cannot remove '<dir>': Is a directory。
rm -r <dir>|<file>：若文件或文件夹存在则删除，否则输出 rm: cannot remove '<dir>|<file>': No such file or directory。
rm -rf <dir>|<file>：如果对应文件或文件夹存在则删除，否则直接退出。
*/
int main(int argc, char *argv[]) {
	int r;
	int rt = 0;
	struct Stat stat_buf;
	if (argc < 2) {
		usage();
	}
    if (argc == 2) {
		int r = fsipc_rm(argv[1], 0);
		if (r== -E_NOT_DIR) {
			debugf("rm: cannot remove '%s': Is a directory\n", argv[1]);
		}
		else if (r<0) {
			debugf("rm: cannot remove '%s': No such file or directory\n", argv[1]);
		}
    }
	else if (argc == 3 && strcmp(argv[1], "-r") == 0) {
		int r = fsipc_rm(argv[2], 1);
		if (r<0) {
			debugf("rm: cannot remove '%s': No such file or directory\n", argv[2]);
		}
	}
	else if (argc == 3 && strcmp(argv[1], "-rf") == 0) {
		int r = fsipc_rm(argv[2], 2);
	}
	else {
		usage();
	}

	return rt;
}