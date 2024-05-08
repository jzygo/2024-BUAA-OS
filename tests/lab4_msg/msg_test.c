#include <lib.h>

u_int ME;
u_int WHOM;

#define _debugf(FORMAT) debugf("%04X: " FORMAT "\n", ME)
#define _vdebugf(FORMAT, ...) debugf("%04X: " FORMAT "\n", ME, __VA_ARGS__)

int main() {
	ME = syscall_getenvid();
	WHOM = ME ^ 0x800 ^ 0x1001;
	int ret;
	u_int whom, value, perm;
	char *message = (char *)UTEMP;
	if (ME == 0x1001) {
		_vdebugf("Sending value to %04X...", WHOM);
		ret = msg_send(WHOM, 2024, NULL, 0);
		if (ret < 0) {
			_debugf("Failed to send value");
		} else {
			_vdebugf("Message %d sent successfully", ret);
		}

		do {
			ret = msg_recv(&whom, NULL, message, &perm);
		} while (ret == -E_NO_MSG);
		if (ret < 0) {
			_vdebugf("Failed to receive message, error code: %d", ret);
		} else {
			_vdebugf("Received message \"%s\" from %04X with permission %x", message,
				 whom, perm);
		}
	} else {
		do {
			ret = msg_recv(&whom, &value, NULL, NULL);
		} while (ret == -E_NO_MSG);
		if (ret < 0) {
			_vdebugf("Failed to receive value, error code: %d", ret);
		} else {
			_vdebugf("Received value %d from %04X", value, whom);
		}
		const char *data = "Good luck, have fun!";
		memcpy(message, data, strlen(data) + 1);
		_vdebugf("Sending message to %04X...", WHOM);
		ret = msg_send(WHOM, 0, message, 0);
		if (ret < 0) {
			_debugf("Failed to send value");
		} else {
			_vdebugf("Message %d sent successfully", ret);
		}
		while (envs[ENVX(WHOM)].env_status != ENV_FREE) {
			syscall_yield();
		}
	}
	return 0;
}
