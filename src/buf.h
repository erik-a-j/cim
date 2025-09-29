#ifndef BUF_H
#define BUF_H

#ifdef BUF_IMPL
	#include <stdarg.h>
#endif
#include "types.h"

typedef struct abuf {
	char *b;
	u64 len;
	u64 cap;
} abuf;


/*** forward declarations ***/
void ab_init(abuf *ab, u64 size);
void ab_append(abuf *ab, const char *s, u64 l);
void ab_fmt_append(abuf *ab, const char *fmt, ...);
void ab_free(abuf *ab);

#endif /*BUF_H*/