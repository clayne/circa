/*
** seq.h | Circa | Dynamically allocated sequences.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

/*
** Compatibility Checks
*/

#ifndef __STDC_VERSION__
  #error "[circa/seq.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/seq.h]: C99 is required."
#endif

#ifndef __GNUC__
  #pragma message "[circa/seq.h] GNU C is missing. Use the _iso versions of functions."  
#endif

/*
** Dependencies
*/

/* Standard */

#include <stdlib.h>
#include <string.h>

/* Internal */

#include "core.h"

/*
** Type Definitions
*/

#define Seq(T) T*

typedef Seq(void) Seq;

struct seq_data {
  size_t cap;   // The capacity of the sequence.
  size_t len;   // The length of the sequence.
  char data[];  // The sequence's data storage.
};

/*
** Function Prototypes
*/

/* Accessors */

#define seq(S) seq_((S), __FILE__, _circa_str_(__LINE__))
_circa_ struct seq_data *seq_(Seq s, circa_msg fname, circa_msg line);

#define seq_set_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), (&V), __FILE__, _circa_str_(__LINE__))
#define seq_set(S, A, V) seq_set_iso(typeof(*S), S, A, V)
#define seq_set_lit_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), &(T){V}, __FILE__, _circa_str_(__LINE__))
#define seq_set_lit(S, A, V) seq_set_lit_iso(typeof(*S), S, A, V)
_circa_ _circa_rets_ Seq seq_set_(size_t siz, Seq s, size_t addr, void *val, circa_msg fname, circa_msg line);

#define seq_set_ext_iso(T, S, A, V, E) (S) = seq_set_ext_(sizeof(T), (S), (A), (&V), (E), __FILE__, _circa_str_(__LINE__))))
#define seq_set_ext(S, A, V, E) seq_set_ext_(typeof(*S), S, A, V, E)
#define seq_set_lit_ext_iso(T, S, A, V, E) (S) = seq_set_ext_(sizeof(T), (S), (A), &(T){V}, (E), __FILE__, _circa_str_(__LINE__))
#define seq_set_lit_ext(S, A, V, E) seq_set_lit_ext_iso(typeof(*S), S, A, V, E)
_circa_ _circa_rets_ Seq seq_set_ext_(size_t siz, Seq s, size_t addr, void *val, size_t pre, circa_msg fname, circa_msg line);

#define seq_get_iso(T, S, A) (*((T*) seq_get_(sizeof(T), (S), (A), __FILE__, _circa_str_(__LINE__))))
#define seq_get(S, A) seq_get_iso(typeof(*S), S, A)
_circa_ _circa_rets_ void *seq_get_(size_t siz, Seq s, size_t addr, circa_msg fname, circa_msg line);

/* Allocators */

#define seq_new_iso(T, C) seq_new_(sizeof(T), (C), __FILE__, _circa_str_(__LINE__))
#define seq_new(T, C) seq_new_iso(T, C)
_circa_ _circa_alcs_ Seq seq_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line);

#define seq_lit_iso(T, ...) seq_lit_(sizeof(T), sizeof((T[]){__VA_ARGS__}) / sizeof(T), &(T[]){__VA_ARGS__}, __FILE__, _circa_str_(__LINE__))
#define seq_lit(T, ...) seq_lit_iso(T, __VA_ARGS__)
_circa_ _circa_alcs_ Seq seq_lit_(size_t siz, size_t len, void *lits, circa_msg fname, circa_msg line);

#define seq_from_iso(T, S) seq_from_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_from(S) seq_from_iso(typeof(*S), (S))
_circa_ _circa_alcs_ Seq seq_from_(size_t siz, Seq s, circa_msg fname, circa_msg line);

#define seq_wrap_iso(T, P, L) seq_wrap_(sizeof(T), (P), (L), __FILE__, _circa_str_(__LINE__))
#define seq_wrap(P, L) seq_wrap_iso(typeof(*P), (P), (L))
_circa_ _circa_alcs_ Seq seq_wrap_(size_t siz, void *ptr, size_t len, circa_msg fname, circa_msg line);

#define seq_rsz_iso(T, S, C) (S) = seq_rsz_(sizeof(T), (S), (C), __FILE__, _circa_str_(__LINE__))
#define seq_rsz(S, C) seq_rsz_iso(typeof(*S), (S), (C))
_circa_ _circa_rets_ Seq seq_rsz_(size_t siz, Seq s, size_t cap, circa_msg fname, circa_msg line);

