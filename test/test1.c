#include <stdio.h>
#include <stdint.h>

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

#define NOF 0

#define AA 1UL << 0
#define BB 1UL << 1
#define CC 1UL << 2
#define DD 1UL << 3

#define IS_FLAG(a, flag) printf(#a" & "#flag" == %s\n", (a & flag)? "TRUE" : "FALSE")

typedef enum editor_mode_t {
	EM_INSERT = 0,
	EM_NORMAL = 0x400,
	EM_VISUAL = 0x800
} editor_mode_t;

typedef enum {
	E_KFLAG_ESCSEQ = 1 << 0,
	E_KFLAG_CTRL = 1 << 1,
	E_KFLAG_SHFT = 1 << 2,
	E_KFLAG_ALT = 1 << 3
} keyflags_t;

typedef struct editor_keys_t {
	const u8_t *const seq;
	const u8_t *const endptr;
	const u8_t bytes_read;
	const keyflags_t flags;
} editor_keys_t;


editor_keys_t test() {
	static u8_t seq[3];
	seq[0] = 1;
	seq[1] = 2;
	seq[2] = 3;
	editor_keys_t ed = {
		.seq = &seq[0],
		.endptr = seq + 3
	};
	return ed;
}

int main() {
	editor_keys_t ed = test();
	for (const u8_t *a = ed.seq; a != ed.endptr; a++) {
		printf("%p, %u\n", a, *a);
	}
}