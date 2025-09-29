#ifndef GLOBAL_H
#define GLOBAL_H

typedef struct EditorConfig EditorConfig;
typedef struct CimRC CimRC;
typedef struct FILE FILE;
typedef struct DWORD DWORD;
typedef struct termios termios;

/*** global variables ***/
extern EditorConfig E;
extern CimRC RC;
extern FILE *logfp;
#ifdef _WIN32
	extern DWORD orig_term_in;
	extern DWORD orig_term_out;
#else
	extern struct termios orig_term;
#endif

#endif /*GLOBAL_H*/