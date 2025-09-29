#ifndef MAIN_H
#define MAIN_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <errno.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "types.h"
#ifdef MAIN_IMPL
	#include <unistd.h>
	#include <sys/ioctl.h>
#endif

/*** forward declarations ***/
	// terminal
noreturn void die(const char *s);
void disable_raw_mode();
void enable_raw_mode();
int get_window_size(u32 *r, u32 *c);
int terminal_read_key();
	// init
void editor_init();

#include "buf.h"
#include "box.h"
#include "cursor.h"
#include "config.h"
#include "file_io.h"
#include "input.h"
#include "output.h"
#include "row_operations.h"
#include "editor_operations.h"
#include "cleanup.h"

typedef enum Emode {
	EM_NORMAL = 1000,
	EM_INSERT = 0
} Emode;

typedef struct EditorConfig {
	Emode mode;
	u32 cy;
	u32 cx;
	u32 rx;
	u32 row_offset;
	u32 col_offset;
	u32 scr_rows;
	u32 scr_cols;
	u32 num_rows;
	i64 dirty;
	Erow *row;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
	struct termios orig_termios;
} EditorConfig;

enum editor_key {
	ARROW       = 10000,
	BACKSPACE   = 127,
	MOVE_LEFT   = 'h' + EM_NORMAL,
	MOVE_RIGHT  = 'l' + EM_NORMAL,
	MOVE_UP     = 'k' + EM_NORMAL,
	MOVE_DOWN   = 'j' + EM_NORMAL,
	PAGE_LEFT   = 'H' + EM_NORMAL,
	PAGE_RIGHT  = 'L' + EM_NORMAL,
	PAGE_UP     = 'K' + EM_NORMAL,
	PAGE_DOWN   = 'J' + EM_NORMAL,
	ARROW_LEFT  = 'h' + ARROW,
	ARROW_RIGHT = 'l' + ARROW,
	ARROW_UP    = 'k' + ARROW,
	ARROW_DOWN  = 'j' + ARROW
};

/*** global variables ***/
extern EditorConfig E;
extern CimRC RC;
extern FILE *logfp;


/*** defines ***/
#define CIM_VERSION "0.0.1"

#define set_altscr() write(STDOUT_FILENO, "\x1b[?1049h", 8)
#define unset_altscr() write(STDOUT_FILENO, "\x1b[?1049l", 8)

#define CTRL_KEY(k) ((k) & 0x1F)
#define INSERT_KEY(k) (k)
#define NORMAL_KEY(k) ((k) + EM_NORMAL)
#define NORMAL_CTRL_KEY(k) (NORMAL_KEY(CTRL_KEY((k))))

#define to_lower(k) ((k) + 32)
#define to_upper(k) ((k) - 32)
#define clear() write(STDOUT_FILENO, "\x1b[2J", 4)
#define restore_cursor_pos() write(STDOUT_FILENO, "\x1b[H", 3)

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

#endif /*MAIN_H*/