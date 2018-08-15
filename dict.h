/*
** dict.h | A dictionary implementation using fnv1a.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

/*
** Compatability Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/dict.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/dict.h]: C99 is required."
#endif

#error "[circa/dict.h]: This module is not implemented yet."

/*
** Dependencies
*/

/* Internal */

#include "core.h"
#include "types.h"
#include "hash.h"
#include "str.h"

/*
** Type Definitions
*/

#define Dict(T) T*
typedef Dict(void) Dict;

struct DictData {
  size_t cap;
  size_t len;
  char data[];
};

/*
** Function Prototypes
*/

/* Accessors */

#define dict(D) dict_((D), __FILE__, _circa_str_(__LINE__))
_circa_ struct DictData *dict_(Dict d, CircaMsg fname, CircaMsg line);

#define dict_set_iso(T, D, A, V) (D) = dict_set_(sizeof(T), (D), (A), &(T){V}, __FILE__, _circa_str_(__LINE__))
#define dict_set(D, A, V) dict_set_iso(typeof(*D), (A), (V))
_circa_ _circa_rets_ Dict dict_set_(size_t siz, Dict d, CircaMsg fname, CircaMsg line);

/* Allocators */

#define dict_new_iso(T, C) dict_new_(sizeof(T), (C), __FILE__, _circa_str_(__LINE__))
#define dict_new(T, C) dict_new_iso(T, C)
_circa_ _circa_rets_ Dict dict_new_(size_t siz, Dict d, CircaMsg fname, CircaMsg line);

#endif /* CIRCA_DICT_H */
