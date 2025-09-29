#define BOX_IMPL
#define g_ECONFIG
#include "box.h"
//#include "box_flags.h"

/*** popup ***/
void box_init(Box *st) {
  BOX_ZERO_(st);
  memset(st->prompt, 0, 256);
  ab_init(&st->ab, 256);
}
void box_free(Box *st) {
  BOX_ZERO_(st);
  memset(st->prompt, 0, 256);
  ab_free(&st->ab);
}
void box_set_prompt(Box *st, const char *fmt, ...) {
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
void box_draw_line(BoxInternal *box, u32 len) {
  //Box *st = box->st;
  abuf *ab = &box->st->ab;
  while (len) {
    ab_append(ab, " ", 1);
    --len;
  }
}
void box_draw(BoxInternal *box) {
  Box *st = box->st;
  abuf *ab = &box->st->ab;
  for (u32 y = box->box_sy; y != box->box_sy + box->st->box_rows; y++) {
    cursor_move(ab, y, box->box_sx);
    box_draw_line(box, st->box_cols);
  }
}
void box_compute(Box *st) {
  BoxInternal box = {
    .st = st,
    .box_sy = 0,
    .box_sx = 0
  };

  if (st->flags & BOX_INV_COLOR) {
    box_flags_inv_color(&box);
  }
  if (st->flags & BOX_CENTERED) {
    box_flags_centered(&box);
  }
  box_draw(&box);

  ab_append(&box.st->ab, "\x1b[m", 3);

/*  abuf *ab = &st->ab;
	ab_append(ab, "\x1b[7m", 4);
  u32 third_v = E.scr_rows / 3;
  u32 third_h = E.scr_cols / 3;
  cursor_move(ab, third_v, third_h);
  if (third_v >= 4) {
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    cursor_move(ab, third_v + third_v - 1, third_h);
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    cursor_move(ab, third_v + 1, third_h);
    if (third_v / 2 >= 4) {
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      cursor_move(ab, third_v + third_v - 2, third_h);
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      cursor_move(ab, third_v + 2, third_h);
    }

  }
  u32 len = strlen(st->prompt);
  if (len > third_h) len = third_h;
  for (u32 j = 0; j <= third_h; j++) {
    if (third_h - len == j * 2) {
      ab_append(ab, st->prompt, len);
      j += len;
    } else {
      ab_append(ab, " ", 1);
    }
  }
	ab_append(ab, "\x1b[m", 3);*/
}
void box_compute2(Box *st) {
  abuf *ab = &st->ab;
	ab_append(ab, "\x1b[7m", 4);
  u32 third_v = E.scr_rows / 3;
  u32 third_h = E.scr_cols / 3;
  cursor_move(ab, third_v, third_h);
  if (third_v >= 4) {
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    cursor_move(ab, third_v + third_v - 1, third_h);
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    cursor_move(ab, third_v + 1, third_h);
    if (third_v / 2 >= 4) {
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      cursor_move(ab, third_v + third_v - 2, third_h);
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      cursor_move(ab, third_v + 2, third_h);
    }

  }
  u32 len = strlen(st->prompt);
  if (len > third_h) len = third_h;
  for (u32 j = 0; j <= third_h; j++) {
    if (third_h - len == j * 2) {
      ab_append(ab, st->prompt, len);
      j += len;
    } else {
      ab_append(ab, " ", 1);
    }
  }
	ab_append(ab, "\x1b[m", 3);
}
