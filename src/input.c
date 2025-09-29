#define INPUT_IMPL
#include "main.h"

/*** input ***/
char *input_prompt(char *prompt) {
  u32 bufsize = 128;
  char *buf = malloc(bufsize);
  die_if(buf == NULL);

  u32 buflen = 0;
  buf[0] = '\0';

  while (1) {
    output_set_statusmsg(prompt, buf);
    output_refresh_screen();

    int c = terminal_read_key() - E.mode;
    if (c == '\r') {
      if (buflen != 0) {
        output_set_statusmsg("");
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }
  }
}
void input_move_cursor(int key) {
	Erow *row = (E.cy >= E.num_rows)? NULL : &E.row[E.cy];
	u32 mode = (E.mode)? 1 : 0;
	switch (key) {
		case ARROW_LEFT:
			if (E.cx != 0) {
				--E.cx;
			} else if (E.cy > 0) {
				--E.cy;
				E.cx = E.row[E.cy].size - mode;
			}
			break;
		case MOVE_LEFT:
			if (E.cx != 0) {
				--E.cx;
			} else if (!E.mode && E.cy > 0) {
				--E.cy;
				E.cx = E.row[E.cy].size;
			}
			break;
		case ARROW_RIGHT:
			if (row && E.cx < row->size - mode) {
				++E.cx;
			} else if (row && E.cx == row->size - mode) {
				++E.cy;
				E.cx = 0;
			}
			break;
		case MOVE_RIGHT:
			if (E.mode) {
				if (row && E.cx < row->size - 1) {
					++E.cx;
				}
			} else if (row && E.cx < row->size) {
				++E.cx;
			} else if (row && E.cx == row->size) {
				++E.cy;
				E.cx = 0;
			}
			break;
		case MOVE_UP:
			if (E.cy != 0) {
				--E.cy;
			}
			break;
		case MOVE_DOWN:
			if (E.cy < E.num_rows) {
				++E.cy;
			}
			break;
	}

	row = (E.cy >= E.num_rows)? NULL : &E.row[E.cy];
	u32 rowlen = (row)? row->size : 0;
	if (E.cx > rowlen) {
		E.cx = (E.mode && rowlen > 0)? rowlen - 1 : rowlen;
	}
}
void input_process_keypress() {
	static int quit_times = 1;
	int c = terminal_read_key();
  
	LOG("key: %c,%d,%d\n", c, c, c - E.mode);

	switch (c) {
		case '\r':
			editor_insert_newline();
			break;

		case NORMAL_CTRL_KEY('q'):
		case CTRL_KEY('q'):
			if (E.dirty && quit_times > 0) {
				output_set_statusmsg("WARNING!!! File has unsaved changes. "
						"Press Ctrl+Q %d more times to quit.", quit_times--);
				return;
			}
			exit(0);
			break;
		
		case NORMAL_CTRL_KEY('s'):
		case CTRL_KEY('s'):
			fileio_save();
			break;

		case '\x1b':
			if (E.mode == EM_INSERT) {
				E.mode = EM_NORMAL;
				Erow *row = (E.cy >= E.num_rows)? NULL : &E.row[E.cy];
				if (row && E.cx == row->size) {
					input_move_cursor(MOVE_LEFT);
				}
			}
			break;
		case NORMAL_KEY('i'):
			if (E.mode == EM_NORMAL) E.mode = EM_INSERT;
			break;
		case NORMAL_KEY('a'):
			if (E.mode == EM_NORMAL) {
				E.mode = EM_INSERT;
				input_move_cursor(MOVE_RIGHT);
			}
			break;

		case NORMAL_KEY('x'):
			editor_del_char(&E.cx);
			break;

		case BACKSPACE:
			editor_del_char(NULL);
			break;

		case ARROW_UP:    /* fall through */
		case ARROW_DOWN:  /* fall through */
		case ARROW_LEFT:  /* fall through */
		case ARROW_RIGHT: /* fall through */
		case MOVE_UP:     /* fall through */
		case MOVE_DOWN:   /* fall through */
		case MOVE_LEFT:   /* fall through */
		case MOVE_RIGHT:
			input_move_cursor(c);
			break;
		case PAGE_UP: /* fall through */
		case PAGE_DOWN:
			{
				int times = 5;
				while (times--)
					input_move_cursor((c == PAGE_UP)? MOVE_UP : MOVE_DOWN);
			}
			break;
		case PAGE_LEFT: /* fall through */
		case PAGE_RIGHT:
			{
				int times = 5;
				while (times--)
					input_move_cursor((c == PAGE_LEFT)? MOVE_LEFT : MOVE_RIGHT);
			}
			break;
		
		case NORMAL_CTRL_KEY('l'):
		case CTRL_KEY('l'):
			break;

		default:
			if (E.mode == EM_INSERT) {
				editor_insert_char(c);
			}
			break;
	}
	quit_times = 1;
}