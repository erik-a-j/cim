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
//#include "box.h"
#include "config/config.h"
#include "file_io.h"
#include "input.h"
#include "output.h"

#include "utils/globals.h"

/*** defines ***/

#endif /*MAIN_H*/