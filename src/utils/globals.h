#ifndef GLOBALS_H
#define GLOBALS_H

/*** global variables ***/
#if defined(g_TERMINAL) || defined(g_ALL)
#ifdef _WIN32
  #include <windows.h>
	extern DWORD orig_term_in;
	extern DWORD orig_term_out;
#else /*_WIN32*/
  #include <termios.h>
	extern struct termios orig_term;
#endif /*_WIN32*/
#endif /*defined(g_TERMINAL) || defined(g_ALL)*/

#if defined(g_EDITOR) || defined(g_ALL)
  #include "editor.h"
  extern editor_t E;
#endif /*defined(g_EDITOR) || defined(g_ALL)*/

#if defined(g_CONFIG) || defined(g_ALL)
  #include "../config/config.h"
  extern cfg_t RC;
#endif /*defined(g_CONFIG) || defined(g_ALL)*/

#include <stdio.h>
extern FILE *logfp;

#endif /*GLOBALS_H*/