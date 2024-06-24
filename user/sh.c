#include <args.h>
#include <lib.h>

#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()#`"

/* Overview:
 *   Parse the next token from the string at s.
 *
 * Post-Condition:
 *   Set '*p1' to the beginning of the token and '*p2' to just past the token.
 *   Return:
 *     - 0 if the end of string is reached.
 *     - '<' for < (stdin redirection).
 *     - '>' for > (stdout redirection).
 *     - '|' for | (pipe).
 *     - 'w' for a word (command, argument, or file name).
 *
 *   The buffer is modified to turn the spaces after words into zero bytes ('\0'), so that the
 *   returned token is a null-terminated string.
 */

char buf[1024];
char buf2[8192];
int waitNew=0;
int _gettoken(char *s, char **p1, char **p2) {
	*p1 = 0;
	*p2 = 0;
	if (s == 0) {
		return 0;
	}

	while (strchr(WHITESPACE, *s)) {
		*s++ = 0;
	}
	if (*s == 0) {
		return 0;
	}

	if (strchr(SYMBOLS, *s)) {
		int t = *s;
		*p1 = s;
		*s++ = 0;
		if (strchr(SYMBOLS, *s)) {
			t+=*s;
			if (t==124) {
				t=126;
			}
			*s++ = 0;
		}
		*p2 = s;
		return t;
	}
	if (strchr("\"", *s)) {
		*s=0;
		s++;
		*p1 = s;
		while (*s && *s != '\"') {
			s++;
		}
		*s++=0;
		*p2 = s;
		return 'w';
	}

	*p1 = s;
	while (*s && !strchr(WHITESPACE SYMBOLS, *s)) {
		s++;
	}
	*p2 = s;
	return 'w';
}

int gettoken(char *s, char **p1) {
	static int c, nc;
	static char *np1, *np2;

	if (s) {
		nc = _gettoken(s, &np1, &np2);
		return 0;
	}
	c = nc;
	*p1 = np1;
	nc = _gettoken(np2, &np1, &np2);
	return c;
}

#define MAXARGS 128

int tag=0;
int lazy=0;
int echoFlag=0;



