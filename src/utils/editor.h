#ifndef EDITOR_H
#define EDITOR_H
#include "types.h"
#include "util.h"

typedef struct CimRC {
	u8_t tabsize;
} CimRC;

typedef enum Emode {
	EM_NORMAL = 1000,
	EM_INSERT = 0
} Emode;

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

typedef struct Erow {
	u32_t size;
	u32_t rsize;
	char *data;
	char *render;
} Erow;

typedef struct EditorConfig {
	Emode mode;
	u32_t cy;
	u32_t cx;
	u32_t rx;
	u32_t row_offset;
	u32_t col_offset;
	u32_t scr_rows;
	u32_t scr_cols;
	u32_t num_rows;
	i64_t dirty;
	Erow *row;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
} EditorConfig;

#endif /*EDITOR_H*/