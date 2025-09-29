#define BOX_IMPL
#define g_ECONFIG
#include "box.h"

/*** box ***/
static inline void box_zero(box_t *st) {
  st->flags &= B_NO_FLAGS;
  st->width = st->height = 0;
  st->corner.lb = st->corner.lt = '\0';
  st->corner.rt = st->corner.rb = '\0';
  st->line.v = st->line.h = '\0';
  st->fg[0] = st->fg[1] = st->fg[2] = 0;
  st->bg[0] = st->bg[1] = st->bg[2] = 0;
  st->err = 0;
  st->_.box_sy = st->_.box_sx = 0;
  memset(st->prompt, 0, BOX_T_PROMPT_SIZE);
  memset(st->_.in_t.raw, 0, BOX_T_INPUT_T_SIZE);
}
void box_init(box_t *st) {
  box_zero(st);
  ab_init(&st->_.ab, 256);
}
void box_free(box_t *st) {
  box_zero(st);
  ab_free(&st->_.ab);
}
void box_set_prompt(box_t *st, const char *fmt, ...) {
  va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(st->prompt, sizeof(st->prompt), fmt, ap);
  if (len >= 254) {
    st->prompt[252] = '.';
    st->prompt[253] = '.';
    st->prompt[254] = '.';
  }
  st->prompt[255] = '\0';
	va_end(ap);
}

static inline int box_compute_flags(box_t *st) {
  if_flag_and_apply(B_COLOR_INV);
  if_flag_and_apply(B_CENTERED);
  return 0;
}
int box_compute(box_t *st) {
  int err = box_compute_flags(st);
  if (err) return err;

  box_draw(st);

  ab_append(&st->_.ab, "\x1b[m", 3);
  return 0;
}
void box_run(box_t *st, char *buf) {

}
void box_draw(box_t *st) {
  box_draw_body(st);
  if (~st->flags & B_NOPROMPT) {
    box_draw_prompt(st);
  }
  box_draw_input(st);
}
static inline void box_draw_input(box_t *st) {
  struct box_input_t *in = &st->_.in_t;
  if (st->flags & B_IN_T_YN) {
    u64_t y_len = strlen(in->yn.y);
    u64_t n_len = strlen(in->yn.n);
    u64_t len = y_len + 1 + n_len;
    if (len > st->width) len = st->width;

    u32_t in_y = st->_.box_sy + (st->height / 2);
    u32_t in_x = st->_.box_sx + (st->width - len) / 2;
    cursor_move(&st->_.ab, in_y, in_x);

    ab_append(&st->_.ab, in->yn.y, y_len);
    ab_append(&st->_.ab, " ", 1);
    ab_append(&st->_.ab, in->yn.n, n_len);
  }
}
static inline void box_draw_prompt(box_t *st) {
  u64_t len = strlen(st->prompt);
  if (len == 0) {
    st->err = BOX_ERR_NULLPROMPT;
    return;
  }
  if (len > st->width) len = st->width;

  u32_t prompt_y = st->_.box_sy + (st->height / 2) - 1;
  u32_t prompt_x = st->_.box_sx + (st->width - len) / 2;
  
  cursor_move(&st->_.ab, prompt_y, prompt_x);
  ab_append(&st->_.ab, st->prompt, len);
}
static inline void box_draw_line(box_t *st, u32_t len) {
  while (len) {
    ab_append(&st->_.ab, " ", 1);
    --len;
  }
}
static inline void box_draw_body(box_t *st) {
  for (u32_t y = st->_.box_sy; y != st->_.box_sy + st->height; y++) {
    cursor_move(&st->_.ab, y, st->_.box_sx);
    box_draw_line(st, st->width);
  }
}
/*** box flag operations ***/
static inline int box_apply(B_COLOR_INV)(box_t *st) {
  return ab_append(&st->_.ab, "\x1b[7m", 4);
}
static inline int box_apply(B_CENTERED)(box_t *st) {
  if (st->height > E.scr_rows) st->height = E.scr_rows;
  if (st->width > E.scr_cols) st->width = E.scr_cols;
  u32_t margin_rows = E.scr_rows - st->height;
  u32_t margin_cols = E.scr_cols - st->width;
  st->_.box_sy = (margin_rows)? margin_rows / 2 : 0;
  st->_.box_sx = (margin_cols)? margin_cols / 2 : 0;
  return 0;
}
static inline int box_apply(B_IN_T_YN)(box_t *st) {
  struct box_input_t *a = &st->_.in_t;
  if (a->yn.y[0] == '\0') strcpy(a->yn.y, "yes");
  if (a->yn.n[0] == '\0') strcpy(a->yn.n, "no");
  return 0;
}
static inline int box_apply(B_IN_T_STR)(box_t *st) {
  struct box_input_t *a = &st->_.in_t;
  if (a->str[0] == '\0') {
    memset(a->str, (int)'_', BOX_T_INPUT_T_STR_SIZE - 1);
    a->str[BOX_T_INPUT_T_STR_SIZE - 1] = '\0';
  }
  return 0;
}