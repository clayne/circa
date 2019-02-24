/*
** map.h | The Circa Library Set | Dual-Generic Maps
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_MAP_H
#define CIRCA_MAP_H

#include "core.h"
#include "bits.h"

/*
** This just silences dumb and unnecessary warnings.
**
** In the case of `-Wpadded`, padding is a *feature* that improves speed and
** portability. It's really not an issue at all, so I'm not sure why clang
** would be spouting it other than to say "you have more space you could be
** using"... but in my case I don't need any more than a bool at the end of
** map_bucket, so I really don't care.
**
** As for `-Wcast-align`, this is just complaining about me casting for the
** `map` function, which is fine but the alignment afforded by `malloc` is
** already enough for this to work 100% of the time, so again there's no real
** problem here, just clang being pedantic for the sake of being pedantic.
*/

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpadded"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wcast-align"
#endif

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

static inline
struct map_data *map(Map m) { 
  return ((struct map_data*) m) - 1;
}

#define map_foreach_iso(TK, TV, M, K, V) \
for (size_t I = 0, J = 0; I < map(M)->cap; I++, J = 0) \
if (map(M)->used[I]) \
for(TK K = *(TK*) (map(M)->key + (I * sizeof(TK))); J != 1; J = 1) \
for(TV V = *(TV*) (map(M)->data + (I * sizeof(TV))); J != 1; J = 1)
#define map_foreach(M, K, V) map_foreach_iso(typeof(*M->k), typeof(*M->v), M, K, V)

#ifdef __clang__
  #pragma clang diagnostic pop // -Wcast-align
  #pragma clang diagnostic pop // -Wpadded
#endif

#endif // CIRCA_MAP_H
