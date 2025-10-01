#define CMD_IMPL
#define g_EDITOR
#include "cmd.h"

void cmd_find_callback(char *query, int key) {
  if (key == '\r' || key == '\x1b') {
    return;
  }

  for (u32_t i = 0; i < E.num_rows; i++) {
    editor_row_t *row = &E.row[i];
    char *match = strstr(row->render, query);
    if (match) {
      E.cy = i;
      E.cx = row_rx_to_cx(row, match - row->render);
      E.row_offset = E.num_rows;
      break;
    }
  }
}
void cmd_find() {
  u32_t sav_cx = E.cx;
  u32_t sav_cy = E.cy;
  u32_t sav_col_offset = E.col_offset;
  u32_t sav_row_offset = E.row_offset;
  
  char *query = input_prompt("Search: %s (ESC to cancel)", cmd_find_callback);

  if (query) {
    free(query);
  } else {
    E.cx = sav_cx;
    E.cy = sav_cy;
    E.col_offset = sav_col_offset;
    E.row_offset = sav_row_offset;
  }
}