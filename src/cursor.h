#ifndef CURSOR_H
#define CURSOR_H

#ifdef CURSOR_IMPL
  #include <unistd.h>
  #include <ctype.h>
#endif
#include "types.h"

/*** forward declarations ***/
typedef struct abuf abuf;
int cursor_get_pos(int *y, int *x);
void cursor_move(abuf *ab, u32 y, u32 x);
void cursor_move_relative(abuf *ab, int y, int x);

#endif /*CURSOR_H*/