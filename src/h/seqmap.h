/*
** seqmap.h | The Circa Library Set | Dual-Generic Seq-Keyed Maps
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQMAP_H
#define CIRCA_SEQMAP_H

/*
** Dependencies
*/

#include "core.h"
#include "bits.h"
#include "seq.h"
#include "map.h"

/*
** Type Definitions
*/

struct seqmap_data {
  size_t  cap;
  size_t  len;
  size_t *probe;
  char   *data;
  Seq     key[];
};

#define SeqMap(K, V) Map(Seq(K), V)
typedef void *SeqMap;

/*
** Accessors
*/

static inline struct seqmap_data *seqmap(SeqMap sm);

#define seqmap_set_iso(KT, VT, SM, K, V) (SM) = seqmap_set_(sizeof(KT), sizeof(VT), (SM), (K), (V))
#define seqmap_set(SM, K, V) seqmap_set_iso(typeof(*(*SM->k)), typeof(*SM->v), SM, K, V)
SeqMap seqmap_set_(size_t sizk, size_t sizv, SeqMap sm, Seq k, void *v);

#define seqmap_has_iso(KT, VT, SM, K) seqmap_has_(sizeof(KT), sizeof(VT), (SM), (K))
#define seqmap_has(SM, K) seqmap_has_iso(typeof(*(*SM->k)), typeof(*SM->v), SM, K)
bool seqmap_has_(size_t sizk, size_t sizv, SeqMap sm, Seq k);

#define seqmap_get_iso(KT, VT, SM, K) (*((VT*) seqmap_get_(sizeof(KT), sizeof(VT), (SM), (K))))
#define seqmap_get(SM, K) seqmap_get_iso(typeof(*(*SM->k)), typeof(*SM->v), SM, K)
void *seqmap_get_(size_t sizk, size_t sizv, SeqMap sm, Seq k);

/*
** Allocators
*/

#define seqmap_alloc_iso(KT, VT, C) seqmap_alloc_(sizeof(KT), sizeof(VT), (C))
#define seqmap_alloc(KT, VT, C) seqmap_alloc_iso(KT, VT, C)
SeqMap seqmap_alloc_(size_t sizk, size_t sizv, size_t cap);

#define seqmap_realloc_iso(KT, VT, SM, C) (SM) = seqmap_realloc_(sizeof(KT), sizeof(VT), (SM), (C))
#define seqmap_realloc(SM, C) seqmap_realloc_iso(typeof(*(*SM->k)), typeof(*SM->v), SM, C)
SeqMap seqmap_realloc_(size_t sizk, size_t sizv, SeqMap sm, size_t cap);

#define seqmap_require_iso(KT, VT, SM, C) (SM) = seqmap_require_(sizeof(KT), sizeof(VT), (SM), (C))
#define seqmap_require(SM, C) seqmap_require_iso(typeof(*(*SM->k)), typeof(*SM->v), SM, C)
SeqMap seqmap_require_(size_t sizk, size_t sizv, SeqMap sm, size_t cap);

#define seqmap_free_iso(KT, VT, SM) (SM) = seqmap_free_((SM))
#define seqmap_free(SM) seqmap_free_iso(_, _, SM)
SeqMap seqmap_free_(SeqMap sm);

/*
** Accessors Implementation
*/

static inline
struct seqmap_data *seqmap(SeqMap sm) {
  return ((struct seqmap_data*) sm) - 1;
}

#endif // CIRCA_SEQMAP_H
