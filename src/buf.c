#define BUF_IMPL
#include "buf.h"

/*** append buffer ***/
void ab_init(abuf *ab, u64 size) {
	ab->b = malloc(size);
	die_if(ab->b == NULL);
	ab->len = 0;
	ab->cap = size;
}
void ab_free(abuf *ab) {
	if (ab) free(ab->b);
}
void ab_append(abuf *ab, const char *s, u64 len) {
	if (ab->cap < ab->len + len) {
		u64 new_cap = (ab->cap)? ab->cap * 2 : 8;
		while (new_cap < ab->len + len) new_cap <<= 1;

		char *new = realloc(ab->b, new_cap);
		if (!new) return;

		ab->b = new;
		ab->cap = new_cap;
	}
	memcpy(&ab->b[ab->len], s, len);
	ab->len += len;
}
void ab_fmt_append(abuf *ab, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char buf[1024];
	int len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	die_if(len >= 1022);
	ab_append(ab, buf, len);
}
