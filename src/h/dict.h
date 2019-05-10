/*
** dict.h | The Circa Library Set | Dictionaries
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

#include "core.h"
#include "txt.h"

/*
** Type Definitions
*/

#define Dict(T) SeqMap(char, T)
typedef void *Dict;

/*
** Accessors
*/

static inline struct seqmap_data *dict(Dict d);

#define dict_set_iso(T, D, K, V) (D) = dict_set_(sizeof(T), (D), (K), (V))
#define dict_set(D, K, V) dict_set_iso(typeof(*D->v), D, K, V)
static inline Dict dict_set_(size_t siz, Dict d, char *k, void *v);

#define dict_has_iso(T, D, K) dict_has_(sizeof(T), (D), (K))
#define dict_has(D, K) dict_has_iso(typeof(*D->v), D, K)
static inline bool dict_has_(size_t siz, Dict d, char *k);

#define dict_del_iso(T, D, K) dict_del_(sizeof(T), (D), (K))
#define dict_del(D, K) dict_del_iso(typeof(*D->v), D, K)
static inline bool dict_del_(size_t siz, Dict d, char *k);

#define dict_get_iso(T, D, K) (*((T*) dict_get_(sizeof(T), (D), (K))))
#define dict_get(D, K) dict_get_iso(typeof(*D->v), D, K)
static inline void *dict_get_(size_t siz, Dict d, char *k);

/*
** Allocators
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
** Accessors Implementation
*/

// TODO: Find some way of getting around this constant Txt allocation.
//       It's definitely not cheap.

static inline
struct seqmap_data *dict(Dict d) {
  return seqmap(d);
}

static inline
Dict dict_set_(size_t siz, Dict d, char *k, void *v) {
  Txt t = txt_wrap(k, strlen(k));
  d = seqmap_set_(1, siz, d, t, v);
  txt_free_(t);
  return d;
}

static inline
bool dict_has_(size_t siz, Dict d, char *k) {
  Txt t = txt_wrap(k, strlen(k));
  bool has = seqmap_has_(1, siz, d, t);
  txt_free_(t);
  return has;
}

static inline
bool dict_del_(size_t siz, Dict d, char *k) {
  Txt t = txt_wrap(k, strlen(k));
  bool del = seqmap_del_(1, siz, d, t);
  txt_free_(t);
  return del;
}

static inline
void *dict_get_(size_t siz, Dict d, char *k) {
  Txt t = txt_wrap(k, strlen(k));
  void *p = seqmap_get_(1, siz, d, t);
  txt_free_(t);
  return p;
}

/*
** Allocators Implementation
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

/*
** Iterators
*/

#define dict_foreach_iso(TK, TV, D, K, V) \
for (size_t I = 0, J = 0; I < dict(D)->cap; I++, J = 0) \
if (dict(D)->key[I]) \
for (TK K = *(TK*) (dict(D)->key + I); J != 1; J = 1) \
for (TV V = *(TV*) (dict(D)->data + (I * sizeof(TV))); J != 1; J = 1)
#define dict_foreach(D, K, V) dict_foreach_iso(typeof(*D->k), typeof(*D->v), D, K, V)

#endif // CIRCA_DICT_H
