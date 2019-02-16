/*
** dict.h | The Circa Library Set | Dictionaries
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

#include "core.h"
#include "txt.h"

/*
** Under the hood `Dict(T)` is just `SeqMap(char, T)`, so the type definitions
** reflect that.
*/

#define Dict(T) SeqMap(char, T)
typedef void *Dict;

/*
** And now we need accessors.
*/

static inline struct seqmap_data *dict(Dict d);

#define dict_set_litv_iso(T, D, K, V) (D) = dict_set_(sizeof(T), (D), (K), &(T){V})
#define dict_set_litv(D, K, V) dict_set_litv_iso(typeof(*D->v), D, K, V)
#define dict_set_iso(T, D, K, V) (D) = dict_set_(sizeof(T), (D), (K), &(V))
#define dict_set(D, K, V) dict_set_iso(typeof(*D->v), D, K, V)
static inline Dict dict_set_(size_t siz, Dict d, char *k, void *v);

#define dict_has_iso(T, D, K) dict_has_(sizeof(T), (D), (K))
#define dict_has(D, K) dict_has_iso(typeof(*D->v), D, K)
static inline bool dict_has_(size_t siz, Dict d, char *k);

#define dict_get_iso(T, D, K) (*((T*) dict_get_(sizeof(T), (D), (K))))
#define dict_get(D, K) dict_get_iso(typeof(*D->v), D, K)
static inline void *dict_get_(size_t siz, Dict d, char *k);

/*
** And some allocators to build the dictionaries.
*/

#define dict_alloc_iso(T, C) dict_alloc_(sizeof(T), (C))
#define dict_alloc(T, C) dict_alloc_iso(T, C)
static inline Dict dict_alloc_(size_t siz, size_t cap);

#define dict_realloc_iso(T, D, C) (D) = dict_realloc_(sizeof(T), (D), (C))
#define dict_realloc(D, C) dict_realloc_iso(typeof(*D->v), D, C)
static inline Dict dict_realloc_(size_t siz, Dict d, size_t cap);

#define dict_require_iso(T, D, C) (D) = dict_require_(sizeof(T), (D), (C))
#define dict_require(D, C) dict_require_iso(typeof(*D->v), D, C)
static inline Dict dict_require_(size_t siz, Dict d, size_t cap);

#define dict_free_iso(T, D) (D) = dict_free_((D))
#define dict_free(D) dict_free_iso(_, D)
static inline Dict dict_free_(Dict d);

/*
** And now we just wrap the accessors from SeqMap with some changes.
** TODO: Find some way of getting around this constant Txt allocation.
**       It's definitely not cheap.
*/

static inline
struct seqmap_data *dict(Dict d) {
  return seqmap(d);
}

static inline
Dict dict_set_(size_t siz, Dict d, char *k, void *v) {
  Txt t = txt_wrap(k, strlen(k));
  d = seqmap_set_(1, siz, d, k, v);
  txt_free_(t);
  return d;
}

static inline
bool dict_has_(size_t siz, Dict d, char *k) {
  Txt t = txt_wrap(k, strlen(k));
  bool has = seqmap_has_(1, siz, d, k);
  txt_free_(t);
  return has;
}

static inline
void *dict_get_(size_t siz, Dict d, char *k) {
  Txt t = txt_wrap(k, strlen(k));
  void *p = seqmap_get_(1, siz, d, k);
  txt_free_(t);
  return p;
}

/*
** Same with the allocators.
*/

static inline
Dict dict_alloc_(size_t siz, size_t cap) {
  return seqmap_alloc_(1, siz, cap);
}

static inline
Dict dict_realloc_(size_t siz, Dict d, size_t cap) {
  return seqmap_realloc_(1, siz, d, cap);
}

static inline
Dict dict_require_(size_t siz, Dict d, size_t cap) {
  return seqmap_require_(1, siz, d, cap);
}

static inline
Dict dict_free_(Dict d) {
  return seqmap_free_(d);
}

#endif // CIRCA_DICT_H
