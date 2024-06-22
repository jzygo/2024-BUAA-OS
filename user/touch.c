#include <lib.h>

void usage(void) {
	debugf("usage: touch file. Type in file which you want to create. Make sure that the path is exsisted.\n");
	exit();
}

int main(int argc, char *argv[]) {
	int fd;
	if (argc < 2) {
		usage();
	}
        debugf("adfad");
    if ((fd = open(argv[1], O_RDONLY)) >= 0) {
        close(fd);
    }
    else {        
        if((fd=open(argv[1], O_CREAT | O_WRONLY))<0) {
            debugf("touch: cannot touch '%s': No such file or directory",argv[1]);
        }
        else {
            close(fd);
        }
    }
	
	return 0;
}