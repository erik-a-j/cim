#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <errno.h>

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

#define clear() write(STDOUT_FILENO, "\x1b[2J", 4)
#define restore_cursor_pos() write(STDOUT_FILENO, "\x1b[H", 3)
#define set_altscr() write(STDOUT_FILENO, "\x1b[?1049h", 8)
#define unset_altscr() write(STDOUT_FILENO, "\x1b[?1049l", 8)

struct termios orig_termios;

void disable_raw_mode() {
	unset_altscr();
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enable_raw_mode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
  set_altscr();
	struct termios raw = orig_termios;

	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

typedef enum {
	E_KFLAG_NONE = 0,
	E_KFLAG_ESCSEQ = 1 << 0,
	E_KFLAG_CTRL = 1 << 1,
	E_KFLAG_SHFT = 1 << 2,
	E_KFLAG_ALT = 1 << 3
} keyflags_t;

typedef struct editor_keys_t {
	const char *const seq;
	const int len;
	const keyflags_t flags;
} editor_keys_t;

//editor_keys_t test() {
//	static u8_t seq[3];
//	seq[0] = 1;
//	seq[1] = 2;
//	seq[2] = 3;
//	editor_keys_t ed = {
//		.seq = &seq[0],
//		.endptr = seq + 3
//	};
//	return ed;
//}
editor_keys_t terminal_read_key() {
	static char seq[8];
	memset(seq, 0, 8);
	int nread;
	while ((nread = read(STDIN_FILENO, seq, 8)) < 1) {
		if (nread == -1 && errno != EAGAIN) return (editor_keys_t){NULL,0,0};
	}
	//int nread;
	//char c;
	//while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
	//	if (nread == -1 && errno != EAGAIN) return (editor_keys_t){NULL,NULL,0};
	//}
	/*if (c == '\x1b') {
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
	}*/
	editor_keys_t k = {
		.seq = &seq[0],
		.len = nread,
		.flags = E_KFLAG_NONE
	};
	return k;
}

int main() {
  enable_raw_mode();
  clear();
  restore_cursor_pos();

	char last_ch = 0;
	while (1) {	
		editor_keys_t k = terminal_read_key();
		if (!k.seq) break;

		clear();
		restore_cursor_pos();
		int val = 0;
		for (int i = 0; i < k.len; i++) {
			printf("%p, %c, %d\r\n", &k.seq[i], k.seq[i], (int)k.seq[i]);
			val += (int)k.seq[i];
		}
		printf("val: %d\r\n", val);
		printf("bytes read: %d\r\n", k.len);
		if (k.seq[0] == 'q' && last_ch == 'q') break;
		last_ch = k.seq[0];
	}

  disable_raw_mode();
	return 0;
}