int parsecmd(char **argv, int *rightpipe) {
	int argc = 0;
	while (1) {
		char *t;
		int fd, r;
		int c = gettoken(0, &t);
		int son;
		switch (c) {
		case 0:
			return argc;
		case 'w':
			if (argc >= MAXARGS) {
				debugf("too many arguments\n");
				exit();
			}
			argv[argc++] = t;
			break;
		case '`':
			if (echoFlag==0) {
				echoFlag=1;
				argc--;
			}
			else {
				echoFlag=0;
				return argc;
			}
			break;
		case '<':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: < not followed by word\n");
				exit();
			}
			// Open 't' for reading, dup it onto fd 0, and then close the original fd.
			// If the 'open' function encounters an error,
			// utilize 'debugf' to print relevant messages,
			// and subsequently terminate the process using 'exit'.
			/* Exercise 6.5: Your code here. (1/3) */
			fd = open(t, O_RDONLY);
			r=dup(fd, 0);
			close(fd);
			if (r < 0) {	
				user_panic("< redirection not implemented");
			}
			break;
		case '>':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: > not followed by word\n");
				exit();
			}
			// Open 't' for writing, create it if not exist and trunc it if exist, dup
			// it onto fd 1, and then close the original fd.
			// If the 'open' function encounters an error,
			// utilize 'debugf' to print relevant messages,
			// and subsequently terminate the process using 'exit'.
			/* Exercise 6.5: Your code here. (2/3) */
			fd = open(t, O_WRONLY);
			r=dup(fd, 1);
			close(fd);
			if (r < 0) {
				user_panic("> redirection not implemented");
			}
			break;
		case ';':
			son = fork();
			if(son==0) {
				return argc;
			} 
			else {
				if(*rightpipe == 0){
					dup(1, 0);
				} else if(*rightpipe == 1) {
					dup(0, 1);
				}
				wait(son);
				return parsecmd(argv, rightpipe);
			}
			break;
		case 126://>>
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: >> not followed by word\n");
				exit();
			}

			fd = open(t, O_WRONLY | O_CREAT);
			int n;
			while ((n = read(fd, buf, (long)sizeof buf)) > 0);
			struct Fd *f;
			f=(struct Fd *)INDEX2FD(fd);
			struct Filefd *filefd=(struct Filefd *)f;
			f->fd_offset=filefd->f_file.f_size;
			r=dup(fd, 1);
			close(fd);
			if (r < 0) {
				user_panic(">> redirection not implemented");
			}
			break;
		case '#' :
			// ignore the rest of the line
			return argc;
		case '|':;
			/*
			 * First, allocate a pipe.
			 * Then fork, set '*rightpipe' to the returned child envid or zero.
			 * The child runs the right side of the pipe:
			 * - dup the read end of the pipe onto 0
			 * - close the read end of the pipe
			 * - close the write end of the pipe
			 * - and 'return parsecmd(argv, rightpipe)' again, to parse the rest of the
			 *   command line.
			 * The parent runs the left side of the pipe:
			 * - dup the write end of the pipe onto 1
			 * - close the write end of the pipe
			 * - close the read end of the pipe
			 * - and 'return argc', to execute the left of the pipeline.
			 */
			int p[2];
			/* Exercise 6.5: Your code here. (3/3) */
			pipe(p);
			*rightpipe = fork();
			if (*rightpipe == 0) {
				r=dup(p[0], 0);
				close(p[0]);
				close(p[1]);
				return parsecmd(argv, rightpipe);
			}  else if (*rightpipe > 0) {
				r=dup(p[1], 1);
				close(p[1]);
				close(p[0]);
				return argc;
			}
			user_panic("| not implemented");
			break;
		case 248:
		// realize || and && in bash
			/* Exercise 6.5: Your code here. (3/3) */
			debugf("|| \n");
			son = fork();
			if (son == 0) {
				tag=1;
				return argc;
			}  else if (son > 0) {
				        //   int result=0;
				int result=ipc_recv(NULL,0,0);
				tag=0;
				if(*rightpipe == 0){
					dup(1, 0);
				} else if(*rightpipe == 1) {
					dup(0, 1);
				}
				wait(son);
				if(result==0) {
					// debugf("son=%d, || \n",son);
					lazy=-1;
				}
				else {
					lazy=0;
				}
				// debugf("son=%d,lazy=%d,tag=%d start exe. || \n",son,lazy,tag);
				return parsecmd(argv, rightpipe);
			}
			break;
		case 76:
		// realize || and && in bash
			/* Exercise 6.5: Your code here. (3/3) */
			son = fork();
			if (son == 0) {
				tag=1;
				return argc;
			}  else if (son > 0) {
				tag=0;
				// int result=0;
				int result=ipc_recv(NULL,0,0);
				if(*rightpipe == 0){
					dup(1, 0);
				} else if(*rightpipe == 1) {
					dup(0, 1);
				}
				wait(son);
				if(result!=0) {
					// debugf("son=%d, && \n",son);
					lazy=1;
				}
				else {
					lazy=0;
				}
					// debugf("son=%d,lazy=%d,tag=%d start exe. && \n",son,lazy,tag);
				return parsecmd(argv, rightpipe);
			}
			break;
		}

	}

	return argc;
}

char *strstr(const char *haystack, const char *needle) {
	if (*needle == '\0') {
		return (char *) haystack;
	}
	
	while (*haystack != '\0') {
		const char *h = haystack;
		const char *n = needle;
		
		while (*n != '\0' && *h == *n) {
			h++;
			n++;
		}
		
		if (*n == '\0') {
			return (char *) haystack;
		}
		
		haystack++;
	}
	
	return NULL;
}
char *strcat(char *dest, const char *src) {
	char *p = dest;
	while (*p) {
		p++;
	}
	while (*src) {
		*p++ = *src++;
	}
	*p = '\0';
	return dest;
}

