/*
** set.h | The Circa Library Set | Dynamic sets.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SET_H
#define CIRCA_SET_H

/*
** Dependencies
*/

/* Circa */

#include "map.h"

/*
** Type Definitions
*/

#define Set(T) Map(T, bool)
typedef void *Set;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE MapData *set(Set e);

#define set_add_iso(T, E, K) (E) = set_add_(sizeof(T), (E), (K))
#define set_add(E, K) set_add_iso(typeof(E->k), E, K)
static inline CIRCA_RETURNS Set set_add_(size_t siz, Set e, void *k);

#define set_rem_iso(T, E, K) (E) = set_rem_(sizeof(T), (E), (K))
#define set_rem(E, K) set_rem_iso(typeof(E->k), E, K)
static inline bool set_rem_(size_t siz, Set e, void *k);

#define set_del_iso(T, E, K) (E) = set_del_(sizeof(T), (E), (K))
#define set_del(E, K) set_del_iso(typeof(E->k), E, K)
static inline bool set_del_(size_t siz, Set e, void *k);

#define set_has_iso(T, E, K) (E) = set_has_(sizeof(T), (E), (K))
#define set_del(E, K) set_del_iso(typeof(E->k), E, K)
static inline bool set_has_(size_t siz, Set e, void *k);

/* Allocators */

#define set_alloc_iso(T, C) set_alloc_(sizeof(T), (C))
#define set_alloc(T, C) set_alloc_iso(T, C)
static inline CIRCA_ALLOCS Set set_alloc_(size_t siz, size_t cap);

#define set_realloc_iso(T, E, C) (E) = set_realloc_(sizeof(T), (E), (C))
#define set_realloc(E, C) set_realloc_iso(typeof(E->k), E, C)
static inline CIRCA_RETURNS Set set_realloc_(size_t siz, Set e, size_t cap);

#define set_require_iso(T, E, C) (E) = set_require_(sizeof(T), (E), (C))
#define set_require(E, C) set_require_iso(typeof(E->k), E, C)
static inline CIRCA_RETURNS Set set_require_(size_t siz, Set e, size_t cap);

#define set_free_iso(T, E) (E) = set_free_(sizeof(T), (E))
#define set_free(E) set_free_iso(typeof(E->k), E)
static inline CIRCA_RETURNS Set set_free_(size_t siz, Set e);

/*
** Accessors
*/

static inline CIRCA_PURE
MapData *set(Set e) {
  return map(e);
}

static inline CIRCA_RETURNS
Set set_add_(size_t siz, Set e, void *k) {
  return map_set_(siz, 1, e, k, &(bool){true});
}

static inline
bool set_rem_(size_t siz, Set e, void *k) {
  if (!set_has_(siz, e, k))
    return false;
  map_set_(siz, 1, e, k, &(bool){false}); // This is safe because it will *NOT* realloc.
  return true;
}

static inline
bool set_del_(size_t siz, Set e, void *k) {
  return map_del_(siz, 1, e, k);
}

static inline
bool set_has_(size_t siz, Set e, void *k) {
  if (map_has_(siz, 1, e, k))
    return *(bool*) map_get_(siz, 1, e, k);
  else
    return false;
}

/*
** Allocators
*/

static inline CIRCA_ALLOCS
Set set_alloc_(size_t siz, size_t cap) {
  circa_guard (!siz || !cap)
    return (circa_throw(CE_ARG), NULL);
  return map_alloc_(siz, 1, cap);
}

static inline CIRCA_RETURNS
Set set_realloc_(size_t siz, Set e, size_t cap) {
  circa_guard (!siz || !e || !cap)
    return (circa_throw(CE_ARG), e);
  return map_realloc_(siz, 1, e, cap);
}

static inline CIRCA_RETURNS
Set set_require_(size_t siz, Set e, size_t cap) {
  circa_guard (!siz || !e || !cap)
    return (circa_throw(CE_ARG), e);
  return map_require_(siz, 1, e, cap);
}

static inline CIRCA_RETURNS
Set set_free_(size_t siz, Set e) {
  circa_guard (!siz)
    return (circa_throw(CE_ARG), e);
  return map_free_(siz, 1, e);
}

/*
** Iterators
*/

#define set_foreach_iso(T, E, K) \
map_foreach_iso(T, bool, E, K, V) \
if (V == true)
#define set_foreach(E, K) set_foreach_iso(typeof(E->k), E, K)


#endif // CIRCA_SET_H
