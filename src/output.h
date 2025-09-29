#ifndef OUTPUT_H
#define OUTPUT_H

#ifdef OUTPUT_IMPL
  #ifdef _WIN32
  #else
    #include <unistd.h>
  #endif
  #include <stdio.h>
  #include <stdarg.h>
  #include <string.h>
  #include <time.h>
  #include "utils/portable.h"
  #include "buf.h"
  #include "row_operations.h"
  #include "utils/globals.h"
#endif

/*** forward declarations ***/
typedef struct abuf abuf;
void output_scroll();
void output_draw_rows(abuf *ab);
void output_draw_status_bar(abuf *ab);
void output_draw_statusmsg_bar(abuf *ab);
void output_set_statusmsg(const char *fmt, ...);
void output_refresh_screen();

#endif /*OUTPUT_H*/