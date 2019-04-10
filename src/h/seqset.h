/*
** seqset.h | The Circa Library Set | Generic Seq-Keyed Sets
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQSET_H
#define CIRCA_SEQSET_H

#include "core.h"
#include "bits.h"
#include "seqmap.h"

/*
** Under the hood `SeqSet(T)` is just `SeqMap(T, bool)`, so the type definitions
** reflect that.
*/ 

#define SeqSet(T) SeqMap(T, bool)
typedef void *SeqSet;

/*
** And then we need accessors.
*/

static inline struct seqmap_data *seqset(SeqSet se);

#define seqset_add_iso(T, SE, K) (SE) = seqset_add_(sizeof(T), (SE), (K))
#define seqset_add(SE, K) seqset_add_iso(typeof(*(*E->k)), SE, K)
static inline SeqSet seqset_add_(size_t siz, SeqSet se, Seq k);

#define seqset_rem_iso(T, SE, K) (SE) = seqset_rem_(sizeof(T), (SE), (K))
#define seqset_rem(SE, K) seqset_rem_iso(typeof(*(*E->k)), SE, K)
static inline SeqSet seqset_rem_(size_t siz, SeqSet se, Seq k);

#define seqset_del_iso(T, SE, K) (SE) = seqset_del_(sizeof(T), (SE), (K))
#define seqset_del(SE, K) seqset_del_iso(typeof(*(*E->k)), SE, K)
static inline SeqSet seqset_del_(size_t siz, SeqSet se, Seq k);

#define seqset_has_iso(T, SE, K) seqset_has_(sizeof(T), (SE), (K))
#define seqset_has(SE, K) seqset_has_iso(typeof(*(*E->k)), SE, K)
static inline bool seqset_has_(size_t siz, SeqSet se, Seq k);

/*
** And then some allocators.
*/

#define seqset_alloc_iso(T, C) seqset_alloc_(sizeof(T), (C))
#define seqset_alloc(T, C) seqset_alloc_iso(T, C)
static inline SeqSet seqset_alloc_(size_t siz, size_t cap);

#define seqset_realloc_iso(T, SE, C) (SE) = seqset_realloc_(sizeof(T), (SE), (C))
#define seqset_realloc(SE, C) seqset_realloc_iso(typeof(*(*E->k)), SE, K)
static inline SeqSet seqset_realloc_(size_t siz, SeqSet se, size_t cap);

#define seqset_require_iso(T, SE, C) (SE) = seqset_require_(sizeof(T), (SE), (C))
#define seqset_require(SE, C) seqset_require_iso(typeof(*(*E->k)), SE, K)
static inline SeqSet seqset_require_(size_t siz, SeqSet se, size_t cap);

#define seqset_free_iso(T, SE) (SE) = seqset_free_((SE))
#define seqset_free(SE) seqset_free_iso(_, SE)
static inline SeqSet seqset_free_(SeqSet se);

/*
** Accessors
*/

static inline
struct seqmap_data *seqset(SeqSet se) {
  return seqmap(se);
}

static inline
SeqSet seqset_add_(size_t siz, SeqSet se, Seq k) {
  static bool seqset_add_true = true;
  return seqmap_set_(siz, 1, se, k, &seqset_add_true);
}

static inline
SeqSet seqset_rem_(size_t siz, SeqSet se, Seq k) {
  static bool seqset_add_false = false;
  return seqmap_set_(siz, 1, se, k, &seqset_add_false);
}

static inline
SeqSet seqset_del_(size_t siz, SeqSet se, Seq k) {
  return seqset_rem_(siz, se, k); // TODO: Remap to seqmap_del once implemented.
}

static inline
bool seqset_has_(size_t siz, SeqSet se, Seq k) {
  if (seqmap_has_(siz, 1, se, k))
    return *(bool*) seqmap_get_(siz, 1, se, k);
  else
    return false;
}

/*
** Allocators
*/

static inline
SeqSet seqset_alloc_(size_t siz, size_t cap) {
  return seqmap_alloc_(siz, 1, cap);
}

static inline
SeqSet seqset_realloc_(size_t siz, SeqSet se, size_t cap) {
  return seqmap_realloc_(siz, 1, se, cap);
}

static inline
SeqSet seqset_require_(size_t siz, SeqSet se, size_t cap) {
  return seqmap_require_(siz, 1, se, cap);
}

static inline
SeqSet seqset_free_(SeqSet se) {
  return seqmap_free_(se);
}

#endif // CIRCA_SEQSET_H
