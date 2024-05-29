#include <lib.h>

#define os_assert(cond, format_str, ...)                                                           \
	do {                                                                                       \
		if (!(cond)) {                                                                     \
			user_halt("ERROR: " format_str "\n", __VA_ARGS__);                         \
		}                                                                                  \
	} while (0)
#define os_assert0(cond, format_str)                                                               \
	do {                                                                                       \
		if (!(cond)) {                                                                     \
			user_halt("ERROR: " format_str "\n");                                      \
		}                                                                                  \
	} while (0)

static char *path_src = "/bin";
static char *file_src = "/bin/rr/test copy.rs";
static char *path_dst = "/target";
static char *file_dst = "/target/rr/test copy.rs";

void test_copy_read(char *copy_src, char *copy_dst, char *src_file, char *dst_file, char *str) {
	char buf1[8192], buf2[8192];
	int r = open(src_file, O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", src_file, r);
	read(r, buf1, 8192);
	close(r);

	r = copy(copy_src, copy_dst);
	debugf("copy return value: %d\n", r);

	r = open(dst_file, O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", dst_file, r);
	read(r, buf2, 8192);
	os_assert(strcmp(buf1, buf2) == 0, "copy failed, buf1 = %s, buf2 = %s", buf1, buf2);
	int len = strlen(str);
	write(r, str, len);
	close(r);
	r = open(src_file, O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", src_file, r);
	read(r, buf2, 8192);
	os_assert0(strcmp(buf1, buf2) == 0, "write to dst result in src change");
	debugf("OSTEST_OK\n");
r = open("/bin/rr/test.rs", O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", src_file, r);
	read(r, buf1, 8192);
	close(r);

	r = open("/target/rr/test.rs", O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", dst_file, r);
	read(r, buf2, 8192);
	os_assert(strcmp(buf1, buf2) == 0, "copy failed, buf1 = %s, buf2 = %s", buf1, buf2);
	len = strlen(str);
	write(r, str, len);
	close(r);
	r = open(src_file, O_RDWR);
	os_assert(r >= 0, "failed to open %s, return value: %d", src_file, r);
	read(r, buf2, 8192);
	os_assert0(strcmp(buf1, buf2) == 0, "write to dst result in src change");
	debugf("OSTEST_OK\n");
}

int main() {
	test_copy_read(path_src, path_dst, file_src, file_dst, "hello world");
	return 0;
}
