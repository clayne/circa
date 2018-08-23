/*
** seq.h | Dynamically allocated sequences.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

/*
** Compatibility Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/seq.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/seq.h]: C99 is required."
#endif

#if !defined(__GNUC__)
  #pragma message "[circa/seq.h] GNU C is missing. Use the _iso versions of functions."  
#endif

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wlanguage-extension-token"
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

struct SeqData {
  size_t cap;   // The capacity of the sequence.
  size_t len;   // The length of the sequence.
  char data[];  // The sequence's data storage.
};

/*
** Function Prototypes
*/

/* Accessors */

#define seq(S) seq_((S), __FILE__, _circa_str_(__LINE__))
_circa_ struct SeqData *seq_(Seq s, CircaMsg fname, CircaMsg line);

#define seq_set_iso(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), &(T){V}, __FILE__, _circa_str_(__LINE__))
#define seq_set(S, A, V) seq_set_iso(typeof(*S), (S), (A), (V))
_circa_ _circa_rets_ Seq seq_set_(size_t siz, Seq s, size_t addr, void *val, CircaMsg fname, CircaMsg line);

#define seq_set_ext_iso(T, S, A, V, E) (*((T*) seq_set_ext_(sizeof(T), (S), (A), &(T){V}, (E), __FILE__, _circa_str_(__LINE__))))
#define seq_set_ext(S, A, V, E) seq_set_ext_(typeof(*S), S, A, V, E)
_circa_ _circa_rets_ void *seq_set_ext_(size_t siz, Seq s, size_t addr, void *val, size_t pre, CircaMsg fname, CircaMsg line);

#define seq_get_iso(T, S, A) (*((T*) seq_get_(sizeof(T), (S), (A), __FILE__, _circa_str_(__LINE__))))
#define seq_get(S, A) seq_get_iso(typeof(*S), (S), (A))
_circa_ _circa_rets_ void *seq_get_(size_t siz, Seq s, size_t addr, CircaMsg fname, CircaMsg line);

/* Allocators */

#define seq_new_iso(T, C) seq_new_(sizeof(T), (C), __FILE__, _circa_str_(__LINE__))
#define seq_new(T, C) seq_new_iso(T, C)
_circa_ _circa_alcs_ Seq seq_new_(size_t siz, size_t cap, CircaMsg fname, CircaMsg line);

#define seq_lit_iso(T, ...) seq_lit_(sizeof(T), sizeof((T[]){__VA_ARGS__}) / sizeof(T), &(T[]){__VA_ARGS__}, __FILE__, _circa_str_(__LINE__))
#define seq_lit(T, ...) seq_lit_iso(T, __VA_ARGS__)
_circa_ _circa_alcs_ Seq seq_lit_(size_t siz, size_t len, void *lits, CircaMsg fname, CircaMsg line);

#define seq_from_iso(T, S) seq_from_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_from(S) seq_from_iso(typeof(*S), (S))
_circa_ _circa_alcs_ Seq seq_from_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

#define seq_wrap_iso(T, P, L) seq_wrap_(sizeof(T), (P), (L), __FILE__, _circa_str_(__LINE__))
#define seq_wrap(P, L) seq_wrap_iso(typeof(*P), (P), (L))
_circa_ _circa_alcs_ Seq seq_wrap_(size_t siz, void *ptr, size_t len, CircaMsg fname, CircaMsg line);

#define seq_rsz_iso(T, S, C) (S) = seq_rsz_(sizeof(T), (S), (C), __FILE__, _circa_str_(__LINE__))
#define seq_rsz(S, C) seq_rsz_iso(typeof(*S), (S), (C))
_circa_ _circa_rets_ Seq seq_rsz_(size_t siz, Seq s, size_t cap, CircaMsg fname, CircaMsg line);

#define seq_rqr_iso(T, S, C) (S) = seq_rqr_(sizeof(T), (S), (C), __FILE__, _circa_str_(__LINE__))
#define seq_rqr(S, C) seq_rqr_iso(typeof(*S), (S), (C))
_circa_ _circa_rets_ Seq seq_rqr_(size_t siz, Seq s, size_t cap, CircaMsg fname, CircaMsg line);

#define seq_shr_iso(T, S) (S) = seq_shr_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_shr(S) seq_shr_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_shr_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

#define seq_del_iso(T, S) (S) = seq_del_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_del(S) seq_del_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_del_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

/* Stack Operations */

#define seq_push_iso(T, S, V) (S) = seq_push_(sizeof(T), (S), &(T){V}, __FILE__, _circa_str_(__LINE__))
#define seq_push(S, V) seq_push_iso(typeof(*S), (S), (V))
_circa_ _circa_rets_ Seq  seq_push_(size_t siz, Seq s, void *val, CircaMsg fname, CircaMsg line);

#define seq_tos_iso(T, S) (*((Seq(T)) seq_tos_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))))
#define seq_tos(S) seq_tos_iso(typeof(*S), (S))
_circa_ _circa_rets_ void *seq_tos_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

#define seq_pop_iso(T, S) (*((Seq(T)) seq_pop_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))))
#define seq_pop(S) seq_pop_iso(typeof(*S), (S))
_circa_ _circa_rets_ void *seq_pop_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

#define seq_dup_iso(T, S) (S) = seq_dup_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_dup(S) seq_dup_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_dup_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

/* Sequence Operations */

