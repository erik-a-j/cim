#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
typedef i16_t char_t;

enum { KEY_esc = 27, KEY_space = 32, KEY_excl, KEY_quot, KEY_hash, KEY_dollar, KEY_percnt, KEY_and,
       KEY_apos, KEY_Lparen, KEY_Rparen, KEY_ast, KEY_plus, KEY_comma, KEY_minus, KEY_dot, KEY_slash };
enum { KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 };
enum { KEY_colon = 58, KEY_semi, KEY_lt, KEY_eq, KEY_gt, KEY_quest, KEY_at };
enum { KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
       KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z };
enum { KEY_Lbrack = 91, KEY_Bslash, KEY_Rbrack, KEY_caret, KEY_lowbar, KEY_Btick };
enum { KEY_a = 97, KEY_b, KEY_c, KEY_d, KEY_e, KEY_f, KEY_g, KEY_h, KEY_i, KEY_j, KEY_k, KEY_l, KEY_m,
       KEY_n, KEY_o, KEY_p, KEY_q, KEY_r, KEY_s, KEY_t, KEY_u, KEY_v, KEY_w, KEY_x, KEY_y, KEY_z };
enum { KEY_Lbrace = 123, KEY_or, KEY_Rbrace, KEY_tilde, KEY_DEL };

int keyname_to_ascii(const char *s, char_t *ascii) {
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

typedef struct keymap_t {
  struct keymap_t *next;
  char_t lhs;
  char_t rhs;
} keymap_t;

void keymap_free(keymap_t *km) {
	if (!km) return;

	while (km) {
		keymap_t *next = km->next;
		free(km);
		km = next;
	}
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
		if (strlen(mod) == 1) {
			if (*mod == 'C') is_ctrl = 1;
			char *key = strtok(NULL, "->");
			if (strlen(key) == 1) {
				ch = (char_t)*key;
			} else {
				keyname_to_ascii(key, &ch);
			}
		} else {
			char *key = mod;
			if (strlen(key) == 1) {
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

int main() {
	const char *b = 
			"nmap <C-s> e\n"
			"nmap <space> <C-s>\n"
			"nmap S h\n";

	keymap_t *km = NULL;
	config_read_keymaps(b, &km);

	keymap_t *next = km;
	while (next) {
		printf("nmap lhs,rhs: %u,%u\n", next->lhs, next->rhs);
		next = next->next;
	}

	keymap_free(km);
} 