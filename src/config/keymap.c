#define KEYMAP_IMPL
#define g_CONFIG
#include "keymap.h"

int keyname_to_ascii(const char *s, char_t *ascii) {
  if (!s) return -1;
	const char *names[] = {"esc","space"};
	int names_len = (sizeof(names) / 8);

	*ascii = 0;
	int i;
	for (i = 0; i < names_len; i++) {
		if (strcmp(s, names[i]) == 0) break;
		if (i == names_len - 1) return -1;
	}

	switch (i) {
		case 0: *ascii = (char_t)KEY_esc;   break;
		case 1: *ascii = (char_t)KEY_space; break;
		default: break;
	}
	return 0;
}
void keymap_free(keymap_t *km) {
	if (!km) return;

	while (km) {
		keymap_t *next = km->next;
		free(km);
		km = next;
	}
}
void keymap_free_global() {
  keymap_free(RC.keymaps);
}
keymap_t *keymap_init() {

	keymap_t *km = malloc(sizeof(keymap_t));
	if (!km) return NULL;

	km->next = NULL;
	km->lhs = '\0';
	km->rhs = '\0';

	return km;
}

char_t keymap_parse_lhsrhs(const char *lhs, int *err) {
	if (!lhs) {
		*err = 1;
		return 0;
	}

	*err = 0;
	char_t ch = 0;
	int is_ctrl = 0;
	char buf[32];
	u64_t lhs_len = strlen(lhs);
	if (!lhs_len || lhs_len >= sizeof(buf)) {
		*err = 2;
		return 0;
	}

	if (lhs_len == 1) {
		return (char_t)lhs[0];
	}

	strcpy(buf, lhs);
	if (buf[0] == '<') {
		char *mod = strtok(&buf[1], "->");
		if (mod && strlen(mod) == 1) {
			if (*mod == 'C') is_ctrl = 1;
			char *key = strtok(NULL, "->");
			if (key && strlen(key) == 1) {
				ch = (char_t)*key;
			} else {
				keyname_to_ascii(key, &ch);
			}
		} else {
			char *key = mod;
			if (key && strlen(key) == 1) {
				ch = (char_t)*key;
			} else {
				keyname_to_ascii(key, &ch);
			}
		}

	}

	if (is_ctrl) {
		ch &= 0x1F;
	}
	return ch;
}
int keymap_add(keymap_t *km, const char *lhs, const char *rhs) {
	if (!km) return -1;
	keymap_t *next = km;

	while (next->lhs != '\0' && next->rhs != '\0') {
		if (next->next == NULL) {
			next->next = keymap_init();
			if (!next->next) return -1;
		}
		next = next->next;
	}

	int err;

	next->lhs = keymap_parse_lhsrhs(lhs, &err);
	if (err) return -1;
	next->rhs = keymap_parse_lhsrhs(rhs, &err);
	if (err) return -1;
	return 0;
}