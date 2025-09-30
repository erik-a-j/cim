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
  #include "utils/editor.h"
  #include "buf.h"
#endif
#include "utils/types.h"

typedef struct cfg_key_t {
  char seq[128];
  int len;
} cfg_key_t;

typedef struct cfg_keymap_t {
  cfg_key_t (*pairs)[2];
  int num_pairs;
} cfg_keymap_t;

typedef struct cfg_t {
  cfg_keymap_t keymap;
	u8_t tabsize;
} cfg_t;

#ifdef CONFIG_IMPL
  #include "utils/globals.h"
#endif

/*** forward declarations ***/
void config_init();
const char *config_find();
i64_t config_get_value(char *buf, i64_t bufsize, const char *data, const char *setting);

#ifdef CONFIG_IMPL
  int config_load(abuf *ab, const char *path);
#endif

#endif /*CONFIG_H*/