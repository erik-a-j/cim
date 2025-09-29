#include <stdio.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define AA 1UL << 0
#define BB 1UL << 1
#define CC 1UL << 2
#define DD 1UL << 3

#define IS_FLAG(a, flag) printf(#a" & "#flag" == %s\n", (a & flag)? "TRUE" : "FALSE")

int main() {
	u64 a = 0;
	a |= (AA | CC);
	printf("%lu\n", a);
	IS_FLAG(a, AA);
	IS_FLAG(a, BB);
	IS_FLAG(a, CC);
	IS_FLAG(a, DD);
}