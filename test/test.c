#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define clear() write(STDOUT_FILENO, "\x1b[2J", 4)
#define restore_cursor_pos() write(STDOUT_FILENO, "\x1b[H", 3)

struct termios orig_termios;

void disable_raw_mode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enable_raw_mode() {
	tcgetattr(STDIN_FILENO, &orig_termios);

	struct termios raw = orig_termios;

	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int getCursorPosition(int *rows, int *cols) {
	char buf[32];
	unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  printf("\r\n");
	while (i < sizeof(buf) - 1) {
	  if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
		if (buf[i] == 'R') break;
		++i;
	}
	buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

	return 0;
}


int main() {
  enable_raw_mode();
  write(STDOUT_FILENO, "\x1b[?1049h", 8);
  clear();
  restore_cursor_pos();
  write(STDOUT_FILENO, "hej", 3);
  write(STDOUT_FILENO, "\x1b[23;43H", 8);

	int rows = 0, cols = 0;
	getCursorPosition(&rows, &cols);

	printf("rows: %d, cols: %d\r\n", rows, cols);

	char c;
	while (read(STDIN_FILENO, &c, 1) != 1)
	;

  write(STDOUT_FILENO, "\x1b[?1049l", 8);

  disable_raw_mode();
	return 0;
}