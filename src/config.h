#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_IMPL
#endif
#include "types.h"

typedef struct CimRC {
	u8 tabsize;
} CimRC;

/*** forward declarations ***/
const char *config_find();
const char *config_get_value(const char *s1, const char *s2);
void config_init();

#endif /*CONFIG_H*/