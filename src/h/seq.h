/*
** seq.h | The Circa Library Set | Seq(T) Header
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

/*
** Dependencies
*/

/* Internal */

#include "core.h"

/*
** Types
*/

#define Seq(T) T*
typedef Seq(void) Seq;

struct seq_data {
  size_t cap, len;
  char data[];
};

/*
** Prototypes
*/

/* Accessors */

#define seq(S) seq_((S), CIRCA_DBGI)
struct seq_data *seq_(Seq s, CIRCA_ARGS);

#define seq_set_lit_ext_iso(T, S, A, V, E) (S) = seq_set_(sizeof(T), (S), (A), &(T){V}, (E), CIRCA_DBGI)
#define seq_set_lit_ext(S, A, V, E) seq_set_lit_ext_iso(typeof(*S), S, A, V, E)
#define seq_set_lit_iso(T, S, A, V) seq_set_lit_ext_iso(T, S, A, V, 0)
#define seq_set_lit(S, A, V) seq_set_lit_iso(typeof(*S), S, A, V)
#define seq_set_ext_iso(T, S, A, V, E) (S) = seq_set_(sizeof(T), (S), (A), &(V), (E), CIRCA_DBGI)
#define seq_set_ext(S, A, V, E) seq_set_ext_iso(typeof(*S), S, A, V, E)
#define seq_set_iso(T, S, A, V) seq_set_ext_iso(T, S, A, V, 0)
#define seq_set(S, A, V) seq_set_iso(typeof(*S), S, A, V)
Seq seq_set_(size_t siz, Seq s, size_t a, void *v, size_t ext, CIRCA_ARGS);

#define seq_get_iso(T, S, A) (*((T*) seq_get_(sizeof(T), (S), (A), CIRCA_DBGI)))
#define seq_get(S, A) seq_get_iso(typeof(*S), S, A)
void *seq_get_(size_t siz, Seq s, size_t a, CIRCA_ARGS);

/* Allocators */

#define seq_new_iso(T, C) seq_new_(sizeof(T), (C), CIRCA_DBGI)
#define seq_new(T, C) seq_new_iso(T, C)
_circa_alcs_ Seq seq_new_(size_t siz, size_t cap, CIRCA_ARGS);

#define seq_wrap_iso(T, L, P) seq_wrap_(sizeof(T), (L), (P), CIRCA_DBGI)
#define seq_wrap(L, P) seq_wrap_iso(typeof(*P), L, P)
#define seq_lit_iso(T, ...) seq_wrap_iso(T, sizeof((T[]){__VA_ARGS__}) / sizeof(T), &(T[]){__VA_ARGS__})
#define seq_lit(T, ...) seq_lit_iso(T, __VA_ARGS__)
_circa_alcs_ Seq seq_wrap_(size_t siz, size_t len, void *v, CIRCA_ARGS);

#define seq_rsz_iso(T, S, C) (S) = seq_rsz_(sizeof(T), (S), (C), CIRCA_DBGI)
#define seq_rsz(S, C) seq_rsz_iso(typeof(*S), S, C)
_circa_rets_ Seq seq_rsz_(size_t siz, Seq s, size_t cap, CIRCA_ARGS);

#define seq_rqr_ext_iso(T, S, C, E) (S) = seq_rqr_(sizeof(T), (S), (C), (E), CIRCA_DBGI)
#define seq_rqr_ext(S, C, E) seq_rqr_ext_iso(typeof(*S), S, C, E)
#define seq_rqr_iso(T, S, C) seq_rqr_ext_iso(T, S, C, 0)
#define seq_rqr(S, C) seq_rqr_iso(typeof(*S), S, C)
_circa_rets_ Seq seq_rqr_(size_t siz, Seq s, size_t cap, size_t ext, CIRCA_ARGS);

#define seq_shr_iso(T, S) (S) = seq_shr_(sizeof(T), (S), CIRCA_DBGI)
#define seq_shr(S) seq_shr_iso(typeof(*S), S)
_circa_rets_ Seq seq_shr_(size_t siz, Seq s, CIRCA_ARGS);

#define seq_del_iso(T, S) (S) = seq_del_(sizeof(T), (S), CIRCA_DBGI)
#define seq_del(S) seq_del_iso(typeof(*S), (S))
_circa_rets_ Seq seq_del_(size_t siz, Seq s, CIRCA_ARGS);

/* Stack Ops */

#define seq_push_lit_ext_iso(T, S, V, E) (S) = seq_push_(sizeof(T), (S), &(T){V}, E, CIRCA_DBGI)
#define seq_push_lit_ext(S, V, E) seq_push_lit_ext_iso(typeof(*S), S, V, E)
#define seq_push_lit_iso(T, S, V) seq_push_lit_ext_iso(T, S, V, 0)
#define seq_push_lit(S, V) seq_push_lit_iso(typeof(*S), S, V)
#define seq_push_ext_iso(T, S, V, E) (S) = seq_push_(sizeof(T), (S), &(V), E, CIRCA_DBGI)
#define seq_push_ext(S, V, E) seq_push_ext_iso(typeof(*S), S, V, E)
#define seq_push_iso(T, S, V) seq_push_ext_iso(T, S, V, 0)
#define seq_push(S, V) seq_push_iso(typeof(*S), S, V)
_circa_rets_ Seq seq_push_(size_t siz, Seq s, void *v, size_t ext, CIRCA_ARGS);

#define seq_tos_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 0, CIRCA_DBGI)))
#define seq_tos(S) seq_tos_iso(typeof(*S), S)
#define seq_pop_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 1, CIRCA_DBGI)))
#define seq_pop(S) seq_pop_iso(typeof(*S), S)
void *seq_pop_(size_t siz, Seq s, size_t n, CIRCA_ARGS);

#define seq_dup_iso(T, S) (S) = seq_dup(sizeof(T), S, CIRCA_DBGI)
#define seq_dup(S) seq_dup_iso(typeof(*S), S)
_circa_rets_ Seq seq_dup_(size_t siz, Seq s, CIRCA_ARGS);

/* Seq Ops */

// TODO: _ext variants
#define seq_cpy_iso(T, DST, SRC) (DST) = seq_cpy_(sizeof(T), (DST), (SRC), 0, CIRCA_DBGI)
#define seq_cpy(DST, SRC) seq_cpy_iso(typeof(*DST), DST, SRC)
_circa_rets_ Seq seq_cpy_(size_t siz, Seq dst, Seq src, size_t ext, CIRCA_ARGS);

// TODO: _ext variants
#define seq_cat_iso(T, DST, SRC) (DST) = seq_cat_(sizeof(T), (DST), (SRC), 0, CIRCA_DBGI)
#define seq_cat(DST, SRC) seq_cat_iso(typeof(*DST), DST, SRC)
_circa_rets_ Seq seq_cat_(size_t siz, Seq dst, Seq src, size_t ext, CIRCA_ARGS);

#define seq_rvs_iso(T, S) (S) = seq_rvs_(sizeof(T), (S), CIRCA_DBGI)
#define seq_rvs(S) seq_rvs_iso(typeof(*S), S)
_circa_rets_ Seq seq_rvs_(size_t siz, Seq s, CIRCA_ARGS);

#endif /* CIRCA_SEQ_H */