char *history[21];
int top;
void runcmd(char *s) {
	gettoken(s, 0);

	char *argv[MAXARGS];
	int rightpipe = 0;
	int argc = parsecmd(argv, &rightpipe);
	if (argc == 0) {
		return;
	}
	// 创建一个新的字符串指针，用于存放argv[0]的内容。按值复制
	char p[128];
	if (strstr(argv[0], "history")!=NULL) {
		argc = 2;
		argv[0] = "cat.b";
		argv[1] = "/.mosh_history";
	}
	strcpy(p, argv[0]);
	if (strstr(argv[0], ".b") == NULL) {
		strcat(p, ".b");
	}
	argv[argc] = 0;
	if (lazy!=0) {
		debugf("lazy=%d,tag=%d, start exe. \n",lazy,tag);
		if (lazy==1) {
			if (tag==1) {
				ipc_send(syscall_get_parent(),1,NULL,0);
			}
		}
		else {
			if (tag==1) {
				ipc_send(syscall_get_parent(),0,NULL,0);
			}
		}
		lazy = 0;
		exit();
	}
	debugf("runcmd: %s,envid=%d\n", p,syscall_getenvid());
	int child = spawn(p, argv);
	debugf("parent: %d\n",syscall_get_parent());
	debugf("sender start=%d\n", child);
	debugf("reicever start=%d\n",syscall_getenvid());
	u_int caller;
	int res = ipc_recv(&caller,0,0);
	close_all();
	if (child >= 0) {
		if (tag==1) {
			ipc_send(syscall_get_parent(),res,NULL,0);
		}
		wait(child);
	} else {
		debugf("spawn %s: %d\n", argv[0], child);
	}
	if (rightpipe) {
		wait(rightpipe);
	}
	exit();
}

void readline(char *buf, u_int n) {
	int r;
	for (int i = 0; i < n; i++) {
		if ((r = read(0, buf + i, 1)) != 1) {
			if (r < 0) {
				debugf("read error: %d\n", r);
			}
			exit();
		}
		if (buf[i] == '\b' || buf[i] == 0x7f) {
			if (i > 0) {
				i -= 2;
			} else {
				i = -1;
			}
			if (buf[i] != '\b') {
				printf("\b");
			}
		}
		if (buf[i] == '\r' || buf[i] == '\n') {
			buf[i] = 0;
			return;
		}
	}
	debugf("line too long\n");
	while ((r = read(0, buf, 1)) == 1 && buf[0] != '\r' && buf[0] != '\n') {
		;
	}
	buf[0] = 0;
}


void usage(void) {
	printf("usage: sh [-ix] [script-file]\n");
	exit();
}

int main(int argc, char **argv) {
	int r;
	int interactive = iscons(0);
	int echocmds = 0;
	printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("::                                                         ::\n");
	printf("::                     MOS Shell 2024                      ::\n");
	printf("::                                                         ::\n");
	printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	ARGBEGIN {
	case 'i':
		interactive = 1;
		break;
	case 'x':
		echocmds = 1;
		break;
	default:
		usage();
	}
	ARGEND

	if (argc > 1) {
		usage();
	}
	if (argc == 1) {
		close(0);
		if ((r = open(argv[0], O_RDONLY)) < 0) {
			user_panic("open %s: %d", argv[0], r);
		}
		user_assert(r == 0);
	}
	int fdnum = fsipc_create("/.mosh_history",0);
	top = 0;
	for (;;) {
		if (interactive) {
			printf("\n$ ");
		}
		readline(buf, sizeof buf);
		// history[top] = 0;
		// strcat(history[top],buf);
		// top++;
		// 将history[top]的最后一个字符替换为\n
		// history[top][strlen(buf)] = '\n';
		int fd = open("/.mosh_history",O_RDWR);
		int n;
		read(fd, buf2, (long)sizeof buf2);
		write(fd,buf,strlen(buf));
		write(fd,"\n",1);
		close(fd);

		if (buf[0] == '#') {
			continue;
		}
		if (echocmds) {
			printf("# %s\n", buf);
		}
		if ((r = fork()) < 0) {
			user_panic("fork: %d", r);
		}
		if (r == 0) {
			runcmd(buf);
			exit();
		} else {
			wait(r);
		}
	}
	if (syscall_get_parent() != 8195) {
		ipc_send(syscall_get_parent(),0,NULL,0);
	}
	return 0;
}
