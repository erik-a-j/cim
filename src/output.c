#define OUTPUT_IMPL
#define g_EDITOR
#include "output.h"

/*** output ***/
void output_scroll() {
	E.rx = 0;
	if (E.cy < E.num_rows) {
		E.rx = row_cx_to_rx(&E.row[E.cy], E.cx);
	}

	if (E.cy < E.row_offset) {
		E.row_offset = E.cy;
	}
	if (E.cy >= E.row_offset + E.scr_rows) {
		E.row_offset = E.cy - E.scr_rows + 1;
	}
	if (E.rx < E.col_offset) {
		E.col_offset = E.rx;
	}
	if (E.rx >= E.col_offset + E.scr_cols) {
		E.col_offset = E.rx - E.scr_cols + 1;
	}
}
void output_draw_rows(abuf *ab) {
	for (u32_t y = 0; y < E.scr_rows; y++) {
		ab_append(ab, "\x1b[K", 3);
		u32_t filerow = y + E.row_offset;
		if (filerow >= E.num_rows) {
			if (E.num_rows == 0 && y == E.scr_rows / 3) {
				char welcome[40];
				u32_t welcomelen = snprintf(welcome, sizeof(welcome),
						"Cim editor -- version %s", CIM_VERSION);
				if (welcomelen > E.scr_cols) welcomelen = E.scr_cols;

				u32_t padding = (E.scr_cols - welcomelen) / 2;
				if (padding) {
					ab_append(ab, "~", 1);
					--padding;
				}
				while (padding--) ab_append(ab, " ", 1);
				ab_append(ab, welcome, welcomelen);
			} else {
				ab_append(ab, "~", 1);
			}
		} else {
			i32_t len = E.row[filerow].rsize - E.col_offset;
			if (len < 0) len = 0;
			if (len > (i32_t)E.scr_cols) len = (i32_t)E.scr_cols;
			ab_append(ab, &E.row[filerow].render[E.col_offset], len);
		}
		//if (y < E.scr_rows - 1) {
		ab_append(ab, "\r\n", 2);
		//}
	}
}
void output_draw_status_bar(abuf *ab) {
	ab_append(ab, "\x1b[7m", 4);
	char status[80], rstatus[80], mstatus[20];
	i64_t len = snprintf(status, sizeof(status), "%.20s - %u lines %s ",
			(E.filename)? E.filename : "[No Name]",
			E.num_rows,
			(E.dirty)? "(modified)" : "");
	i64_t rlen = snprintf(rstatus, sizeof(rstatus), "%u/%u",
			E.cy + 1, E.num_rows);
	i64_t mlen = snprintf(mstatus, sizeof(mstatus), "--%s--",
			(E.mode == EM_NORMAL)? "NORMAL" : "INSERT");

	if (len > (i64_t)E.scr_cols) len = E.scr_cols;
	ab_append(ab, status, len);

	if (mlen > (i64_t)E.scr_cols - len) mlen = E.scr_cols - len;
	ab_append(ab, mstatus, mlen);

	i64_t llen = len + mlen;
	while (llen < (i64_t)E.scr_cols) {
		if (E.scr_cols - llen == rlen) {
			ab_append(ab, rstatus, rlen);
			break;
		} else {
			ab_append(ab, " ", 1);
			++llen;
		}
	}
	ab_append(ab, "\x1b[m", 3);
	ab_append(ab, "\r\n", 2);
}
void output_draw_statusmsg_bar(abuf *ab) {
	ab_append(ab, "\x1b[K", 3);
	u32_t msglen = strlen(E.statusmsg);
	if (msglen > E.scr_cols) msglen = E.scr_cols;
	if (msglen && time(NULL) - E.statusmsg_time < 5) {
		ab_append(ab, E.statusmsg, msglen);
	}
}
void output_set_statusmsg(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
	va_end(ap);
	E.statusmsg_time = time(NULL);
}
void output_refresh_screen() {
	output_scroll();

	abuf ab;
	ab_init(&ab, 32);

	ab_append(&ab, "\x1b[?25l", 6);
	ab_append(&ab, "\x1b[H", 3);

	output_draw_rows(&ab);
	output_draw_status_bar(&ab);
	output_draw_statusmsg_bar(&ab);

	char buf[27];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.row_offset) + 1,
																						(E.rx - E.col_offset) + 1);
	ab_append(&ab, buf, strlen(buf));

	ab_append(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	ab_free(&ab);
}