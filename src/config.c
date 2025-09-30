#define CONFIG_IMPL
#define g_CONFIG
#include "config.h"

/*** config ***/
i64_t config_read_keymaps(abuf *ab, cfg_keymap_t *st) {
	u32_t num_keymaps = 0;
	u32_t cap = 8;
	st->pairs = malloc(sizeof(cfg_key_t[2]) * cap);
	if (!st->pairs) return -1;

	u32_t i = 0;
	char *ptr = ab->b;
	char buf[127];
	i64_t len;
	while ((len = config_get_value(buf, sizeof(buf), ptr, "nmap"))) {
		if (len == -1) return -1;
		LOG("buf: %s", buf);

		if (i >= cap) {
			u32_t newcap = cap << 1;
			void *newpairs = realloc(st->pairs, sizeof(cfg_key_t[2]) * newcap);
			if (!newpairs) return -1;

			st->pairs = newpairs;
			cap = newcap;
		}

		memcpy(st->pairs[i][0].seq, buf, len);
		memcpy(st->pairs[i][1].seq, buf, len);

		ptr += len;
		num_keymaps++;
		i++;
	}

	st->num_pairs = num_keymaps;

	return num_keymaps;
}
void config_init() {
	RC.keymap.pairs = NULL;
	RC.keymap.num_pairs = 0;
	RC.tabsize = 4;
	const char *rc_path = config_find();
	if (!rc_path) return;

	abuf ab;
	ab_init(&ab, 64);
	if (config_load(&ab, rc_path) != 0) {
		ab_free(&ab);
		return;
	}

	if (config_read_keymaps(&ab, &RC.keymap) == -1) {
		LOG("ERROR");
		return;
	}

	for (u32_t i = 0; i < RC.keymap.num_pairs; i++) {
		LOG("keymap: %s, %s", RC.keymap.pairs[i][0].seq, RC.keymap.pairs[i][1].seq);
	}
	free(RC.keymap.pairs);

	char *endptr;
	char tabsize_s[100];
	i64_t len = config_get_value(tabsize_s, sizeof(tabsize_s), ab.b, "tabsize");

	LOG("%s", tabsize_s);
	if (len > 0) {
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
	i64_t linecap = 0;
	i32_t linelen;
	while ((linelen = getline_p(&line, &linecap, fp)) != -1) {
		LOG("%s", line);
		ab_append(ab, line, linelen);
	}
	free(line);
	fclose(fp);
	ab_append(ab, "\0", 1);

	return 0;
}
i64_t config_get_value(char *buf, i64_t bufsize, const char *data, const char *setting) {
	if (!buf || bufsize < 1 || !data || !setting) return -1;

	i64_t len = 0;

	memset(buf, '\0', bufsize);

	const char *s = strstr(data, setting);
	if (!s) return 0;

	s += strlen(setting);
	while (*s == ' ' || *s == '\t') ++s;

	while (len < bufsize && *s != '\r' && *s != '\n' && *s != '\0') {
		buf[len++] = *s++;
	}

	buf[len] = '\0';
	while (len > 0) {
		if (buf[len - 1] == ' ') {
			buf[--len] = '\0';
		} else break;
	}

	return len;
}

//char *config_map_key(keys_t *k) {
//	
//}