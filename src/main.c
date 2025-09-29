#define MAIN_IMPL
#include "main.h"

/*** global variables ***/
EditorConfig E;
CimRC RC;
FILE *logfp;

/*** terminal ***/
noreturn void die(const char *s) {
	perror(s);
	exit(1);
}
void disable_raw_mode() {
	unset_altscr();
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
		die("tcsetattr");
	}
}
void enable_raw_mode() {
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
		die("tcgetattr");
	}
	atexit(disable_raw_mode);
	set_altscr();

	struct termios raw = E.orig_termios;

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
int get_window_size(u32 *rows, u32 *cols) {
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		return -1;
	}
	*rows = ws.ws_row;
	*cols = ws.ws_col;
	return 0;
}
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
	return c + E.mode;
}

/*** init ***/
void editor_init() {
	E.mode = EM_NORMAL;
	E.cy = E.cx = E.rx = 0;
	E.num_rows = 0;
	E.row_offset = E.col_offset = 0;
	E.dirty = 0;
	E.row = NULL;
	E.filename = NULL;
	memset(E.statusmsg, 0, sizeof(E.statusmsg));
	E.statusmsg_time = 0;
	
	if (get_window_size(&E.scr_rows, &E.scr_cols) == -1) die("get_window_size");
	E.scr_rows -= 2;
}

int main(int argc, char *argv[]) {
	logfp = fopen("log.txt", "w");
	if (!logfp) return 1;
	atexit(close_logfp);

	config_init();

	enable_raw_mode();
	editor_init();
	if (argc >= 2) {
		fileio_open(argv[1]);
	}

	output_set_statusmsg("HELP: Ctrl+S = save | Ctrl+Q = quit");

		Box box;
		box_init(&box);
		box_set_prompt(&box, "do you want to continue? %s/%s", "yes", "no");
		box_compute(&box);
		LOG("%s\n", box.prompt);
		LOG("%.*s\n", (int)box.buf.len, box.buf.b);
		write(STDOUT_FILENO, box.buf.b, box.buf.len);
		box_free(&box);
		char c;
		while (read(STDIN_FILENO, &c, 1) != 1);

	while (1) {
		output_refresh_screen();



		input_process_keypress();
	}
	return 0;
}