#define seq_rqr_iso(T, S, C) (S) = seq_rqr_(sizeof(T), (S), (C), __FILE__, _circa_str_(__LINE__))
#define seq_rqr(S, C) seq_rqr_iso(typeof(*S), (S), (C))
_circa_ _circa_rets_ Seq seq_rqr_(size_t siz, Seq s, size_t cap, circa_msg fname, circa_msg line);

#define seq_shr_iso(T, S) (S) = seq_shr_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_shr(S) seq_shr_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_shr_(size_t siz, Seq s, circa_msg fname, circa_msg line);

#define seq_del_iso(T, S) (S) = seq_del_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_del(S) seq_del_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_del_(size_t siz, Seq s, circa_msg fname, circa_msg line);

/* Stack Operations */

#define seq_push_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), (&V), __FILE__, _circa_str_(__LINE__))
#define seq_push(S, V) seq_push_iso(typeof(*S), (S), (V))
#define seq_push_lit_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), &(T){V}, __FILE__, _circa_str_(__LINE__))
#define seq_push_lit(S, V) seq_push_lit_iso(typeof(*S), S, V)
_circa_ _circa_rets_ Seq  seq_push_(size_t siz, Seq s, void *val, circa_msg fname, circa_msg line);

#define seq_tos_iso(T, S) (*((seq(T)) seq_tos_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))))
#define seq_tos(S) seq_tos_iso(typeof(*S), (S))
_circa_ _circa_rets_ void *seq_tos_(size_t siz, Seq s, circa_msg fname, circa_msg line);

#define seq_pop_iso(T, S) (*((seq(T)) seq_pop_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))))
#define seq_pop(S) seq_pop_iso(typeof(*S), (S))
_circa_ _circa_rets_ void *seq_pop_(size_t siz, Seq s, circa_msg fname, circa_msg line);

#define seq_dup_iso(T, S) (S) = seq_dup_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_dup(S) seq_dup_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_dup_(size_t siz, Seq s, circa_msg fname, circa_msg line);

/* Sequence Operations */

#define seq_cpy_iso(T, D, S) (D) = seq_cpy_(sizeof(T), (D), (S), __FILE__, _circa_str_(__LINE__))
#define seq_cpy(D, S) seq_cpy_iso(typeof(*D), (S))
_circa_ _circa_rets_ Seq seq_cpy_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line);

#define seq_cat_iso(T, D, S) (D) = seq_cat_(sizeof(T), (D), (S), __FILE__, _circa_str_(__LINE__))
#define seq_cat(D, S) seq_cat_iso(typeof(*D), (D), (S))
_circa_ _circa_rets_ Seq seq_cat_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line);

#define seq_rvs_iso(T, S) (S) = seq_rvs_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_rvs(S) seq_rvs_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_rvs_(size_t siz, Seq s, circa_msg fname, circa_msg line);

/*
** Accessors
*/

/* 
** -- seq_ --
** Description
**   Returns a pointer to the structure of a sequence.
** Arguments
**   s: Sequence (seq)

** Returns
**   Sequence Structure Pointer (struct seq_data*)
*/

_circa_
struct seq_data *seq_(Seq s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-align"
  #endif
  return ((struct seq_data*) s) - 1;
  #if defined(__GNUC__)
    #pragma GCC diagnostic pop
  #endif
}

/*
** -- seq_set_ --
** Description
**   Sets a sequence's value at a given address.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   addr: Address (size_t)
**   val: Value (void*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_set_(size_t siz, Seq s, size_t addr, void *val, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(val != NULL, fname, line);
  }
  return seq_set_ext_(siz, s, addr, val, 0, fname, line);
}

/*
** -- seq_set_ext_ --
** Description
**   Sets a sequence's value at a given address, preallocating
**   extra memory.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   addr: Address (size_t)
**   val: Value (void*)
**   pre: Preallocation (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
 ** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_set_ext_(size_t siz, Seq s, size_t addr, void *val, size_t pre,
circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(val != NULL, fname, line);
  }
  s = seq_rqr_(siz, s, addr + 1 + pre, fname, line);
  if (addr >= seq(s)->len) seq(s)->len = addr + 1;
  memcpy(((char*) s) + (siz * addr), val, siz);
  return s;
}

/*
** -- seq_get_ --
** Description
**   Gets a sequence's value at a given address.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   addr: Address (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Value (void*)
*/

_circa_ _circa_rets_
void *seq_get_(size_t siz, Seq s, size_t addr, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(seq(s)->len > 0, fname, line);
    circa_assert(seq(s)->len > addr, fname, line);
  }
  return (void*) (((char*) s) + (siz * addr));
}

/*
** Allocators
*/

