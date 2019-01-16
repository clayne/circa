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

/*
** Just like the `Dict` type, `Map` has a bucket type. However, there is one key
** difference-- `Map`'s key type is `void*` instead of `char*`, because it has
** to be generic.
**
** If you're curious what the other fields mean, I'd recommend digging through
** `dict.h`, which already explains it.
*/

struct map_bucket {
  void *data; // TODO: Pack `data` and `key` into one array, if possible.
  void *key;
  size_t probe;
  bool deleted;
};

/*
** `Map` again works the same way as `Seq`, `Str`, and `Dict`. A fat pointer
** with a capacity, length, etc.
** And again much like `Dict`, the type the macro uses actually isn't the type
** of the data held by the structure-- you aren't meant to access the type
** directly, it is just to store type data for non-`_iso` versions of functions.
*/

struct map_data {
  size_t cap;
  size_t len;
  struct map_bucket buckets[];
};

/*
** Now here's something interesting-- (ab)using anonymous structs to store two
** types in the signature of our Map type. This makes the non-`_iso` calls much
** more ergonomic, saving us two arguments.
*/

#define Map(K, V) struct { K *k; V *v; } *
typedef void *Map;

/*
** Now here are the accessor functions:
*/

static inline struct map_data *map(Map m);

#define map_set_litkv_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), &(KT){K}, &(VT){V})
#define map_set_litkv(M, K, V) map_set_litkv_iso(typeof(*M->k), typeof(*M->v), M, K, V)
#define map_set_litk_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), &(KT){K}, &(V))
#define map_set_litk(M, K, V) map_set_litk_iso(typeof(*M->k), typeof(*M->v), M, K, V)
#define map_set_litv_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), &(K), &(VT){V})
#define map_set_litv(M, K, V) map_set_litv_iso(typeof(*M->k), typeof(*M->v), M, K, V)
#define map_set_iso(KT, VT, M, K, V) (M) = map_set_(sizeof(KT), sizeof(VT), (M), &(K), &(V))
#define map_set(M, K, V) map_set_iso(typeof(*M->k), typeof(*M->v), M, K, V)
Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v);

#define map_get_litk_iso(KT, VT, M, K) (*((VT*) map_get_(sizeof(KT), sizeof(VT), (M), &(KT){K})))
#define map_get_litk(M, K) map_get_litk_iso(typeof(*M->k), typeof(*M->v), K)
#define map_get_iso(KT, VT, M, K) (*((VT*) map_get_(sizeof(KT), sizeof(VT), (M), &(K))))
#define map_get(M, K) map_get_iso(typeof(*M->k), typeof(*M->v), M, K)
void *map_get_(size_t sizk, size_t sizv, Map m, void *k);

/*
** Then we need some allocators to build our maps.
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

#define map_free(M) (M) = map_free_((M))
Map map_free_(Map m);

/*
** And finally we need to implement the static structure accessor function.
** Exactly the same as `Dict`, `Seq`, and `Str`; header for performance.
*/

static inline
struct map_data *map(Map m) { 
  return ((struct map_data*) m) - 1;
}

#ifdef __clang__
  #pragma clang diagnostic pop // -Wcast-align
  #pragma clang diagnostic pop // -Wpadded
#endif

#endif // CIRCA_MAP_H
