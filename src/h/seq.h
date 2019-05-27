/*
** seq.h | The Circa Library Set | Dynamic sequences.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

/*
** Dependencies
*/

/* Standard */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Circa */

#include "core.h"
#include "debug.h"
#include "slice.h"

/*
** Type Definitions
*/

typedef struct {
  size_t cap;
  size_t len;
  char data[];
} SeqData;

#define Seq(T) T*
typedef Seq(void) Seq;

/*
** Function Declarations
*/

/* Accessors */

static inline CIRCA_PURE CIRCA_NONNULL(1) SeqData *seq(Seq s);

#define seq_set_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), (V))
#define seq_set(S, A, V) seq_set_iso(typeof(*S), S, A, V)
CIRCA CIRCA_RETURNS Seq seq_set_(size_t siz, Seq s, size_t a, void *v);

#define seq_get_ptr_iso(T, S, A) ((T*) seq_get_(sizeof(T), (S), (A)))
#define seq_get_ptr(S, A) seq_get_ptr_iso(typeof(*S), S, A)
#define seq_get_iso(T, S, A) (*seq_get_ptr_iso(T, S, A))
#define seq_get(S, A) seq_get_iso(typeof(*S), S, A)
CIRCA void *seq_get_(size_t siz, Seq s, size_t a);

/* Allocators */

#define seq_alloc_iso(T, C) seq_alloc_(sizeof(T), (C))
#define seq_alloc(T, C) seq_alloc_iso(T, C)
CIRCA CIRCA_ALLOCS Seq seq_alloc_(size_t siz, size_t cap);

#define seq_realloc_iso(T, S, C) (S) = seq_realloc_(sizeof(T), (S), (C))
#define seq_realloc(S, C) seq_realloc_iso(typeof(*S), (S), (C))
CIRCA CIRCA_RETURNS Seq seq_realloc_(size_t siz, Seq s, size_t cap);

#define seq_require_iso(T, S, C) (S) = seq_require_(sizeof(T), (S), (C))
#define seq_require(S, C) seq_require_iso(typeof(*S), (S), (C))
CIRCA CIRCA_RETURNS Seq seq_require_(size_t siz, Seq s, size_t cap);

#define seq_free_iso(T, S) (S) = seq_free_(sizeof(T), (S))
#define seq_free(S) seq_free_iso(typeof(*S), S)
CIRCA CIRCA_RETURNS Seq seq_free_(size_t siz, Seq s);

/* Sequence Operations */

#define seq_cpy_iso(T, DST, SRC) (DST) = seq_cpy_(sizeof(T), (DST), (SRC))
#define seq_cpy(DST, SRC) seq_cpy_iso(typeof(*DST), DST, SRC)
CIRCA CIRCA_RETURNS Seq seq_cpy_(size_t siz, Seq dst, Seq src);

#define seq_cpy_slice_iso(T, DST, SRC, SLICE) (DST) = seq_cpy_slice_(sizeof(T), (DST), (SRC), (SLICE))
#define seq_cpy_slice(DST, SRC, SLICE) seq_cpy_slice_iso(typeof(*DST), DST, SRC, SLICE)
CIRCA CIRCA_RETURNS Seq seq_cpy_slice_(size_t siz, Seq dst, Seq src, Slice slice);

/* Stack Operations */

#define seq_push_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), (V))
#define seq_push(S, V) seq_push_iso(typeof(*S), (S), (V))
CIRCA CIRCA_RETURNS Seq seq_push_(size_t siz, Seq s, void *v);

#define seq_pop_iso(T, S) ((T*) seq_pop_(sizeof(T), (S), 1))
#define seq_pop(S) seq_pop_iso(typeof(*S), S)
#define seq_tos_ptr_iso(T, S) ((T*) seq_pop_(sizeof(T), (S), 0))
#define seq_tos_ptr(S) seq_tos_ptr_iso(typeof(*S), S)
#define seq_tos_iso(T, S) (*seq_tos_ptr_iso(T, S))
#define seq_tos(S) seq_tos_iso(typeof(*S), S)
CIRCA void *seq_pop_(size_t siz, Seq s, size_t n);

#define seq_pull_iso(T, S) seq_pull_(sizeof(T), (S))
#define seq_pull(S) seq_pull_iso(typeof(*S), S)
CIRCA void *seq_pull_(size_t siz, Seq s);

/*
** Function Implementations
*/

static inline CIRCA_PURE CIRCA_NONNULL(1)
SeqData *seq(Seq s) {
  return ((SeqData*) s) - 1;
}

/*
** Iterator Macros
*/

#define seq_foreach_iso(T, S, V)                       \
for (size_t I = 0, J = 0; I < seq(S)->len; I++, J = 0) \
for (T V = seq_get_iso(T, S, I); J != 1; J = 1)
#define seq_foreach(S, V) seq_foreach_iso(typeof(*S), S, V)

#define seq_filter_iso(T, S, EXPR)                                   \
do {                                                                 \
  size_t LEN = seq(S)->len;                                          \
  T it;                                                              \
  for (size_t I = 0; I < LEN; I++) {                                 \
    it = seq_get_iso(T, S, I);                                       \
    if (EXPR) seq_push_iso(T, S, &it);                               \
  }                                                                  \
  seq(S)->len -= LEN;                                                \
  memcpy(S, ((char*) S) + LEN * sizeof(T), seq(S)->len * sizeof(T)); \
} while(0)
#define seq_filter(S, F) seq_filter_iso(typeof(*S), S, F)

#define seq_map_iso(T, S, EXPR)               \
do {                                          \
  for (size_t I = 0; I < seq(S)->len; I++) {  \
    T it = seq_get_iso(T, (S), (I));          \
    seq_set_iso(T, S, I, &(T){EXPR});         \
  }                                           \
} while (0)
#define seq_map(S, F) seq_map_iso(typeof(*S), S, F)

// TODO: seq_foldl and seq_foldr

/*
** Header-Only Mode
*/

#ifdef CIRCA_HEADER_ONLY
  #include "../c/seq.c"
#endif

#endif // CIRCA_SEQ_H
