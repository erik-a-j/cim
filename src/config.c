#define CONFIG_IMPL
#define g_CIMRC
#include "config.h"

/*** config ***/
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
const char *config_get_value(const char *data, const char *setting) {
	static char v[64];
	if (!data || !setting) return NULL;

	memset(v, 0, sizeof(v));

	const char *s = strstr(data, setting);
	if (!s) return NULL;

	s += strlen(setting);
	while (*s == ' ' || *s == '=') ++s;

	int i = 0;
	while (*s != '\r' && *s != '\n' && *s != ' ') {
		v[i++] = *s++;
	}
	v[i] = '\0';

	return &v[0];
}
void config_init() {
	RC.tabsize = 4;
	const char *rc_path = config_find();
	if (!rc_path) return;

	FILE *fp = fopen(rc_path, "r");
	if (!fp) return;

	abuf ab;
	ab_init(&ab, 64);

	char *line = NULL;
	i64_t linecap = 0;
	i32_t linelen;
	while ((linelen = getline_p(&line, &linecap, fp)) != -1) {
		LOG("%s\n", line);
		ab_append(&ab, line, linelen);
	}
	free(line);
	ab_append(&ab, "\0", 1);

	char *endptr;

	const char *tabsize_s = config_get_value(ab.b, "tabsize");
	LOG("%s\n", tabsize_s);
	if (tabsize_s) {
		u64_t tabsize = strtoull(tabsize_s, &endptr, 10);
		if (endptr != tabsize_s && *endptr == '\0') {
			if (tabsize <= UINT8_MAX) {
				RC.tabsize = (u8_t)tabsize;
			}
		}
	}
	ab_free(&ab);
	fclose(fp);
}