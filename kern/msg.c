#include <env.h>

struct Msg msgs[NMSG];
struct Msg_list msg_free_list;

void msg_init(void) {
	int i;
	TAILQ_INIT(&msg_free_list);
	for (i = 0; i < NMSG; i++) {
		msgs[i].msg_status = MSG_FREE;
		TAILQ_INSERT_TAIL(&msg_free_list, &msgs[i], msg_link);
	}
}

inline u_int msg2id(struct Msg *m) {
	return (m->msg_tier) * NMSG + (m - msgs);
}
