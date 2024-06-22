#include <lib.h>

void usage(void) {
	debugf("usage: touch file. Type in file which you want to create. Make sure that the path is exsisted.\n");
	exit();
}

/*
touch <file>：创建空文件 file，若文件存在则放弃创建，正常退出无输出。 若创建文件的父目录不存在则输出 touch: cannot touch '<file>': No such file or directory。 例如 touch nonexistent/dir/a.txt 时应输出 touch: cannot touch 'nonexistent/dir/a.txt': No such file or directory。
*/

int main(int argc, char *argv[]) {
	int fd;
	if (argc < 2) {
		usage();
	}
    int r = fsipc_create(argv[1], 0);
    if (r == -E_FILE_EXISTS || r >= 0) {
        return 0;
    }
    debugf("touch: cannot touch '%s': No such file or directory\n", argv[1]);
	return 0;
}