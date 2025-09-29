#ifndef EDITOR_OPERATIONS_H
#define EDITOR_OPERATIONS_H

#ifdef EDITOR_OPERATIONS_IMPL
#endif
#include "types.h"

/*** forward declarations ***/
void editor_insert_char(int c);
void editor_insert_newline();
void editor_del_char(u32 *i);

#endif /*EDITOR_OPERATIONS_H*/