#define seq_cpy_iso(T, D, S) (D) = seq_cpy_(sizeof(T), (D), (S), __FILE__, _circa_str_(__LINE__))
#define seq_cpy(D, S) seq_cpy_iso(typeof(*D), (S))
_circa_ _circa_rets_ Seq seq_cpy_(size_t siz, Seq dst, Seq src, CircaMsg fname, CircaMsg line);

#define seq_cat_iso(T, D, S) (D) = seq_cat_(sizeof(T), (D), (S), __FILE__, _circa_str_(__LINE__))
#define seq_cat(D, S) seq_cat_iso(typeof(*D), (D), (S))
_circa_ _circa_rets_ Seq seq_cat_(size_t siz, Seq dst, Seq src, CircaMsg fname, CircaMsg line);

#define seq_rvs_iso(T, S) (S) = seq_rvs_(sizeof(T), (S), __FILE__, _circa_str_(__LINE__))
#define seq_rvs(S) seq_rvs_iso(typeof(*S), (S))
_circa_ _circa_rets_ Seq seq_rvs_(size_t siz, Seq s, CircaMsg fname, CircaMsg line);

/*
** Accessors
*/

/* 
** -- seq_ --
** Description
**   Returns a pointer to the structure of a sequence.
** Arguments
**   s: Sequence (Seq)
** Returns
**   Sequence Structure Pointer (struct SeqData*)
*/

_circa_
struct SeqData *seq_(Seq s, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(s != NULL, fname, line);
  }
  #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-align"
  #endif
  return ((struct SeqData*) s) - 1;
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_set_(size_t siz, Seq s, size_t addr, void *val, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_set_ext_(size_t siz, Seq s, size_t addr, void *val, size_t pre, CircaMsg fname, CircaMsg line)
{
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
**   
*/

_circa_ _circa_rets_
void *seq_get_(size_t siz, Seq s, size_t addr, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_new_(size_t siz, size_t cap, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  const size_t blk = cap * siz;
  struct SeqData *s = NULL;
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
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_lit_(size_t siz, size_t len, void *lits, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_from_(size_t siz, Seq s, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   New Sequence (Seq)
*/

_circa_ _circa_alcs_
Seq seq_wrap_(size_t siz, void *ptr, size_t len, CircaMsg fname, CircaMsg line) {
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rsz_(size_t siz, Seq s, size_t cap, CircaMsg fname, CircaMsg line)
{
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
  struct SeqData *s2 = NULL;
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rqr_(size_t siz, Seq s, size_t cap, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_shr_(size_t siz, Seq s, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
  }
  return (seq(s)->cap > seq(s)->len) ? seq_rsz_(siz, s, seq(s)->len, fname, line) : s;
}

/*
** -- seq_del_ --
** Description
**   Deletes a sequence.
** Arguments
**   siz: Type Size (size_t)
**   s: Sequence (Seq)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_del_(size_t siz, Seq s, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_push_(size_t siz, Seq s, void *val, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_push_ext_(size_t siz, Seq s, void *val, size_t pre, CircaMsg fname, CircaMsg line)
{
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(s != NULL, fname, line);
    circa_assert(val != NULL, fname, line);
  }
  return seq_set_ext_(siz, s, seq(s)->len, val, pre, fname, line);
}

/*
** Sequence Ops
*/

/*
** -- seq_cpy_ --
** Description
**   Copies the elements of one sequence to another.
** Arguments
**   siz: Type Size (size_t)
**   dst: Destination (Seq)
**   src: Source (Seq)
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_cpy_(size_t siz, Seq dst, Seq src, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_cat_(size_t siz, Seq dst, Seq src, CircaMsg fname, CircaMsg line)
{
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
** Returns
**   Feedback Sequence (Seq)
*/

_circa_ _circa_rets_
Seq seq_rvs_(size_t siz, Seq s, CircaMsg fname, CircaMsg line)
{
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

#define seq_do_iso(T, S, F) \
do { \
  for (size_t I = 0; I < seq(S)->len; I++) { \
    F(seq_get_iso(T, S, I)); \
  } \
} while (0)

#define seq_do(S, F) seq_do_iso(typeof(*S), S, F)

#define seq_apply_iso(T, S, F) \
do { \
  for (size_t I = 0; I < seq(S)->len; I++) { \
    seq_set_iso(T, S, I, F(seq_get_iso(T, S, I)); \
  } \
} while (0)

#define seq_apply(S, F) seq_apply_iso(typeof(*S), S, F)

#define seq_map_iso(T, A, F, B) \
do { \
  seq_rqr_iso(T, B, seq(A)->len); \
  seq(B)->len = seq(A)->len; \
  for (size_t I = 0; I < seq(A)->len; I++) { \
    seq_set_iso(T, B, I, F(seq_get_iso(T, A, I))); \
  } \
} while (0)

#define seq_map(A, F, B) seq_map_iso(typeof(*A), A, F, B)

#define seq_filter_iso(T, A, F, B) \
do { \
  seq_rqr(B, seq(A)->len); \
  seq(B)->len = 0; \
  for(size_t I = 0; I < seq(A)->len; I++) { \
    if (F(seq_get_iso(T, A, I))) { \
      seq_push_iso(T, B, seq_get_iso(T, A, I));\
    } \
  } \
} while (0)

#define seq_filter(A, F, B) seq_filter_iso(typeof(*A), A, F, B)

#define seq_reduce_iso(T, A, F, B) \
do { \
  for (size_t I = 0; I < seq(A)->len; I++) { \
    B = F(B, seq_get_iso(T, A, I)); \
  } \
} \

#define seq_reduce(A, F, B) seq_reduce_iso(typeof(*A), A, F, B)

#endif /* CIRCA_SEQ_H */
