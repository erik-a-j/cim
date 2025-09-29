#ifndef EDITOR_H
#define EDITOR_H
#include "types.h"
#include "util.h"

typedef struct CimRC {
	u8 tabsize;
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
	u32 size;
	u32 rsize;
	char *data;
	char *render;
} Erow;

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
} EditorConfig;

#endif /*EDITOR_H*/