#ifndef KEYS_H
#define KEYS_H
#include "types.h"

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

typedef enum {
	E_KFLAG_ESCSEQ = 1 << 0,
	E_KFLAG_CTRL = 1 << 1,
	E_KFLAG_SHFT = 1 << 2,
	E_KFLAG_ALT = 1 << 3
} keyflags_t;

typedef struct keys_t {
	const char *const seq;
	const int len;
	const keyflags_t flags;
} keys_t;

#endif /*KEYS_H*/