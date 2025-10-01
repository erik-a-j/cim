#define TERMINAL_IMPL
#define g_TERMINAL
#define g_EDITOR
#include "terminal.h"

/*** terminal ***/
noreturn void die(const char *s) {
	perror(s);
	exit(1);
}
void disable_raw_mode() {
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),  orig_term_in);
  SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), orig_term_out);
}

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

int get_window_size(u32_t *rows, u32_t *cols) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    return 0;
  *cols = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
  *rows = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
  return 1;
}

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
