/*
** set.h | The Circa Library Set | Dynamic Sets
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SET_H
#define CIRCA_SET_H

/*
** Dependencies
*/

#include "core.h"
#include "bits.h"
#include "map.h"

/*
** Type Definitions
*/

#define Set(T) Map(T, bool)
typedef void *Set;

/*
** Accessors
*/

static inline struct map_data *set(Set e);

#define set_add_iso(T, E, K) (E) = set_add_(sizeof(T), (E), (K))
#define set_add(E, K) set_add_iso(typeof(*(*E->k)), E, K)
static inline Set set_add_(size_t siz, Set e, void *k);

#define set_rem_iso(T, E, K) (E) = set_rem_(sizeof(T), (E), (K))
#define set_rem(E, K) set_rem_iso(typeof(*(*E->k)), E, K)
static inline Set set_rem_(size_t siz, Set e, void *k);

#define set_del_iso(T, E, K) (E) = set_del_(sizeof(T), (E), (K))
#define set_del(E, K) set_del_iso(typeof(*(*E->k)), E, K)
static inline Set set_del_(size_t siz, Set e, void *k);

#define set_has_iso(T, E, K) set_has_(sizeof(T), (E), (K))
#define set_has(E, K) set_has_iso(typeof(*(*E->k)), E, K)
static inline bool set_has_(size_t siz, Set e, void *k);

/*
** Allocators
*/

#define set_alloc_iso(T, C) set_alloc_(sizeof(T), (C))
#define set_alloc(T, C) set_alloc_iso(T, C)
static inline Set set_alloc_(size_t siz, size_t cap);

#define set_realloc_iso(T, E, C) (E) = set_realloc_(sizeof(T), (E), (C))
#define set_realloc(E, C) set_realloc_iso(typeof(*E->k), E, C)
static inline Set set_realloc_(size_t siz, Set e, size_t cap);

#define set_require_iso(T, E, C) (E) = set_require_(sizeof(T), (E), (C))
#define set_require(E, C) set_require_iso(typeof(*E->k), E, C)
static inline Set set_require_(size_t siz, Set e, size_t cap);

#define set_free_iso(T, E) (E) = set_free_((E))
#define set_free(E) set_free_iso(_, E)
static inline Set set_free_(Set e);

/*
** Accessors Implementation
*/

static inline
struct map_data *set(Set e) {
  return map(e);
}

static inline
Set set_add_(size_t siz, Set e, void *k) {
  static bool set_add_true = true;
  return map_set_(siz, 1, e, k, &set_add_true);
}

static inline
Set set_rem_(size_t siz, Set e, void *k) {
  static bool set_add_false = false;
  return map_set_(siz, 1, e, k, &set_add_false);
}

static inline
Set set_del_(size_t siz, Set e, void *k) {
  return set_rem_(siz, e, k); // TODO: Remap to map_del once implemented.
}

static inline
bool set_has_(size_t siz, Set e, void *k) {
  if (map_has_(siz, 1, e, k))
    return *(bool*) map_get_(siz, 1, e, k);
  else
    return false;
}

/*
** Allocators Implementation
*/

static inline
Set set_alloc_(size_t siz, size_t cap) {
  return map_alloc_(siz, 1, cap);
}

static inline
Set set_realloc_(size_t siz, Set e, size_t cap) {
  return map_realloc_(siz, 1, e, cap);
}

static inline
Set set_require_(size_t siz, Set e, size_t cap) {
  return map_require_(siz, 1, e, cap);
}

static inline
Set set_free_(Set e) {
  return map_free_(e);
}

#endif // CIRCA_SET_H
