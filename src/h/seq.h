/*
** seq.h | The Circa Library Set | Dynamic Sequences
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

/*
** Dependencies
*/

#include "core.h"
#include "slice.h"

/*
** Type Definitions
*/

struct seq_data {
  size_t cap;
  size_t len;
  char data[];
};

#define Seq(T) T*
typedef Seq(void) Seq;

/*
** Accessors
*/

static inline struct seq_data *seq(Seq s);

#define seq_clear_iso(T, S) seq_clear_(sizeof(T), (S))
#define seq_clear(S) seq_clear_iso(typeof(*S), S)
void seq_clear_(size_t siz, Seq s);

#define seq_set_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), (V))
#define seq_set(S, A, V) seq_set_iso(typeof(*S), S, A, V)
Seq seq_set_(size_t siz, Seq s, size_t a, void *v);

#define seq_has_iso(T, S, A) seq_has_(sizeof(T), (S), (A))
#define seq_has(S, A) seq_has_iso(typeof(*S), S, A)
bool seq_has_(size_t siz, Seq s, size_t a);

#define seq_get_iso(T, S, A) (*((T*) seq_get_(sizeof(T), (S), (A))))
#define seq_get(S, A) seq_get_iso(typeof(*S), S, A)
void *seq_get_(size_t siz, Seq s, size_t a);

/*
** Allocators
*/

#define seq_alloc_iso(T, C) seq_alloc_(sizeof(T), C)
#define seq_alloc(T, C) seq_alloc_iso(T, C)
Seq seq_alloc_(size_t siz, size_t cap);

#define seq_realloc_iso(T, S, C) (S) = seq_realloc_(sizeof(T), (S), (C))
#define seq_realloc(S, C) seq_realloc_iso(typeof(*S), S, C)
Seq seq_realloc_(size_t siz, Seq s, size_t cap);

#define seq_require_iso(T, S, C) (S) = seq_require_(sizeof(T), (S), (C))
#define seq_require(S, C) seq_require_iso(typeof(*S), S, C)
Seq seq_require_(size_t siz, Seq s, size_t cap);

#define seq_from_iso(T, S) seq_wrap_(sizeof(T), (S), seq(S)->len)
#define seq_from(S) seq_wrap_(typeof(*S), S)
#define seq_wrap_iso(T, N, V) seq_wrap_(sizeof(T), (N), (V))
#define seq_wrap(N, V) seq_wrap_iso(typeof(*V), N, V)
Seq seq_wrap_(size_t siz, size_t n, void *v);

#define seq_free_iso(T, S) seq_free(S)
#define seq_free(S) (S) = seq_free_((S))
Seq seq_free_(Seq s);

/*
** Sequence Functions
*/

#define seq_cpy_iso(T, A, B) (A) = seq_cpy_(sizeof(T), (A), (B))
#define seq_cpy(A, B) seq_cpy_iso(typeof(*A), A, B)
Seq seq_cpy_(size_t siz, Seq a, Seq b);

#define seq_cpy_slice_iso(T, A, B, S) (A) = seq_cpy_slice_(sizeof(T), (A), (B), (S))
#define seq_cpy_slice(A, B, S) seq_cpy_slice_iso(typeof(*A), A, B, S)
Seq seq_cpy_slice_(size_t siz, Seq a, Seq b, Slice s);

/*
** Stack Functions
*/

#define seq_push_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), (V))
#define seq_push(S, V) seq_push_iso(typeof(*S), S, V)
Seq seq_push_(size_t siz, Seq s, void *v);

#define seq_tos_ptr_iso(T, S) ((T*) seq_pop_(sizeof(T), (S), 0))
#define seq_tos_ptr(S) seq_tos_ptr_iso(typeof(*S), S)
#define seq_tos_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 0)))
#define seq_tos(S) seq_tos_iso(typeof(*S), S)
#define seq_pop_iso(T, S) (*((T*) seq_pop_(sizeof(T), (S), 1)))
#define seq_pop(S) seq_pop_iso(typeof(*S), S)
void *seq_pop_(size_t siz, Seq s, size_t n);

#define seq_pull_iso(T, S) (*((T*) seq_pull_(sizeof(T), (S))))
#define seq_pull(S) seq_pull_iso(typeof(*S), S)
void *seq_pull_(size_t siz, Seq s);

/*
** Comparison Functions
*/

