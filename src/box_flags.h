#ifndef BOX_FLAGS_H
#define BOX_FLAGS_H

typedef enum box_flag_t {
  B_NO_FLAGS  = 0,
  B_COLOR_INV = 1UL << 0,
  B_CENTERED  = 1UL << 1,

  B_IN_T_YN   = 1UL << 2,
  B_IN_T_STR  = 1UL << 3,

  B_NOPROMPT  = 1UL << 4
} box_flag_t;

typedef enum box_err_t {
  BOX_ERR_NULLPROMPT = 1
} box_err_t;

/*** defines ***/
/*#define BOX_CENTERED    1UL << 0
#define BOX_INV_COLOR   1UL << 1

#define BOX_IN_T_BOOL   1UL << 2
#define BOX_IN_T_STR    1UL << 3*/

#endif /*BOX_FLAGS_H*/