#ifndef ROW_OPERATIONS_H
#define ROW_OPERATIONS_H

#ifdef ROW_OPERATIONS_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "utils/editor.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"
#include "utils/editor.h"


/*** forward declarations ***/
void row_insert(u32_t at, const char *s, u32_t l);
void row_free(Erow *r);
void row_del(u32_t at);
void row_insert_char(Erow *r, u32_t at, int c);
void row_append_str(Erow *r, char *s, u32_t l);
void row_del_char(Erow *r, u32_t at);
void row_update(Erow *r);
int row_cx_to_rx(Erow *r, u32_t cx);

#endif /*ROW_OPERATIONS_H*/