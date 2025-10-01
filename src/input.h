#ifndef INPUT_H
#define INPUT_H

#ifdef INPUT_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <stdlib.h>
  #include <ctype.h>
  #include "utils/editor.h"
  #include "utils/util.h"
  #include "file_io.h"
  #include "output.h"
  #include "cmd.h"
  #include "editor_operations.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"

/*** forward declarations ***/
char *input_prompt(char *s, void (*callback)(char *, int));
void input_move_cursor(int k);
void input_process_keypress();

#endif /*INPUT_H*/