#ifndef EDITOR_H
#define EDITOR_H
#include "types.h"
#include "util.h"
#include "keys.h"


typedef enum editor_mode_t {
	EM_INSERT = 0,
	EM_NORMAL = 0x400,
	EM_VISUAL = 0x800
} editor_mode_t;

typedef enum {
	ARROW       = 0x1000,
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
} editor_key_t;

typedef struct editor_row_t {
	u32_t size;
	u32_t rsize;
	char *data;
	char *render;
} editor_row_t;

typedef struct editor_t {
	editor_mode_t mode;
	u32_t cy;
	u32_t cx;
	u32_t rx;
	u32_t row_offset;
	u32_t col_offset;
	u32_t scr_rows;
	u32_t scr_cols;
	u32_t num_rows;
	i64_t dirty;
	editor_row_t *row;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
} editor_t;

#endif /*EDITOR_H*/