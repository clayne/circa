/*
** map.h | The Circa Library Set | Dynamic maps.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_MAP_H
#define CIRCA_MAP_H

/*
** Dependencies
*/

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

static inline MapData *map(Map m);

#define map_set_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), (K), (V))
#define map_set(M, K, V) map_set_iso(typeof(M->k), typeof(M->v), M, K, V)
Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v);

#define map_del_iso(KT, VT, M, K) map_del_(sizeof(KT), sizeof(VT), (M), (K))
#define map_del(M, K) map_del_iso(typeof(M->k), typeof(M->v), M, K)
bool map_del_(size_t sizk, size_t sizv, Map m, void *k);

#define map_has_iso(KT, VT, M, K) map_has_(sizeof(KT), sizeof(VT), (M), (K))
#define map_has(M, K) map_has_iso(typeof(M->k), typeof(M->v), M, K)

bool map_has_(size_t sizk, size_t sizv, Map m, void *k);

void *map_get_(size_t siz, size_t sizv, Map m, void *k);

/* Allocators */

#define map_alloc_iso(KT, VT, C) map_alloc_(sizeof(KT), sizeof(VT), (C))
#define map_alloc(KT, VT, C) map_alloc_iso(KT, VT, C)
Map map_alloc_(size_t sizk, size_t sizv, size_t cap);



#endif // CIRCA_MAP_H
