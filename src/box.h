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
#endif /*BOX_IMPL*/
#include "box_flags.h"
#include "utils/types.h"

#define BOX_T_PROMPT_SIZE 256
#define BOX_T_INPUT_T_SIZE 64
#define BOX_T_INPUT_T_STR_SIZE 64
#define BOX_T_INPUT_T_YN_SIZE 32

typedef struct abuf abuf;
typedef struct box_t{
  char prompt[BOX_T_PROMPT_SIZE];
  u32 height;
  u32 width;
  u64 flags;
  u8 fg[3];
  u8 bg[3];
  struct box_corner_char_t {
    char lb;
    char lt;
    char rt;
    char rb;
  } corner;
  struct box_line_char_t {
    char v;
    char h;
  } line;
  struct box_internal_t {
    abuf ab;
    u32 box_sy;
    u32 box_sx;
    struct box_input_t {
      union {
        struct {
          char str[BOX_T_INPUT_T_STR_SIZE];
        };
        struct {
          char y[BOX_T_INPUT_T_YN_SIZE];
          char n[BOX_T_INPUT_T_YN_SIZE];
        } yn;
        char raw[BOX_T_INPUT_T_SIZE];
      };
    } in_t;
  } _;
} box_t;


/*** forward declarations ***/
void box_init(box_t *st);
void box_free(box_t *st);
void box_set_prompt(box_t *st, const char *fmt, ...);
int box_compute(box_t *st);

#ifdef BOX_IMPL
#define box_apply(flag) box_apply_##flag
static inline int box_apply(B_COLOR_INV)(box_t *st);
static inline int box_apply(B_CENTERED)(box_t *st);
static inline int box_apply(B_IN_T_YN)(box_t *st);


static inline void box_draw_line(box_t *st, u32 len) {
  while (len) {
    ab_append(&st->_.ab, " ", 1);
    --len;
  }
}
static inline void box_draw(box_t *st) {
  for (u32 y = st->_.box_sy; y != st->_.box_sy + st->height; y++) {
    cursor_move(&st->_.ab, y, st->_.box_sx);
    box_draw_line(st, st->width);
  }
}

#define if_flag_and_apply(flag) do { \
  if (st->flags & flag) { \
    int err = box_apply(flag)(st); \
    if (err) return err; \
  } \
} while (0)
#endif /*BOX_IMPL*/

#endif /*POPUP_H*/