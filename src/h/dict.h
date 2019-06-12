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

#define Dict(T) SeqMap(char, T)
typedef void *Dict;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE SeqMapData *dict(Dict d);

static inline Dict dict_set_(size_t siz, Dict d, char *k, void *v);

static inline bool dict_has_(size_t siz, Dict d, char *k);

static inline bool dict_del_(size_t siz, Dict d, char *k);

static inline void *dict_get_(size_t siz, Dict d, char *k);

/* Allocators */

static inline Dict dict_alloc_(size_t siz, size_t cap);

static inline Dict dict_realloc_(size_t siz, Dict d, size_t cap);

static inline Dict dict_require_(size_t siz, Dict d, size_t cap);

static inline Dict dict_free_(size_t siz, Dict d);

/*
** Accessors
*/

static inline CIRCA_PURE
SeqMapData *dict(Dict d) {
  return ((SeqMapData*) d) - 1;
}

/*
** Iterators
*/

#define dict_foreach_iso(TK, TV, D, K, V) \
for (size_t I = 0, J = 0; I < dict(D)->cap; I++, J = 0) \
if (dict(D)->key[I]) \
for (TK K = *(TK*) (dict(D)->key + I); J != 1; J = 1) \
for (TV V = *(TV*) (dict(D)->data + (I * sizeof(TV)); J != 1; J = 1)
#define dict_foreach(D, K, V) dict_foreach_iso(typeof(*D->k), typeof(*D->v), D, K, V)

#endif // CIRCA_DICT_H

