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
  char *query = input_prompt("Search: %s (ESC to cancel)", cmd_find_callback);
  if (query) {
    free(query);
  }
}