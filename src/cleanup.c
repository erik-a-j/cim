#define CLEANUP_IMPL
#include "main.h"

/*** cleanup ***/

void close_logfp() {
	if (logfp) fclose(logfp);
}