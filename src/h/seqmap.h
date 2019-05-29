/*
** seqmap.h | The Circa Library Set | Dynamic seq-keyed maps.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQMAP_H
#define CIRCA_SEQMAP_H

/*
** Dependencies
*/

/* Circa */

#include "seq.h"

/*
** Type Definitions
*/

typedef struct {
  size_t cap;
  size_t len;
  size_t *probe;
  char   *data;
  Seq     key[];
} SeqMapData;

#define SeqMap(K, V) struct { K k; V v; } *
typedef void *SeqMap;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE SeqMapData *seqmap(SeqMap sm);

#define seqmap_set_iso(KT, VT, SM, K, V) (SM) = seqmap_set_(sizeof(KT), sizeof(VT), (SM), (K), (V))
#define seqmap_set(SM, K, V) seqmap_set_iso(typeof((SM)->k), typeof(SM->v), SM, K, V)
CIRCA CIRCA_RETURNS SeqMap seqmap_set_(size_t sizk, size_t sizv, SeqMap sm, Seq k, void *v);

#define seqmap_del_iso(KT, VT, SM, K) seqmap_del_(sizeof(KT), sizeof(VT), (SM), (K))
#define seqmap_del(SM, K) seqmap_del_iso(typeof((SM)->k), typeof(SM->v), SM, K)
CIRCA bool seqmap_del_(size_t sizk, size_t sizv, SeqMap sm, Seq k);

#define seqmap_has_iso(KT, VT, SM, K) seqmap_has_(sizeof(KT), sizeof(VT), (SM), (K))
#define seqmap_has(SM, K) seqmap_has_iso(typeof((SM)->k), typeof(SM->v), SM, K)
CIRCA bool seqmap_has_(size_t sizk, size_t sizv, SeqMap sm, Seq k);

#define seqmap_get_iso(KT, VT, SM, K) (*((VT*) seqmap_get_(sizeof(KT), sizeof(VT), (SM), (K))))
#define seqmap_get(SM, K) seqmap_get_iso(typeof((SM)->k), typeof(SM->v), SM, K)
CIRCA void *seqmap_get_(size_t sizk, size_t sizv, SeqMap sm, Seq k);

/* Allocators */

#define seqmap_alloc_iso(TK, TV, C) seqmap_alloc_(sizeof(TK), sizeof(TV), (C))
#define seqmap_alloc(TK, TV, C) seqmap_alloc_iso(TK, TV, C)
CIRCA CIRCA_ALLOCS SeqMap seqmap_alloc_(size_t sizk, size_t sizv, size_t cap);

#define seqmap_realloc_iso(TK, TV, SM, C) (SM) = seqmap_realloc_(sizeof(KT), sizeof(VT), (SM), (C))
#define seqmap_realloc(SM, C) seqmap_realloc_iso(typeof((SM)->k), typeof(SM->v), SM, C)
CIRCA CIRCA_RETURNS SeqMap seqmap_realloc_(size_t sizk, size_t sizv, SeqMap sm, size_t cap);

#define seqmap_free_iso(TK, TV, SM) (SM) = seqmap_free_(sizeof(TK), sizeof(TV), (SM))
#define seqmap_free(SM) seqmap_free_iso(typeof((SM)->k), typeof((SM)->v), SM)
CIRCA CIRCA_RETURNS SeqMap seqmap_free_(size_t sizk, size_t sizv, SeqMap sm);

/*
** Accessors
*/

static inline CIRCA_PURE
SeqMapData *seqmap(SeqMap sm) {
  return ((SeqMapData*) sm) - 1;
}

#endif // CIRCA_SEQ_H
