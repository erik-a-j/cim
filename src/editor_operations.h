#ifndef EDITOR_OPERATIONS_H
#define EDITOR_OPERATIONS_H

#ifdef EDITOR_OPERATIONS_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include "row_operations.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"

/*** forward declarations ***/
void editor_insert_char(int c);
void editor_insert_newline();
void editor_del_char(u32_t *i);

#endif /*EDITOR_OPERATIONS_H*/