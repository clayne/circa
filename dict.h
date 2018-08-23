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

/*
** Dependencies
*/

/* Vendored */

#ifndef XXH_INLINE_ALL
  #define XXH_INLINE_ALL
#endif

#include "lib/xxHash/xxhash.h"

/* Internal */

#include "core.h"
#include "types.h"
#include "bits.h"

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
#define dict_set(D, A, V) dict_set_iso(typeof(*D), (D), (A), (V))
_circa_ _circa_rets_ Dict dict_set_(size_t siz, Dict d, char *a, void *val, CircaMsg fname, CircaMsg line);

#define dict_get_iso(T, D, A) (*((T*) dict_get_(sizeof(T), (D), (A), __FILE__, _circa_str_(__LINE__))))
#define dict_get(D, A) dict_get_iso(typeof(*D), (D), (A))
_circa_ void *dict_get_(size_t siz, Dict d, char *a, CircaMsg fname, CircaMsg line);

/* Allocators */

#define dict_new_iso(T, C) dict_new_(sizeof(T), (C), __FILE__, _circa_str_(__LINE__))
#define dict_new(T, C) dict_new_iso(T, C)
_circa_ _circa_rets_ Dict dict_new_(size_t siz, size_t cap, CircaMsg fname, CircaMsg line);

#define dict_rsz_iso(T, D, C) (D) = dict_rsz_(sizeof(T), (D), (C), __FILE__, _circa_str_(__LINE__))
#define dict_rsz(D, C) dict_rsz_iso(typeof(*D), (D), (C))
_circa_ _circa_rets_ Dict dict_rsz_(size_t siz, Dict d, size_t cap, CircaMsg fname, CircaMsg line);

#define dict_rqr_iso(T, D, C) (D) = dict_rqr_(sizeof(T), (D), (C), __FILE__, _circa_str_(__LINE__))
#define dict_rqr(D, C) dict_rqr_iso(typeof(*D), (D), (C))
_circa_ _circa_rets_ Dict dict_rqr_(size_t siz, Dict d, size_t cap, CircaMsg fname, CircaMsg line);

#define dict_del_iso(T, D) (D) = dict_del_(sizeof(T), (D), __FILE__, _circa_str_(__LINE__))
#define dict_del(D) dict_del_iso(typeof(*D), (D))
_circa_ _circa_rets_ Dict dict_del_(size_t siz, Dict d, CircaMsg fname, CircaMsg line);

/*
** Accessors
*/

/*
** -- dict_ --
** Description
**   Returns a pointer to the structure of a dictionary.
** Arguments
**   d: Dictionary (Dict)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Dictionary Structure Pointer (struct DictData*)
*/

_circa_
struct DictData *dict_(Dict d, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(d != NULL, fname, line);
  }
  return ((struct DictData*) d) - 1;
}

/*
** -- dict_set_ --
** Description
**   Sets a dictionary's value at a given address.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   a: Address (char*)
**   val: Value (void*)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_set_(size_t siz, Dict d, char *a, void *val, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
  }
  if (dict(d)->len == dict(d)->cap)
    d = dict_rsz_(siz, d, dict(d)->cap * 2, fname, line);
  const size_t len = strlen(a);
  const size_t hash = (sizeof(size_t) == 8) ? XXH64(a, len, 0) : XXH32(a, len, 0);
  const size_t addr = hash % dict(d)->cap;
  memcpy(((char*) d) + siz * addr, val, siz);
  dict(d)->len++;
  return d;
}

/*
** -- dict_get_ --
** Description
**   Gets a dictionary's value at a given address.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   a: Address (char*)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Value (void*)
*/

_circa_
void *dict_get_(size_t siz, Dict d, char *a, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
  }
  const size_t len = strlen(a);
  const size_t hash = (sizeof(size_t) == 8) ? XXH64(a, len, 0) : XXH32(a, len, 0);
  const size_t addr = hash % dict(d)->cap;
  return ((char*) d) + siz * addr;
}

/*
** Allocators
*/

/*
** -- dict_new_ --
** Description
**   Allocates a new dictionary of a given type-size and capaciy.
** Arguments
**   siz: Type Size (size_t)
**   cap: Capacity (size_t)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_new_(size_t siz, size_t cap, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  cap = u32_primegt(cap);
  const size_t blk = cap * siz;
  struct DictData *d = NULL;
  while (d == NULL) d = malloc(sizeof(*d) + blk);
  d->cap = cap;
  d->len = 0;
  return d->data;
}

/*
** -- dict_rsz_ --
** Description
**   Resizes a dictionary's capacity.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   cap: Capacity (size_t)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_rsz_(size_t siz, Dict d, size_t cap, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(d != NULL, fname, line);
  }
  cap = u32_primegt(cap);
  const size_t blk = cap * siz;
  struct DictData *d2 = NULL;
  while (d2 == NULL) d2 = realloc(dict(d), sizeof(*d) + blk);
  d2->cap = cap;
  return d2->data;
}

/*
** -- dict_rqr_ --
** Description
**   Requires that a dictionary have at least a given capacity.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   cap: Capacity (size_t)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_rqr_(size_t siz, Dict d, size_t cap, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(d != NULL, fname, line);
  }
  if (dict(d)->cap < cap) d = dict_rsz_(siz, d, cap, fname, line);
  return d;
}

/*
** -- dict_del_ --
** Description
**   Deletes a dictionary.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   fname: Filename (CircaMsg)
**   line: Line Number (CircaMsg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_del_(size_t siz, Dict d, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
  }
  if (d != NULL) free(dict(d));
  return NULL;
}

#endif /* CIRCA_DICT_H */
