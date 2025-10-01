#define CONFIG_IMPL
#define g_CONFIG
#include "config.h"

/*** config ***/
i64_t config_read_keymaps(const char *b, keymap_t **out_km) {
	if (!b || !out_km) return -1;

	keymap_t *km = keymap_init();
	if (!km) return -1;
	*out_km = km;

	const char *p = b;
	u64_t len = strlen(b);

	i64_t consumed = 0;
	char m[128];
	while ((consumed = config_get_value(m, sizeof(m), p, "nmap")) > 0) {
		char *lhs = strtok(m, "\t =");
		printf("lhs: %s ", lhs);
		char *rhs = strtok(NULL, "\t =");
		printf("rhs: %s\n", rhs);

		keymap_add(km, lhs, rhs);

		if ((u64_t)(p - b) >= len) return 0;

		// Jump to the end of the current line
		const char *line_end = p + consumed;
		if ((u64_t)(line_end - b) > len) line_end = b + len;

		p = line_end;

		// Skip any and all line endings: \n, \r, or \r\n — robustly
		while ((u64_t)(p - b) < len && (*p == '\r' || *p == '\n')) p++;

		// If we’re at or past the buffer end, stop
		if ((u64_t)(p - b) >= len || *p == '\0') return 0;
	}

	return 0;
}
void config_init() {
	RC.keymaps = NULL;
	RC.tabsize = 4;
	const char *rc_path = config_find();
	if (!rc_path) return;

	abuf ab;
	ab_init(&ab, 64);
	if (config_load(&ab, rc_path) != 0) {
		ab_free(&ab);
		return;
	}

	config_read_keymaps(ab.b, &RC.keymaps);
	atexit(keymap_free_global);
	keymap_t *next = RC.keymaps;
	while (next) {
		LOG("lhs: %d, rhs: %d", next->lhs, next->rhs);
		next = next->next;
	}

	char tabsize_s[100];
	i64_t len = config_get_value(tabsize_s, sizeof(tabsize_s), ab.b, "tabsize");
	LOG("%s", tabsize_s);
	if (len > 0) {
		char *endptr;
		u64_t tabsize = strtoull(tabsize_s, &endptr, 10);
		if (endptr != tabsize_s && *endptr == '\0') {
			if (tabsize <= UINT8_MAX) {
				RC.tabsize = (u8_t)tabsize;
			}
		}
	}
	ab_free(&ab);
}
const char *config_find() {
	static const char *rc_here = ".cimrc";
	static const char *rc_path = NULL;
	if (rc_path != NULL) return rc_path;

	FILE *fp;
	fp = fopen(rc_here, "r");
	if (!fp) return NULL;

	rc_path = rc_here;
	fclose(fp);
	
	return rc_path;
}
int config_load(abuf *ab, const char *path) {
	FILE *fp = fopen(path, "r");
	if (!fp) return -1;

	char *line = NULL;
	u64_t linecap = 0;
	i64_t linelen;
	while ((linelen = getline(&line, &linecap, fp)) != -1) {
		LOG("%s", line);
		ab_append(ab, line, linelen);
	}
	free(line);
	fclose(fp);
	ab_append(ab, "\0", 1);

	return 0;
}
i64_t config_get_value(char *buf, u64_t bufsize, const char *data, const char *setting) {
	if (!buf || bufsize < 2 || !data || !setting) return -1;

	memset(buf, 0, bufsize);

	u64_t keylen = strlen(setting);
	const char *p = data;
	while (p && *p != '\0') {
		const char *line = p;
		const char *nl = strpbrk(line, "\r\n");
		u64_t linelen = nl ? (u64_t)(nl - line) : strlen(line);

		u64_t i = 0;
		while (i < linelen && (line[i] == ' ' || line[i] == '\t')) i++;

		if (i + keylen <= linelen
				&& strncmp(line + i, setting, keylen) == 0
				&& (i + keylen == linelen || line[i + keylen] == ' '
						|| line[i + keylen] == '\t' || line[i + keylen] == '=')) {

			i += keylen;
			while (i < linelen && (line[i] == ' ' || line[i] == '\t')) i++;
			if (i < linelen && line[i] == '=') i++;
			while (i < linelen && (line[i] == ' ' || line[i] == '\t')) i++;

			u64_t j = linelen;
			while (j > i && (line[j - 1] == ' ' || line[j - 1] == '\t')) j--;
			u64_t val_len = (j > i)? (j - i) : 0;

			if (val_len >= bufsize) val_len = bufsize - 1;
			memcpy(buf, line + i, val_len);
			buf[val_len] = '\0';
			return (val_len)? (i64_t)j : 0;
		}

		p = nl ? nl + 1 : line + linelen;
		if (*p == '\r' && *(p+1) == '\n') p += 2;
	}
	return 0;
}

//char *config_map_key(keys_t *k) {
//	
//}