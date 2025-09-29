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

#define print_bitidx(flag) printf(#flag" bit index == %d\n", __builtin_ctzll(flag))

const char *aaa[4] = {
	[__builtin_ctzll(CC)] = "hello",
	[__builtin_ctzll(BB)] = "NO",
	[__builtin_ctzll(AA)] = "yeeee",
	[__builtin_ctzll(DD)] = "ssddsds",
};

int main() {
	u64_t a = 0;
	printf("0 %% 2 = %d\n", 0 % 2);
	a |= (AA | CC);
	if (~a & BB) printf("BB not set\n");
	printf("%lu\n", a);
	IS_FLAG(a, AA);
	IS_FLAG(a, BB);
	IS_FLAG(a, CC);
	IS_FLAG(a, DD);
	print_bitidx(AA);
	print_bitidx(BB);
	print_bitidx(CC);
	print_bitidx(DD);
	printf("%s\n", aaa[__builtin_ctzll(AA)]);
	printf("%s\n", aaa[__builtin_ctzll(BB)]);
	printf("%s\n", aaa[__builtin_ctzll(CC)]);
	printf("%s\n", aaa[__builtin_ctzll(DD)]);
}