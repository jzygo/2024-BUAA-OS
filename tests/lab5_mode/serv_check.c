#include <lib.h>

#define _die(FORMAT, ...)                                                                          \
	do {                                                                                       \
		debugf(FORMAT, __VA_ARGS__);                                                       \
		user_panic("ERROR");                                                               \
	} while (0)

#define _die_if_not(condition, FORMAT, ...)                                                        \
	do {                                                                                       \
		if (!(condition)) {                                                                \
			_die(FORMAT, __VA_ARGS__);                                                 \
		}                                                                                  \
	} while (0)

int main() {
	int r;
	int fdnum;
	struct Stat stat_buf;

	if ((r = open("/newmotd", O_RDWR)) < 0) {
		_die("Cannot open /newmotd: %d", r);
	} else {
		debugf("OK, can open /newmotd with O_RDWR\n");
		fdnum = r;
		if ((r = close(fdnum)) < 0) {
			_die("Close /newmotd failed: %d", r);
		}
	}

	if ((r = chmod("/newmotd", FMODE_W, 2)) < 0) {
		_die("Cannot remove write permission of /newmotd: %d", r);
	} else {
		debugf("Removed write permisson of /newmotd\n");
	}

	stat("/newmotd", &stat_buf);
	_die_if_not(stat_buf.st_mode == FMODE_R, "Wrong mode, expect %d but got %d", FMODE_R,
		    stat_buf.st_mode);

	if ((r = open("/newmotd", O_WRONLY)) >= 0) {
		_die("Bad, can still open /newmotd with O_WRONLY: %d\n", r);
	} else {
		debugf("OK, can not open /newmotd with O_WRONLY\n");
	}
	if ((r = open("/newmotd", O_RDWR)) >= 0) {
		_die("Shoud't open /newmotd: %d", r);
	} else {
		debugf("OK, can not open /newmotd with O_RDWR\n");
	}

	if ((r = chmod("/newmotd", FMODE_W, 0)) < 0) {
		_die("Cannot chmod /newmotd: %d", r);
	} else {
		debugf("Set /newmotd mode to write only\n");
	}

	if ((r = stat("/newmotd", &stat_buf)) >= 0) {
		_die("Bad, can still get stat of a write only file: %d", r);
	} else {
		debugf("OK, could not get stat of a write only file\n");
	}

	if ((r = open("/newmotd", O_RDONLY)) >= 0) {
		_die("Bad, can still open /newmotd with O_RDONLY: %d", r);
	} else {
		debugf("OK, can not open /newmotd with O_RDONLY\n");
	}

	if ((r = chmod("/newmotd", 0, 0)) < 0) {
		_die("Cannot chmod /newmotd: %d", r);
	} else {
		debugf("OK, cleared all permissions of /newmotd\n");
	}
	if ((r = chmod("/newmotd", 7, 0)) < 0) {
		_die("Cannot chmod /newmotd: %d", r);
	} else {
		debugf("OK, granted all permissions of /newmotd\n");
	}

	debugf("File mode test passed!\n");

	return 0;
}
