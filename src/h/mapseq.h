/*
** mapseq.h | The Circa Library Set | Dynamic seq-valued maps.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_MAPSEQ_H
#define CIRCA_MAPSEQ_H

/*
** Dependencies
*/

/* Circa */

#include "seq.h"

/*
** Type Definitions
*/

typedef struct {
  size_t  cap;
  size_t  len;
  size_t *probe;
  Seq *data;
  char key[];
} MapSeqData;

#define MapSeq(K, V) struct { K k; V v; } *
typedef void *MapSeq;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE MapSeqData *mapseq(MapSeq m);

#define mapseq_set_iso(KT, VT, MS, K, V) (MS) = mapseq_set_(sizeof(KT), sizeof(VT), (MS), (K), (V))
#define mapseq_set(MS, K, V) mapseq_set_iso(typeof((MS)->k), typeof((MS)->v), MS, K, V)
CIRCA CIRCA_RETURNS MapSeq mapseq_set_(size_t sizk, size_t sizv, MapSeq ms, void *k, Seq v);

#define mapseq_del_iso(KT, VT, MS, K) mapseq_del_(sizeof(KT), sizeof(VT), (MS), (K))
#define mapseq_del(MS, K) mapseq_del_iso(typeof((MS)->k), typeof((MS)->v), MS, K)
CIRCA bool mapseq_del_(size_t sizk, size_t sizv, MapSeq ms, void *k);

#define mapseq_has_iso(KT, VT, MS, K) mapseq_has_(sizeof(KT), sizeof(VT), (MS), (K))
#define mapseq_has(MS, K) mapseq_has_iso(typeof((MS)->k), typeof((MS)->v), MS, K)
CIRCA bool mapseq_has_(size_t sizk, size_t sizv, MapSeq ms, void *k);

#define mapseq_get_iso(KT, VT, MS, K) mapseq_get_(sizeof(KT), sizeof(VT), (MS), (K))
#define mapseq_get(MS, K) mapseq_get_iso(typeof((MS)->k), typeof((MS)->v), MS, K)
CIRCA Seq mapseq_get_(size_t sizk, size_t sizv, MapSeq ms, void *k);

/* Allocators */

#define mapseq_alloc_iso(KT, VT, C) mapseq_alloc_(sizeof(KT), sizeof(VT), (C))
#define mapseq_alloc(KT, VT, C) mapseq_alloc_iso(KT, VT, C)
CIRCA CIRCA_ALLOCS MapSeq mapseq_alloc_(size_t sizk, size_t sizv, size_t cap);

#define mapseq_realloc_iso(KT, VT, MS, C) (MS) = mapseq_realloc_(sizeof(KT), sizeof(VT), (MS), (C))
#define mapseq_realloc(MS, C) mapseq_realloc_iso(typeof((MS)->k), typeof((MS)->v), MS, C)
CIRCA CIRCA_RETURNS MapSeq mapseq_realloc_(size_t sizk, size_t sizv, MapSeq ms, size_t cap);

#define mapseq_require_iso(KT, VT, MS, C) (MS) = mapseq_require_(sizeof(KT), sizeof(VT), (MS), (C))
#define mapseq_require(MS, C) mapseq_require_iso(typeof((MS)->k), typeof((MS)->v), MS, C)
CIRCA CIRCA_RETURNS MapSeq mapseq_require_(size_t sizk, size_t sizv, MapSeq ms, size_t cap);

#define mapseq_free_iso(KT, VT, MS) (MS) = mapseq_free_(sizeof(KT), sizeof(VT), (MS))
#define mapseq_free(MS) mapseq_free_iso(typeof((MS)->k), typeof((MS)->v), MS)
CIRCA CIRCA_RETURNS MapSeq mapseq_free_(size_t sizk, size_t sizv, MapSeq ms);

/*
** Function Definitions
*/

/* Accessors */

static inline CIRCA_PURE
MapSeqData *mapseq(MapSeq ms) {
  circa_guard (!ms)
    return (circa_throw(CE_ARG), ms);
  return ((MapSeqData*) ms) - 1;
}

/*
** Iterators
*/

#define mapseq_foreach_iso(TK, TV, MS, K, V) \
for (size_t I = 0, J = 0; I < mapseq(MS)->cap; I++, J = 0) \
if (mapseq(MS)->data[I] != NULL) \
for (TK K = *(TK*) (mapseq(MS)->key + (I * sizeof(TK))); J != 1; J = 1) \
for (Seq(TV) V = mapseq(MS)->data[I]; J != 1; J = 1)
#define mapseq_foreach(MS, K, V) mapseq_foreach_iso(typeof((MS)->k), typeof((MS)->v), MS, K, V)

#ifdef CIRCA_HEADER_ONLY
  #include "../c/mapseq.c"
#endif

#endif // CIRCA_MAPSEQ_H
