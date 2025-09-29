#ifndef FILE_IO_H
#define FILE_IO_H

#ifdef FILE_IO_IMPL
  #ifdef _WIN32
  #else
    #include <unistd.h>
  #endif
  #include <errno.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <fcntl.h>
  #include "utils/portable.h"
  #include "utils/editor.h"
  #include "row_operations.h"
  #include "input.h"
  #include "output.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"

/*** forward declarations ***/
void fileio_open(const char *s);
void fileio_save();
char *fileio_rows_to_str(u64 *l);

#endif /*FILE_IO_H*/