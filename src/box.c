#define BOX_IMPL
#include "main.h"

/*** popup ***/
void box_init(Box *st) {
  st->box_cols = st->box_rows = 0;
  st->corner.lb = st->corner.lt = '\0';
  st->corner.rt = st->corner.rb = '\0';
  st->line.v = st->line.h = '\0';
  st->fg[0] = st->fg[1] = st->fg[2] = 0;
  st->bg[0] = st->bg[1] = st->bg[2] = 0;
  memset(st->prompt, 0, 256);
  ab_init(&st->buf, 256);
}
void box_free(Box *st) {
  st->box_cols = st->box_rows = 0;
  st->corner.lb = st->corner.lt = '\0';
  st->corner.rt = st->corner.rb = '\0';
  st->line.v = st->line.h = '\0';
  st->fg[0] = st->fg[1] = st->fg[2] = 0;
  st->bg[0] = st->bg[1] = st->bg[2] = 0;
  memset(st->prompt, 0, 256);
  ab_free(&st->buf);
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
void box_compute(Box *st) {
  abuf *ab = &st->buf;
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
void box_compute2(Box *st) {
  abuf *ab = &st->buf;
	ab_append(ab, "\x1b[7m", 4);
  u32 third_v = E.scr_rows / 3;
  u32 third_h = E.scr_cols / 3;
  ab_fmt_append(ab, "\x1b[%d;%dH", third_v, third_h);
  if (third_v >= 4) {
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    ab_fmt_append(ab, "\x1b[%d;%dH", third_v + third_v - 1, third_h);
    for (u32 j = 0; j < third_h; j++) {
      ab_append(ab, " ", 1);
    }
    ab_fmt_append(ab, "\x1b[%d;%dH", third_v + 1, third_h);
    if (third_v / 2 >= 4) {
      ab_fmt_append(ab, "\x1b[%d;%dH", third_v + 1, third_h);
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      ab_fmt_append(ab, "\x1b[%d;%dH", third_v + third_v - 2, third_h);
      for (u32 j = 0; j < third_h; j++) {
        ab_append(ab, " ", 1);
      }
      ab_fmt_append(ab, "\x1b[%d;%dH", third_v + 2, third_h);
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