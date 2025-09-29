#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "utils/portable.h"
  #include "buf.h"
  #include "utils/globals.h"
#endif
#include "utils/types.h"



/*** forward declarations ***/
const char *config_find();
const char *config_get_value(const char *s1, const char *s2);
void config_init();

#endif /*CONFIG_H*/