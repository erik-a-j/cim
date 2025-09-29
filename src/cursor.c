#define CURSOR_IMPL
#include "main.h"

int cursor_get_pos(int *y, int *x) {
	char buf[32];
	u32 i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  printf("\r\n");
	while (i < sizeof(buf) - 1) {
	  if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
		if (buf[i] == 'R') break;
		++i;
	}
	buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", y, x) != 2) return -1;

	return 0;
}
void cursor_move(abuf *ab, u32 y, u32 x) {
  ab_fmt_append(ab, "\x1b[%d;%dH", y + 1, x + 1);
}
void cursor_move_relative(abuf *ab, int y, int x) {
  int cur_y = 0, cur_x = 0;
  if (cursor_get_pos(&cur_y, &cur_x) != 0) return;

  cursor_move(ab,
      (u32)((cur_y + y >= 0)? cur_y + y : 0),
      (u32)((cur_x + x >= 0)? cur_y + x : 0));

}