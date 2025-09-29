#ifndef TERMINAL_H
#define TERMINAL_H

#ifdef TERMINAL_IMPL
  #ifdef _WIN32
    #include <windows.h>
		#include <io.h>
	#else
    #include <termios.h>
		#include <unistd.h>
		#include <sys/ioctl.h>
  #endif
  #include <errno.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include "utils/editor.h"
  #include "utils/globals.h"
  /*** defines ***/
  #ifdef _WIN32
    #define set_altscr() _write(STDOUT_FILENO, "\x1b[?1049h", 8)
    #define unset_altscr() _write(STDOUT_FILENO, "\x1b[?1049l", 8)
  #else
    #define set_altscr() write(STDOUT_FILENO, "\x1b[?1049h", 8)
    #define unset_altscr() write(STDOUT_FILENO, "\x1b[?1049l", 8)
  #endif /*_WIN32*/
#endif /*TERMINAL_IMPL*/

#include <stdnoreturn.h>
#include "utils/types.h"
#include "utils/editor.h"

/*** forward declarations ***/
noreturn void die(const char *s);
void disable_raw_mode();
void enable_raw_mode();
int get_window_size(u32 *rows, u32 *cols);
int terminal_read_key();

#endif /*TERMINAL_H*/