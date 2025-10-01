#ifndef KEYMAP_H
#define KEYMAP_H

#ifdef KEYMAP_IMPL
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "../utils/keys.h"
  #include "config.h"
  #include "../utils/globals.h"
#endif
#include "../utils/types.h"

typedef struct keymap_t {
  struct keymap_t *next;
  char_t lhs;
  char_t rhs;
} keymap_t;

/*** forward declarations ***/
keymap_t *keymap_init();
int keymap_add(keymap_t *km, const char *lhs, const char *rhs);
void keymap_free(keymap_t *km);
void keymap_free_global();
int keyname_to_ascii(const char *s, char_t *ascii);
char_t keymap_parse_lhsrhs(const char *lhs, int *err);

#endif /*KEYMAP_H*/