/*
** -- seq_new_ --
** Description
**   Allocates a sequence of a given type-size and capacity.
** Arguments
**   siz: Type Size (size_t)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  const size_t blk = cap * siz;
  struct seq_data *s = NULL;
  while (s == NULL) s = malloc(sizeof(*s) + blk);
  s->cap = cap;
  s->len = 0;
  return s->data;
}

/*
** -- seq_lit_ --
** Description
**   Allocates a sequence of a given type-size and length from a set of
**   literals.
** Arguments
**   siz:  Type Size (size_t)
**   len:  Length (size_t)
**   lits: Literals (void*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_lit_(size_t siz, size_t len, void *lits, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(len > 0, fname, line);
    circa_assert(lits != NULL, fname, line);
  }
  Seq s = seq_new_(siz, len, fname, line);
  memcpy(s, lits, (len * siz));
  seq(s)->len = len;
  return s;
} 

/*
** -- seq_from_ --
** Description
**   Creates a new sequence from another.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_from_(size_t siz, Seq s, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  return seq_wrap_(siz, s, seq(s)->len, fname, line);
}

/*
** -- seq_wrap_ --
** Description
**   Creates a new sequence from a pointer.
** Arguments
**   siz: Type Size (size_t)
**   ptr: Pointer (void*)
**   len: Length (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_wrap_(size_t siz, void *ptr, size_t len, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(ptr != NULL, fname, line);
    circa_assert(len > 0, fname, line);
  }
  Seq s = seq_new_(siz, len, fname, line);
  seq(s)->len = len;
  memcpy(s, ptr, (siz * len));
  return s;
}

/*
** -- seq_rsz_ --
** Description
**   Resizes a sequence's capacity.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rsz_(size_t siz, Seq s, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(cap >= seq(s)->len, fname, line);
  }
  #ifdef CIRCA_SECURE
    if (cap < seq(s)->cap)
      memset(((char*) s) + cap, 0, (seq(s)->cap - cap) * siz);
  #endif
  struct seq_data *s2 = NULL;
  while (s2 == NULL) s2 = realloc(seq(s), sizeof(*s2) + (cap * siz));
  s2->cap = cap;
  return s2->data;
}

/*
** -- seq_rqr_ --
** Description
**   Requires that a sequence have at least a given capacity.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rqr_(size_t siz, Seq s, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  return (cap > seq(s)->cap) ? seq_rsz_(siz, s, cap, fname, line) : s;
}

/*
** -- seq_shr_ --
** Description
**   Shrinks a sequence's capacity to its length.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_shr_(size_t siz, Seq s, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  const size_t len = seq(s)->len;
  if (seq(s)->cap > len)
    return seq_rsz_(siz, s, len, fname, line);
  else
    return s;
}

/*
** -- seq_del_ --
** Description
**   Deletes a sequence.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_del_(size_t siz, Seq s, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
  }
  if (s != NULL) {
    #ifdef CIRCA_SECURE
      memset(s, 0, siz * seq(s)->cap);
      seq(s)->cap = 0;
      seq(s)->len = 0;
    #endif
    free(seq(s));
  }
  return NULL;
}

/*
** Stack Ops
*/

/*
** -- seq_push_ --
** Description
**   Pushes a new value onto a sequence.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   val: Value (void*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_push_(size_t siz, Seq s, void *val, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(val != NULL, fname, line);
  }
  return seq_set_(siz, s, seq(s)->len, val, fname, line);
}

/*
** -- seq_push_ext_
** Description
**   Pushes a new value onto a sequence, preallocating extra memory.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   val: Value (void*)
**   pre: Preallocation (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (seq)
*/

_circa_ _circa_rets_
Seq seq_push_ext_(size_t siz, Seq s, void *val, size_t pre, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(val != NULL, fname, line);
  }
  return seq_set_ext_(siz, s, seq(s)->len, val, pre, fname, line);
}

/*
** sequence Ops
*/

