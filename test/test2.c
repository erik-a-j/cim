#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef int8_t i8_t_t;
typedef int16_t i16_t_t;
typedef int32_t i32_t_t;
typedef int64_t i64_t_t;
typedef uint8_t u8_t_t;
typedef uint16_t u16_t_t;
typedef uint32_t u32_t_t;
typedef uint64_t u64_t_t;

typedef struct test_t {
  struct internal {
    u32_t_t box_sy;
    u32_t_t box_sx;
    struct corner_char_t {
      char lb;
      char lt;
      char rt;
      char rb;
    } corner;
    struct line_char_t {
      char v;
      char h;
    } line;
    struct input_t {
      union {
        struct {
          char str[64];
        };
        struct {
          char y[32];
          char n[32];
        } yn;
        char raw[64];
      };
    } in_t;
  } _;
} test_t;
#define pchar(c) printf(#c" == %c\n", c)
int main() {
  test_t a = {0};
  printf("test_t %zu\n", sizeof(test_t));
  printf("a %zu\n", sizeof(a));
  memset(a._.in_t.str, (int)'_', 63);
  a._.in_t.str[63] = '\0';
  printf("%s\n", a._.in_t.str);
  //a._.in_t.str[0] = 'G';
  //a._.in_t.str[1] = 'T';
  //a._.in_t.str[2] = 'A';
  //a._.in_t.str[32] = 'U';
  //pchar(a._.in_t.str[0]);
  //pchar(a._.in_t.str[1]);
  //pchar(a._.in_t.str[2]);
  //pchar(a._.in_t.yn.y[0]);
  //pchar(a._.in_t.yn.n[0]);
  //pchar(a._.in_t.raw[0]);
  //pchar(a._.in_t.raw[32]);
}