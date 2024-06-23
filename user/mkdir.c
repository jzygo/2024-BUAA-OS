#include <lib.h>

void usage(void) {
	printf("usage: mkdir dir.\n");
	exit();
}

/*
mkdir <dir>：若目录已存在则输出 mkdir: cannot create directory '<dir>': File exists，若创建目录的父目录不存在则输出 mkdir: cannot create directory '<dir>': No such file or directory，否则正常创建目录。
mkdir -p <dir>：当使用 -p 选项时忽略错误，若目录已存在则直接退出，若创建目录的父目录不存在则递归创建目录。

*/
// realize strncpy without any other function
void strncpy(char *dst, char *src, int n) {
	for (int i = 0; i < n; i++) {
		dst[i] = src[i];
	}
}
#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()"

int main(int argc, char *argv[]) {
	int r;
	int rt = 0;
	struct Stat stat_buf;
	if (argc < 2) {
		usage();
	}
    if (argc == 2) {
		int r = fsipc_create(argv[1], 1);
		if (r == -E_FILE_EXISTS) {
			debugf("mkdir: cannot create directory '%s': File exists\n", argv[1]);
		}
		else if (r < 0) {
			debugf("mkdir: cannot create directory '%s': No such file or directory\n", argv[1]);
		}
    }
	// 将目录按照/拆解，循环创建
	else if (argc == 3 && strcmp(argv[1], "-p") == 0) {
		char *path = argv[2];
		char *p = path;
		while (*p == '/') {
			p++;
		}
		char dir[128];
		while (strchr(WHITESPACE SYMBOLS, *p)) {
			while (*p != '/' && *p != '\0') {
				p++;
			}
			strncpy(dir, path, p - path);
			dir[p - path] = '\0';
			int r = fsipc_create(dir, 1);
			p++;
		}
	}
	else {
		usage();
	}
    
	return rt;
}