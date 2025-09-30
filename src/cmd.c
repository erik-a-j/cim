#define CMD_IMPL
#define g_EDITOR
#include "cmd.h"

void cmd_find() {
  char *query = input_prompt("Search: %s (ESC to cancel)");
  if (query == NULL) return;

  for (u32_t i = 0; i < E.num_rows; i++) {
    editor_row_t *row = &E.row[i];
    char *match = strstr(row->render, query);
    if (match) {
      E.cy = i;
      E.cx = match - row->render;
      E.row_offset = E.num_rows;
      break;
    }
  }
  free(query);
}