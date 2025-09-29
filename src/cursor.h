#ifndef CURSOR_H
#define CURSOR_H

#ifdef CURSOR_IMPL
  #ifdef _WIN32
  #else
    #include <unistd.h>
  #endif
  #include <stdio.h>
  #include <ctype.h>
  #include "utils/portable.h"
  #include "buf.h"
#endif
#include "utils/types.h"

/*** forward declarations ***/
typedef struct abuf abuf;
int cursor_get_pos(int *y, int *x);
void cursor_move(abuf *ab, u32_t y, u32_t x);
void cursor_move_relative(abuf *ab, int y, int x);

#endif /*CURSOR_H*/