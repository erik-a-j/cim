#ifndef MAIN_H
#define MAIN_H

#define _DEFAULT_SOURCE
#ifndef _WIN32
	#define _BSD_SOURCE
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#else
	#include <unistd.h>
#endif

#include "utils/portable.h"
#include "buf.h"
#include "box.h"
#include "config.h"
#include "file_io.h"
#include "input.h"
#include "output.h"

#include "utils/globals.h"

//#include <errno.h>
//#include <stdnoreturn.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#ifdef _WIN32
//	#include <windows.h>
//#else
//	#include <termios.h>
//#endif
//#ifdef MAIN_IMPL
//	#ifdef _WIN32
//		#include <io.h>
//	#else
//		#include <unistd.h>
//		#include <sys/ioctl.h>
//	#endif
//#endif

/*** forward declarations ***/


//#include "buf.h"
//#include "box.h"
//#include "box_flags.h"
//#include "cursor.h"
//#include "config.h"
//#include "editor_operations.h"
//#include "file_io.h"
//#include "input.h"
//#include "output.h"
//#include "row_operations.h"
//#include "terminal.h"

/*** defines ***/

#endif /*MAIN_H*/