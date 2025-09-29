#ifndef INPUT_H
#define INPUT_H

#ifdef INPUT_IMPL
  #include <ctype.h>
#endif
#include "types.h"

/*** forward declarations ***/
char *input_prompt(char *s);
void input_move_cursor(int k);
void input_process_keypress();

#endif /*INPUT_H*/