#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_IMPL
  #ifdef _WIN32
  #else
  #endif
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  //#include "utils/portable.h"
  #include "../utils/editor.h"
  #include "../buf.h"
#endif
#include "../utils/types.h"
#include "keymap.h"

typedef struct cfg_t {
  struct keymap_t *keymaps;
	u8_t tabsize;
} cfg_t;

#ifdef CONFIG_IMPL
  #include "../utils/globals.h"
#endif

/*** forward declarations ***/
void config_init();
const char *config_find();
i64_t config_get_value(char *buf, u64_t bufsize, const char *data, const char *setting);

#ifdef CONFIG_IMPL
  int config_load(abuf *ab, const char *path);
  i64_t config_read_keymaps(const char *b, keymap_t **out_km);
#endif

#endif /*CONFIG_H*/