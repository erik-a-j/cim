#define g_ALL
#include "main.h"

/*** global variables ***/
EditorConfig E;
CimRC RC;
FILE *logfp;
#ifdef _WIN32
	DWORD orig_term_in;
	DWORD orig_term_out;
#else
	struct termios orig_term;
#endif

void close_logfp() {
	if (logfp) fclose(logfp);
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

		box_t box;
		box_init(&box);
		//box_set_prompt(&box, "do you want to continue? %s/%s", "yes", "no");
		box.height = 10;
		box.width = 40;
		box.flags |= (B_CENTERED |  B_COLOR_INV);
		box_compute(&box);
		//LOG("%s\n", box.prompt);
		LOG("%.*s\n", (int)box.ab.len, box.ab.b);
		write(STDOUT_FILENO, box.ab.b, box.ab.len);
		box_free(&box);
		char c;
		while (read(STDIN_FILENO, &c, 1) != 1);

	while (1) {
		output_refresh_screen();



		input_process_keypress();
	}
	return 0;
}