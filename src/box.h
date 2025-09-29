#ifndef BOX_H
#define BOX_H

#ifdef BOX_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <string.h>
  #include <stdarg.h>
  #include "utils/editor.h"
  #include "buf.h"
  #include "cursor.h"
  #include "utils/globals.h"
  #define BOX_ZERO_(x) do {                   \
    Box *x__ = _Generic((x),                  \
        Box*: (x),                            \
        Box: &(x)                             \
    );                                        \
    x__->box_cols = x__->box_rows = 0;        \
    x__->corner.lb = x__->corner.lt = '\0';   \
    x__->corner.rt = x__->corner.rb = '\0';   \
    x__->line.v = x__->line.h = '\0';         \
    x__->fg[0] = x__->fg[1] = x__->fg[2] = 0; \
    x__->bg[0] = x__->bg[1] = x__->bg[2] = 0; \
  } while (0)
  #define BOXINTERNAL_ZERO_(x) do {  \
    BoxInternal *x__ = _Generic((x), \
        BoxInternal*: (x),           \
        BoxInternal: &(x)            \
    );                               \
    x__->box_sy = 0;          \
    x__->box_sx = 0;          \
  } while (0)
#endif /*BOX_IMPL*/
#include "box_flags.h"
#include "utils/types.h"

typedef struct abuf abuf;
typedef struct Box {
  abuf ab;
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
  u64 flags;
  u8 fg[3];
  u8 bg[3];
} Box;
typedef struct BoxInternal {
  Box *st;
  u32 box_sy;
  u32 box_sx;
} BoxInternal;

/*** forward declarations ***/
void box_init(Box *st);
void box_free(Box *st);
void box_set_prompt(Box *st, const char *fmt, ...);
void box_compute(Box *st);
void box_draw_line(BoxInternal *box, u32 len);
void box_draw(BoxInternal *box);

#ifdef BOX_IMPL
static inline void box_flags_inv_color(BoxInternal *box) {
  ab_append(&box->st->ab, "\x1b[7m", 4);
}
static inline void box_flags_centered(BoxInternal *box) {
  Box *st = box->st;
  if (st->box_rows > E.scr_rows) st->box_rows = E.scr_rows;
  if (st->box_cols > E.scr_cols) st->box_cols = E.scr_cols;
  u32 margin_rows = E.scr_rows - st->box_rows;
  u32 margin_cols = E.scr_cols - st->box_cols;
  box->box_sy = (margin_rows)? margin_rows / 2 : 0;
  box->box_sx = (margin_cols)? margin_cols / 2 : 0;
}
#endif /*BOX_IMPL*/

#endif /*POPUP_H*/