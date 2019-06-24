/*
** dict.h | The Circa Library Set | Dynamic dictionaries.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

/*
** Dependencies
*/

/* Circa */

#include "seqmap.h"

/*
** Type Definitions
*/

typedef struct {
  size_t  cap;
  size_t  len;
  size_t *probe;
  char   *data;
  char   *key[];
} DictData;

#define Dict(T) struct { T k; } *
typedef void *Dict;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE SeqMapData *dict(Dict d);

#define dict_set_iso(T, D, K, V) (D) = dict_set_(sizeof(T), (D), (K), (V), false)
#define dict_set(D, K, V) dict_set_iso(typeof(D->k), D, K, V)
CIRCA Dict dict_set_(size_t siz, Dict d, char *k, void *v, bool reuse_key);

#define dict_has_iso(T, D, K) dict_has_(sizeof(T), (D), (K))
#define dict_has(D, K) dict_has_iso(typeof(D->k), D, K)
CIRCA bool dict_has_(size_t siz, Dict d, char *k);

#define dict_del_iso(T, D, K) dict_del_(sizeof(T), (D), (K))
#define dict_del(D, K) dict_del_iso(typeof(D->k), D, K)
CIRCA bool dict_del_(size_t siz, Dict d, char *k);

#define dict_get_iso(T, D, K) (*((T*) dict_get_(sizeof(T), (D), (K))))
#define dict_get(D, K) dict_get_iso(typeof(D->k), D, K)
CIRCA void *dict_get_(size_t siz, Dict d, char *k);

/* Allocators */

#define dict_alloc_iso(T, C) dict_alloc_(sizeof(T), (C))
#define dict_alloc(T, C) dict_alloc_iso(T, C)
CIRCA Dict dict_alloc_(size_t siz, size_t cap);

#define dict_realloc_iso(T, D, C) (D) = dict_realloc_(sizeof(T), (D), (C))
#define dict_realloc(D, C) dict_realloc_iso(typeof(D->k), D, C)
CIRCA Dict dict_realloc_(size_t siz, Dict d, size_t cap);

#define dict_require_iso(T, D, C) (D) = dict_require_(sizeof(T), (D), (C))
#define dict_require(D, C) dict_require_iso(typeof(D->k), D, C)
CIRCA Dict dict_require_(size_t siz, Dict d, size_t cap);

#define dict_free_iso(T, D) (D) = dict_free_(sizeof(T), (D))
#define dict_free(D) dict_free_iso(typeof(D->k), D)
CIRCA Dict dict_free_(size_t siz, Dict d);

/*
** Accessors
*/

static inline CIRCA_PURE
SeqMapData *dict(Dict d) {
  circa_guard (!d)
    return (circa_throw(CE_ARG), d);
  return ((SeqMapData*) d) - 1;
}

/*
** Iterators
*/

#define dict_foreach_iso(T, D, K, V) \
for (size_t I = 0, J = 0; I < dict(D)->cap; I++, J = 0) \
if (dict(D)->key[I]) \
for (char *K = dict(D)->key[I]; J != 1; J = 1) \
for (T V = *(T*) (dict(D)->data + (I * sizeof(T))); J != 1; J = 1)
#define dict_foreach(D, K, V) dict_foreach_iso(typeof(D->k), D, K, V)

#endif // CIRCA_DICT_H

