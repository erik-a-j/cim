#ifndef BUF_H
#define BUF_H

#ifdef BUF_IMPL
  #ifdef _WIN32
  #else
  #endif
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>
	#include "utils/editor.h"
	#include "utils/util.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"

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