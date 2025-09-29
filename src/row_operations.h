#ifndef ROW_OPERATIONS_H
#define ROW_OPERATIONS_H

#ifdef ROW_OPERATIONS_IMPL
#endif
#include "types.h"

typedef struct Erow {
	u32 size;
	u32 rsize;
	char *data;
	char *render;
} Erow;

/*** forward declarations ***/
void row_insert(u32 at, const char *s, u32 l);
void row_free(Erow *r);
void row_del(u32 at);
void row_insert_char(Erow *r, u32 at, int c);
void row_append_str(Erow *r, char *s, u32 l);
void row_del_char(Erow *r, u32 at);
void row_update(Erow *r);
int row_cx_to_rx(Erow *r, u32 cx);

#endif /*ROW_OPERATIONS_H*/