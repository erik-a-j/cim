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
void row_free(editor_row_t *r);
void row_del(u32_t at);
void row_insert_char(editor_row_t *r, u32_t at, int c);
void row_append_str(editor_row_t *r, char *s, u32_t l);
void row_del_char(editor_row_t *r, u32_t at);
void row_update(editor_row_t *r);
u32_t row_cx_to_rx(editor_row_t *r, u32_t cx);
u32_t row_rx_to_cx(editor_row_t *r, u32_t rx);

#endif /*ROW_OPERATIONS_H*/