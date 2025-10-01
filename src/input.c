#define INPUT_IMPL
#define g_EDITOR
#include "input.h"

/*** input ***/
char *input_prompt(char *prompt, void (*callback)(char *, int)) {
  u32_t bufsize = 128;
  char *buf = malloc(bufsize);
  die_if(buf == NULL);

  u32_t buflen = 0;
  buf[0] = '\0';

  while (1) {
    output_set_statusmsg(prompt, buf);
    output_refresh_screen();

    int c = terminal_read_key();
		if (c == BACKSPACE) {
			if (buflen != 0) buf[--buflen] = '\0';
		} else if (c == '\x1b') {
			output_set_statusmsg("");
			if (callback) callback(buf, c);
			free(buf);
			return NULL;
		} else if (c == '\r') {
      if (buflen != 0) {
        output_set_statusmsg("");
				if (callback) callback(buf, c);
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
		
		if (callback) callback(buf, c);
  }
}
void input_move_cursor(int key) {
	editor_row_t *row = (E.cy >= E.num_rows)? NULL : &E.row[E.cy];
	u32_t mode = (E.mode)? 1 : 0;
	switch (key) {
		case ARROW_LEFT:
		case MOVE_LEFT:
			if (E.cx != 0) {
				--E.cx;
			} else if (E.cy > 0) {
				--E.cy;
				E.cx = E.row[E.cy].size - mode;
			}
			break;
		case ARROW_RIGHT:
		case MOVE_RIGHT:
			if (row && row->size && E.cx != row->size - mode) {
				++E.cx;
			} else if (E.cy < E.num_rows) {
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
	u32_t rowlen = (row)? row->size : 0;
	if (!rowlen || E.cx > rowlen - mode) {
		E.cx = (E.mode && rowlen > 0)? rowlen - 1 : rowlen;
	}
}
void input_process_keypress() {
	static int quit_times = 1;
	int c = terminal_read_key();
  
	LOG("key: %c,%d,%d", c, c, (c | E.mode));

	switch (c | E.mode) {
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
				editor_row_t *row = (E.cy >= E.num_rows)? NULL : &E.row[E.cy];
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
			input_move_cursor(c | E.mode);
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

		case NORMAL_KEY('/'):
			cmd_find();
			break;

		default:
			if (E.mode == EM_INSERT) {
				editor_insert_char(c);
			}
			break;
	}
	quit_times = 1;
}