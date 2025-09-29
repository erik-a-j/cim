#ifndef FILE_IO_H
#define FILE_IO_H

#ifdef FILE_IO_IMPL
  #include <string.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif
#include "types.h"

/*** forward declarations ***/
void fileio_open(const char *s);
void fileio_save();
char *fileio_rows_to_str(u64 *l);

#endif /*FILE_IO_H*/