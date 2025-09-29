#define FILE_IO_IMPL
#define g_ECONFIG
#include "file_io.h"
 
/*** file i/o ***/
void fileio_open(const char *filename) {
	free(E.filename);
	E.filename = strdup(filename);
	
	FILE *fp = fopen(filename, "r");
	if (!fp) die("fopen");

	char *line = NULL;
	i64 linecap = 0;
	i32 linelen;
	while ((linelen = getline_p(&line, &linecap, fp)) != -1) {
		while (linelen > 0 && (line[linelen - 1] == '\n' ||
													 line[linelen - 1] == '\r'))
			--linelen;
		row_insert(E.num_rows, line, linelen);
	}
	free(line);
	fclose(fp);
	E.dirty = 0;
}
void fileio_save() {
	if (E.filename == NULL) {
		E.filename = input_prompt("Save as: %s");
	}

	u64 len;
	char *buf = fileio_rows_to_str(&len);
	if (len > INT64_MAX) {
		free(buf);
		return;
	}

	int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1) {
		if (ftruncate(fd, len) != -1) {
			if (write(fd, buf, len) == (i64)len) {
				close(fd);
				free(buf);
				E.dirty = 0;
				output_set_statusmsg("%lu bytes written to disk", len);
				return;
			}
		}
		close(fd);
	}
	free(buf);
	output_set_statusmsg("Can't save! I/O error: %s", strerror(errno));
}
char *fileio_rows_to_str(u64 *buflen) {
	u64 totlen = 0;
	for (u32 j = 0; j < E.num_rows; j++) {
		totlen += E.row[j].size + 1;
	}
	*buflen = totlen;
	if (*buflen == 0) return NULL;

	char *buf = malloc(totlen);
	die_if(buf == NULL);

	char *p = buf;
	for (u32 j = 0; j < E.num_rows; j++) {
		memcpy(p, E.row[j].data, E.row[j].size);
		p += E.row[j].size;
		*p++ = '\n';
	}

	return buf;
}