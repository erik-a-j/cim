#define ROW_OPERATIONS_IMPL
#define g_CONFIG
#define g_EDITOR
#include "row_operations.h"

/*** row operations ***/
void row_insert(u32_t at, const char *s, u32_t len) {
	if (at > E.num_rows) return;
	
	E.row = realloc(E.row, sizeof(editor_row_t) * (E.num_rows + 1));
	memmove(&E.row[at + 1], &E.row[at], sizeof(editor_row_t) * (E.num_rows - at));

	E.row[at].size = len;
	E.row[at].data = malloc(len + 1);
	die_if(E.row[at].data == NULL);

	memcpy(E.row[at].data, s, len);
	E.row[at].data[len] = '\0';

	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	row_update(&E.row[at]);

	++E.num_rows;
	++E.dirty;
}
void row_free(editor_row_t *row) {
	free(row->render);
	free(row->data);
}
void row_del(u32_t at) {
	if (at >= E.num_rows) return;
	row_free(&E.row[at]);
	memmove(&E.row[at], &E.row[at + 1], sizeof(editor_row_t) * (E.num_rows - at - 1));
	--E.num_rows;
	++E.dirty;
}
void row_insert_char(editor_row_t *row, u32_t at, int c) {
	if (at > row->size) at = row->size;
	row->data = realloc(row->data, row->size + 2);
	memmove(&row->data[at + 1], &row->data[at], row->size - at + 1);
	++row->size;
	row->data[at] = c;
	row_update(row);
	++E.dirty;
}
void row_append_str(editor_row_t *row, char *s, u32_t len) {
	row->data = realloc(row->data, row->size + len + 1);
	memcpy(&row->data[row->size], s, len);
	row->size += len;
	row->data[row->size] = '\0';
	row_update(row);
	++E.dirty;
}
void row_del_char(editor_row_t *row, u32_t at) {
	if (at >= row->size) return;
	memmove(&row->data[at], &row->data[at + 1], row->size - at);
	--row->size;
	row_update(row);
	++E.dirty;
}
void row_update(editor_row_t *row) {
	u32_t tabs = 0;
	for (u32_t j = 0; j < row->size; j++) {
		if (row->data[j] == '\t') ++tabs;
	}

	if (row->render) free(row->render);
	row->render = malloc(row->size + (tabs * (RC.tabsize - 1)) + 1);
	die_if(row->render == NULL);

	u32_t idx = 0;
	for (u32_t j = 0; j < row->size; j++) {
		if (tabs && row->data[j] == '\t') {
			row->render[idx++] = ' ';
			while (idx % RC.tabsize != 0) row->render[idx++] = ' ';
		} else {
			row->render[idx++] = row->data[j];
		}
	}
	row->render[idx] = '\0';
	row->rsize = idx;
}
int row_cx_to_rx(editor_row_t *row, u32_t cx) {
	u32_t rx = 0;
	for (u32_t j = 0; j < cx; j++) {
		if (row->data[j] == '\t') {
			rx += (RC.tabsize - 1) - (rx % RC.tabsize);
		}
		++rx;
	}
	return rx;
}