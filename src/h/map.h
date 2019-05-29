/*
** map.h | The Circa Library Set | Dynamic maps.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_MAP_H
#define CIRCA_MAP_H

/*
** Dependencies
*/

/* Standard */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Circa */

#include "config.h"

/*
** Type Definitions
*/

typedef struct {
  size_t cap;
  size_t len;
  intmax_t *probe; // We use intmax_t here to have negative mean unused.
  char *data;
  char key[];
} MapData;

#define Map(K, V) struct { K k; V v; } *
typedef void *Map;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE MapData *map(Map m);

#define map_set_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), (K), (V))
#define map_set(M, K, V) map_set_iso(typeof((M)->k), typeof((M)->v), M, K, V)
CIRCA CIRCA_RETURNS Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v);

#define map_del_iso(KT, VT, M, K) map_del_(sizeof(KT), sizeof(VT), (M), (K))
#define map_del(M, K) map_del_iso(typeof((M)->k), typeof((M)->v), M, K)
CIRCA bool map_del_(size_t sizk, size_t sizv, Map m, void *k);

#define map_has_iso(KT, VT, M, K) map_has_(sizeof(KT), sizeof(VT), (M), (K))
#define map_has(M, K) map_has_iso(typeof((M)->k), typeof((M)->v), M, K)
CIRCA bool map_has_(size_t sizk, size_t sizv, Map m, void *k);

#define map_get_iso(KT, VT, M, K) ((VT*) map_get_(sizeof(KT), sizeof(VT), (M), (K)))
#define map_get(M, K) map_get_iso(typeof((M)->k), typeof((M)->v), M, K)
CIRCA void *map_get_(size_t siz, size_t sizv, Map m, void *k);

/* Allocators */

#define map_alloc_iso(KT, VT, C) map_alloc_(sizeof(KT), sizeof(VT), (C))
#define map_alloc(KT, VT, C) map_alloc_iso(KT, VT, C)
CIRCA CIRCA_ALLOCS Map map_alloc_(size_t sizk, size_t sizv, size_t cap);

#define map_realloc_iso(KT, VT, M, C) (M) = map_realloc_(sizeof(KT), sizeof(VT), (M), (C))
#define map_realloc(KT, VT, M, C) map_realloc_iso(typeof((M)->k), typeof((M)->v), M, K)
CIRCA CIRCA_RETURNS Map map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap);

#define map_require_iso(KT, VT, M, C) (M) = map_require_(sizeof(KT), sizeof(VT), (M), (C))
#define map_require(M, C) map_require_iso(typeof((M)->k), typeof((M)->v), M, C)
CIRCA CIRCA_RETURNS Map map_require_(size_t sizk, size_t sizv, Map m, size_t cap);

#define map_free_iso(KT, VT, M) (M) = map_free_(sizeof(KT), sizeof(VT), (M))
#define map_free(M) map_free_iso(typeof((M)->k), typeof((M)->v), M)
CIRCA CIRCA_RETURNS Map map_free_(size_t sizk, size_t sizv, Map m);

/*
** Function Definitions
*/

/* Accessors */

static inline CIRCA_PURE
MapData *map(Map m) {
  return ((MapData*) m) - 1;
}

/*
** Iterators
*/

#define map_foreach_iso(TK, TV, M, K, V) \
for (size_t I = 0, J = 0; I < map(M)->cap; I++, J = 0) \
if (map(M)->probe[I] != -1) \
for (TK K = *(TK*) (map(M)->key  + (I * sizeof(TK))); J != 1; J = 1) \
for (TV V = *(TV*) (map(M)->data + (I * sizeof(TV))); J != 1; J = 1)
#define map_foreach(M, K, V) map_foreach_iso(typeof((M)->k), typeof((M)->v), M, K, V)

/*
** Header Only Mode
*/ 

#ifdef CIRCA_HEADER_ONLY
  #include "../c/map.c"
#endif

#endif // CIRCA_MAP_H
