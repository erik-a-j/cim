#define EDITOR_OPERATIONS_IMPL
#include "main.h"

/*** editor operations ***/
void editor_insert_char(int c) {
  if (E.cy == E.num_rows) {
    row_insert(E.num_rows, "", 0);
  }
  row_insert_char(&E.row[E.cy], E.cx, c);
  ++E.cx;
}
void editor_insert_newline() {
  if (E.cx == 0) {
    row_insert(E.cy, "", 0);
  } else {
    Erow *row = &E.row[E.cy];
    row_insert(E.cy + 1, &row->data[E.cx], row->size - E.cx);
    row = &E.row[E.cy];
    row->size = E.cx;
    row->data[row->size] = '\0';
    row_update(row);
  }
  ++E.cy;
  E.cx = 0;
}
void editor_del_char(u32 *i) {
  if (E.cy == E.num_rows) return;
  Erow *row = &E.row[E.cy];

  if (i && *i < row->size) {
    row_del_char(row, *i);
    if (E.cx == row->size) {
      --E.cx;
    }
    return;
  }

  if (E.cx == 0 && E.cy == 0) return;
  if (E.cx > 0) {
    row_del_char(row, E.cx - 1);
    --E.cx;
  } else {
    E.cx = E.row[E.cy - 1].size;
    row_append_str(&E.row[E.cy - 1], row->data, row->size);
    row_del(E.cy);
    --E.cy;
  }
}