/*
** -- seq_cpy_ --
** Description
**   Copies the elements of one sequence to another.
** Arguments
**   siz: Type Size (size_t)
**   dst: Destination (Seq)
**   src: Source (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_cpy_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
  }
  const size_t src_len = seq(src)->len;
  dst = seq_rqr_(siz, dst, src_len, fname, line);
  memcpy(dst, src, siz * src_len);
  seq(dst)->len = src_len;
  return dst;
}

/*
** -- seq_cat_ --
** Description
**   Catenate the contents of one sequence to another.
**   (Yes, catenate is a word in of itself. I'm as surprised as you are.)
** Arguments
**   siz: Type Size (size_t)
**   dst: Destination (Seq)
**   src: Source (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_cat_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
  }
  const size_t dst_len = seq(dst)->len, src_len = seq(src)->len;
  dst = seq_rqr_(siz, dst, dst_len + src_len, fname, line);
  memcpy((((char*) dst) + (siz * dst_len)), src, siz * src_len);
  seq(dst)->len = dst_len + src_len;
  return dst;
}

/*
** -- seq_rvs_ --
** Desscription
**   Reverses the contents of a sequence.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rvs_(size_t siz, Seq s, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  const size_t s_len = seq(s)->len;
  s = seq_rqr_(siz, s, s_len + 1, fname, line);
  void *const restrict tmp = (((char*) s) + (siz * s_len));
  for (size_t i = 0, j = s_len - 1; i < s_len / 2; i++, j--) {
    memmove(tmp, seq_get_(siz, s, i, fname, line), siz);
    memmove(seq_get_(siz, s, i, fname, line), seq_get_(siz, s, j, fname, line), siz);
    memmove(seq_get_(siz, s, j, fname, line), tmp, siz);
  }
  return s;
}

/*
** Functional Ops
*/

#define seq_do_(T, S, F, FNAME, LINE) \
do { \
  for (size_t I = 0; I < seq_(S, FNAME, LINE)->len; I++) { \
    F((*((T*) seq_get_(sizeof(T), S, I, FNAME, LINE)))); \
  } \
} while (0)

#define seq_do_iso(T, S, F) seq_do_(T, S, F, __FILE__, _circa_str_(__LINE__))

#define seq_do(S, F) seq_do_iso(typeof(*S), S, F)

#define seq_apply_(T, S, F, FNAME, LINE) \
do { \
  T V; \
  for (size_t I = 0; I < seq_(S, FNAME, LINE)->len; I++) { \
    (V) = F((*((T*) seq_get_(sizeof(T), S, I, FNAME, LINE)))); \
    (S) = seq_set_(sizeof(T), S, I, &V, FNAME, LINE); \
  } \
} while (0)

#define seq_apply_iso(T, S, F) seq_apply_(T, S, F, __FILE__, _circa_str_(__LINE__))

#define seq_apply(S, F) seq_apply_iso(typeof(*S), S, F)

#define seq_map_(T, A, F, B, FNAME, LINE) \
do { \
  T V; \
  (B) = seq_rqr_(sizeof(T), B, seq_(A, FNAME, LINE)->len, FNAME, LINE); \
  seq_(B, FNAME, LINE)->len = seq_(A, FNAME, LINE)->len; \
  for (size_t I = 0; I < seq_(A, FNAME, LINE)->len; I++) { \
    (V) = F((*((T*) seq_get_(sizeof(T), A, I, FNAME, LINE)))); \
    (B) = seq_set_(sizeof(T), B, I, &V, FNAME, LINE); \
  } \
} while (0)

#define seq_map_iso(T, A, F, B) seq_map_(T, A, F, B, __FILE__, _circa_str_(__LINE__))

#define seq_map(A, F, B) seq_map_iso(typeof(*A), A, F, B)

#define seq_filter_(T, A, F, B, FNAME, LINE) \
do { \
  T V; \
  (B) = seq_rqr_(sizeof(T), B, seq_(A, FNAME, LINE)->len, FNAME, LINE); \
  seq_(B, FNAME, LINE)->len = 0; \
  for(size_t I = 0; I < seq_(A, FNAME, LINE)->len; I++) { \
    (V) = (*((T*) seq_get_(sizeof(T), A, I, FNAME, LINE))); \
    if (F(V)) { \
      (B) = seq_push_(sizeof(T), B, &V, FNAME, LINE);\
    } \
  } \
} while (0)

#define seq_filter_iso(T, A, F, B) seq_filter_(T, A, F, B, __FILE__, _circa_str_(__LINE__))

#define seq_filter(A, F, B) seq_filter_iso(typeof(*A), A, F, B)

#define seq_reduce_(T, A, F, B) \
do { \
  for (size_t I = 0; I < seq_(A, FNAME, LINE)->len; I++) { \
    B = F(B, (*((T*) seq_get_(sizeof(T), A, I, FNAME, LINE)))); \
  } \
} while (0)

#define seq_reduce_iso(T, A, F, B) seq_reduce_(T, A, F, B, __FILE__, _circa_str_(__LINE__))

#define seq_reduce(A, F, B) seq_reduce_iso(typeof(*A), A, F, B)

#endif /* CIRCA_SEQ_H */
