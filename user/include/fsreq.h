#ifndef _FSREQ_H_
#define _FSREQ_H_

#include <fs.h>
#include <types.h>

// Definitions for requests from clients to file system

enum {
	FSREQ_OPEN,
	FSREQ_MAP,
	FSREQ_SET_SIZE,
	FSREQ_CLOSE,
	FSREQ_DIRTY,
	FSREQ_REMOVE,
	FSREQ_SYNC,
	FSREQ_CHMOD,
	MAX_FSREQNO,
};
struct Fsreq_chmod {
    char req_path[MAXPATHLEN];
    u_int req_mode;
    int req_type;
};
struct Fsreq_open {
	char req_path[MAXPATHLEN];
	u_int req_omode;
};

struct Fsreq_map {
	int req_fileid;
	u_int req_offset;
};

struct Fsreq_set_size {
	int req_fileid;
	u_int req_size;
};

struct Fsreq_close {
	int req_fileid;
};

struct Fsreq_dirty {
	int req_fileid;
	u_int req_offset;
};

struct Fsreq_remove {
	char req_path[MAXPATHLEN];
};

#endif