#define seq_cmp_iso(T, A, B) seq_cmp_(sizeof(T), (A), (B))
#define seq_cmp(A, B) seq_cmp_iso(typeof(*A), A, B)
bool seq_cmp_(size_t siz, Seq a, Seq b);

#define seq_cmp_len_iso(T, A, B, L) seq_cmp_len_(sizeof(T), (A), (B), (L))
#define seq_cmp_len(A, B, L) seq_cmp_len_iso(typeof(*A), A, B, L)
bool seq_cmp_len_(size_t siz, Seq a, Seq b, size_t len);

#define seq_cmp_slice_iso(T, A, SA, B, SB) seq_cmp_slice_(sizeof(T), (A), (SA), (B), (SB))
#define seq_cmp_slice(A, SA, B, SB) seq_cmp_slice_iso(typeof(*A), A, SA, B, SB)
bool seq_cmp_slice_(size_t siz, Seq a, Slice sa, Seq b, Slice sb);

/*
** Accessors Implementation
*/

static inline
struct seq_data *seq(Seq s) {
  return s ? ((struct seq_data*) s) - 1 : s;
}

/*
** Iterators
*/

#define seq_foreach_iso(T, S, V) \
if (S) \
for (size_t I = 0, J = 0; I < seq(S)->len; I++, J = 0) \
for (T V = seq_get(S, I); J != 1; J = 1)
#define seq_foreach(S, V) seq_foreach_iso(typeof(*S), S, V)

#define seq_map_iso(T, A, F, B)              \
do {                                         \
  T FV;                                      \
  seq_require_iso(T, A, seq(B)->len);        \
  seq(A)->len = 0;                           \
  for (size_t I = 0; I < seq(B)->len; I++) { \
    FV = F(seq_get_iso(T, B, I));            \
    seq_push_iso(T, A, FV);                  \
  }                                          \
} while (0)
#define seq_map(A, F, B) seq_map_iso(typeof(*B), A, F, B)

#define seq_filter_iso(T, A, F, B)            \
do {                                          \
  T V;                                        \
  seq_require_iso(T, A, seq(B)->len);         \
  seq(A)->len = 0;                            \
  for (size_t I = 0; I < seq(B)->len; I++) {  \
    V = seq_get_iso(T, B, I);                 \
    if (F(V))                                 \
      seq_push_iso(T, A, V);                  \
  }                                           \
} while (0)
#define seq_filter(A, F, B) seq_filter_iso(typeof(*B), A, F, B)

#define seq_reduce_iso(T, A, F, B)         \
do {                                       \
  for (size_t I = 0; I < seq(B)->len; I++) \
    A = F(A, seq_get_iso(T, B, I));        \
} while (0)
#define seq_reduce(A, F, B) seq_reduce_iso(typeof(*B), A, F, B)

#define seq_apply_iso(T, S, F)               \
do {                                         \
  T FV;                                      \
  for (size_t I = 0; I < seq(S)->len; I++) { \
    FV = F(seq_get_iso(T, S, I));            \
    seq_set_iso(T, S, I, FV);                \
  }                                          \
} while (0)
#define seq_apply(S, F) seq_apply_iso(typeof(*S), S, F)

#define seq_keep_iso(T, S, F)                                        \
do {                                                                 \
  size_t LEN = seq(S)->len;                                          \
  T V;                                                               \
  for (size_t I = 0; I < LEN; I++) {                                 \
    V = seq_get_iso(T, S, I);                                        \
    if (F(V))                                                        \
      seq_push_iso(T, S, V);                                         \
  }                                                                  \
  seq(S)->len -= LEN;                                                \
  memcpy(S, ((char*) S) + LEN * sizeof(T), seq(S)->len * sizeof(T)); \
} while (0)
#define seq_keep(S, F) seq_keep_iso(typeof(*S), S, F)

#define seq_collapse_iso(T, S, F) \
do {                              \
  T VAL = {0};                    \
  seq_reduce_iso(T, VAL, F, S);   \
  seq(S)->len = 1;                \
  seq_set_iso(T, S, 0, &VAL);     \
} while (0)
#define seq_collapse(S, F) seq_collapse_iso(typeof(*S), S, F)

#define seq_do_iso(T, S, F)                \
do {                                       \
  for (size_t I = 0; I < seq(S)->len; I++) \
    F(seq_get_iso(T, S, I));               \
} while (0)
#define seq_do(S, F) seq_do_iso(typeof(*S), S, F)

#endif // CIRCA_SEQ_H
