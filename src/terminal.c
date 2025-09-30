#define TERMINAL_IMPL
#define g_TERMINAL
#define g_EDITOR
#include "terminal.h"

/*** terminal ***/
noreturn void die(const char *s) {
	perror(s);
	exit(1);
}
#ifdef _WIN32
void disable_raw_mode() {
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),  orig_term_in);
  SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), orig_term_out);
}
#else
void disable_raw_mode() {
	unset_altscr();
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term) == -1) {
		die("tcsetattr");
	}
}
#endif
#ifdef _WIN32
void enable_raw_mode() {

  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleMode(hin, &orig_term_in);
  GetConsoleMode(hout, &orig_term_out);
	atexit(disable_raw_mode);
	set_altscr();

  DWORD in_mode  = orig_term_in;
  DWORD out_mode = orig_term_out;

  // Raw-ish input: no line buffering, no echo, get key events
  in_mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
  in_mode |=  (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT); // if you want resize/mouse events
  SetConsoleMode(hin, in_mode);

  // Enable ANSI/VT sequences for colors/cursor movement on Win10+
  out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hout, out_mode);
}
#else
void enable_raw_mode() {
//  struct termios orig_term;

	if (tcgetattr(STDIN_FILENO, &orig_term) == -1) {
		die("tcgetattr");
	}
	atexit(disable_raw_mode);
	set_altscr();

	struct termios raw = orig_term;

	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		die("tcsetattr");
	}
}
#endif
#ifdef _WIN32
int get_window_size(u32_t *rows, u32_t *cols) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    return 0;
  *cols = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
  *rows = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
  return 1;
}
#else
int get_window_size(u32_t *rows, u32_t *cols) {
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		return -1;
	}
	*rows = ws.ws_row;
	*cols = ws.ws_col;
	return 0;
}
#endif
#ifdef _WIN32
int terminal_read_key() {
  INPUT_RECORD rec;
  DWORD nread = 0;
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  for (;;) {
    if (!ReadConsoleInput(hin, &rec, 1, &nread)) die("ReadConsoleInput");
    if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown) {
      WORD vk = rec.Event.KeyEvent.wVirtualKeyCode;
      switch (vk) {
        case VK_LEFT:  return ARROW_LEFT;
        case VK_RIGHT: return ARROW_RIGHT;
        case VK_UP:    return ARROW_UP;
        case VK_DOWN:  return ARROW_DOWN;
        case VK_NEXT:  return PAGE_DOWN;
        case VK_PRIOR: return PAGE_UP;
        default:       return rec.Event.KeyEvent.uChar.AsciiChar;
      }
    } else if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      // handle resize if you want
    }
  }
}
#else
int terminal_read_key() {
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) die("read");
	}

	if (c == '\x1b') {
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

		if (seq[0] == '[') {
			if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
				if (seq[2] == '~') {
					switch (seq[1]) {
						case '5': return PAGE_UP;
						case '6': return PAGE_DOWN;
					}
				}
			} else {
				switch (seq[1]) {
					case 'A': return ARROW_UP;
					case 'B': return ARROW_DOWN;
					case 'C': return ARROW_RIGHT;
					case 'D': return ARROW_LEFT;
				}
			}
		}
		return '\x1b';
	}
	return c;
}
int terminal_read_key1() {
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) die("read");
	}

	if (c == '\x1b') {
		char seq[3];

		if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
		if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

		if (seq[0] == '[') {
			if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
				if (seq[2] == '~') {
					switch (seq[1]) {
						case '5': return PAGE_UP;
						case '6': return PAGE_DOWN;
					}
				}
			} else {
				switch (seq[1]) {
					case 'A': return ARROW_UP;
					case 'B': return ARROW_DOWN;
					case 'C': return ARROW_RIGHT;
					case 'D': return ARROW_LEFT;
				}
			}
		}
		return '\x1b';
	}
	return c;
}
#endif