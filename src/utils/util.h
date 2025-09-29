#ifndef UTIL_H
#define UTIL_H
#include "../terminal.h"

/*** defines ***/
#define CIM_VERSION "0.0.1"
#define LOG(f, ...) do {                              \
  if (logfp) {                                        \
    fprintf(logfp, "%4d|"f, __LINE__, ##__VA_ARGS__); \
    fflush(logfp);                                    \
  }                                                   \
} while (0)
#define die_if(cond) do {                                       \
	if ((cond)) {                                                 \
		LOG("%s: "#cond" == TRUE", __func__);                       \
		char tmp_dbgmsg[256];                                       \
		snprintf(tmp_dbgmsg, sizeof(tmp_dbgmsg),                    \
				"\x1b[38;2;0;120;255m%s()\x1b[m: "	                    \
				"\x1b[38;2;255;65;65m"#cond" == TRUE\x1b[m", __func__); \
		die(tmp_dbgmsg);                                            \
	}                                                             \
} while (0)
#define CTRL_KEY(k) ((k) & 0x1F)
#define NORMAL_KEY(k) ((k) + EM_NORMAL)
#define NORMAL_CTRL_KEY(k) (NORMAL_KEY(CTRL_KEY((k))))

#endif /*UTIL_H*/