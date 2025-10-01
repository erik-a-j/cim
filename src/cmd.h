#ifndef CMD_H
#define CMD_H

#ifdef CMD_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "utils/editor.h"
  #include "input.h"
  #include "row_operations.h"
  #include "utils/globals.h"
#endif

/*** forward declarations ***/
void cmd_find_callback(char *query, int key);
void cmd_find();

#endif /*CMD_H*/