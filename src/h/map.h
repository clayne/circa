/*
** map.h | The Circa Library Set | Dual-Generic Maps
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_MAP_H
#define CIRCA_MAP_H

/*
** Dependencies
*/

#include "core.h"
#include "bits.h"

/*
** Type Definitions
*/

struct map_data {
  size_t  cap;
  size_t  len;
  bool   *used;
  size_t *probe;
  char   *data;
  char    key[];
};

#define Map(K, V) struct { K *k; V *v; } *
typedef void *Map;

/*
** Accessors
*/

static inline struct map_data *map(Map m);

#define map_set_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), (K), (V))
#define map_set(M, K, V) map_set_iso(typeof(*M->k), typeof(*M->v), M, K, V)
Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v);

#define map_has_iso(KT, VT, M, K) map_has_(sizeof(KT), sizeof(VT), (M), (K))
#define map_has(M, K) map_has_iso(typeof(*M->k), typeof(*M->v), M, K)
bool map_has_(size_t sizk, size_t sizv, Map m, void *k);

#define map_get_iso(KT, VT, M, K) (*((VT*) map_get_(sizeof(KT), sizeof(VT), (M), (K))))
#define map_get(M, K) map_get_iso(typeof(*M->k), typeof(*M->v), M, K)
void *map_get_(size_t sizk, size_t sizv, Map m, void *k);

/*
** Allocators
*/

#define map_alloc_iso(KT, VT, C) map_alloc_(sizeof(KT), sizeof(VT), (C))
#define map_alloc(KT, VT, C) map_alloc_iso(KT, VT, C)
Map map_alloc_(size_t sizk, size_t sizv, size_t cap);

#define map_realloc_iso(KT, VT, M, C) (M) = map_realloc_(sizeof(KT), sizeof(VT), (M), (C))
#define map_realloc(M, C) map_realloc_iso(typeof(*M->k), typeof(*M->v), M, C)
Map map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap);

#define map_require_iso(KT, VT, M, C) (M) = map_require_(sizeof(KT), sizeof(VT), (M), (C))
#define map_require(M, C) map_require_iso(typeof(*M->k), typeof(*M->v), (M), (C))
Map map_require_(size_t sizk, size_t sizv, Map m, size_t cap);

#define map_free_iso(KT, VT, M) (M) = map_free_((M))
#define map_free(M) map_free_iso(_, _, M)
Map map_free_(Map m);

/*
** Accessors Implementation
*/

static inline
struct map_data *map(Map m) { 
  return ((struct map_data*) m) - 1;
}

/*
** Iterators
*/

#define map_foreach_iso(TK, TV, M, K, V) \
for (size_t I = 0, J = 0; I < map(M)->cap; I++, J = 0) \
if (map(M)->used[I]) \
for(TK K = *(TK*) (map(M)->key + (I * sizeof(TK))); J != 1; J = 1) \
for(TV V = *(TV*) (map(M)->data + (I * sizeof(TV))); J != 1; J = 1)
#define map_foreach(M, K, V) map_foreach_iso(typeof(*M->k), typeof(*M->v), M, K, V)

#endif // CIRCA_MAP_H
