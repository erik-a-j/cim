#ifndef BOX_H
#define BOX_H

#ifdef BOX_IMPL
  #include <stdarg.h>
#endif
#include "types.h"

typedef struct abuf abuf;
typedef struct Box {
  abuf buf;
  u32 box_rows;
  u32 box_cols;
  struct {
    char lb;
    char lt;
    char rt;
    char rb;
  } corner;
  struct {
    char v;
    char h;
  } line;
  char prompt[256];
  u8 fg[3];
  u8 bg[3];
} Box;

/*** forward declarations ***/
void box_init(Box *st);
void box_free(Box *st);
void box_set_prompt(Box *st, const char *fmt, ...);
void box_compute(Box *st);

#endif /*POPUP_H*/