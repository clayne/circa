   //                                               ////////////////////////////
  // circa.h | The Circa Header File               ////////////////////////////
 // Source: https://github.com/davidgarland/circa ////////////////////////////
//                                               ////////////////////////////

#ifndef CIRCA_H
#define CIRCA_H

  //              //////////////////////////////////////////////////////////////
 // Dependencies //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

  //        ////////////////////////////////////////////////////////////////////
 // Config ////////////////////////////////////////////////////////////////////
//        ////////////////////////////////////////////////////////////////////

#define CIRCA_SECURE_D true // Enable security. (Memset freed memory to 0, etc.)
#define CIRCA_ALLOC_D 0     // How much "extra" memory to allocate.

#define CIRCA_DBG_D true // Enable debugging features (Assertions, errors, etc.)

  //                 ///////////////////////////////////////////////////////////
 // Config Handling ///////////////////////////////////////////////////////////
//                 ///////////////////////////////////////////////////////////

#if !defined(CIRCA_SECURE) && !defined (CIRCA_NO_SECURE)
  #if CIRCA_SECURE_D
    #define CIRCA_SECURE
  #endif
#elif defined(CIRCA_SECURE) && !defined(CIRCA_NO_SECURE)
  #undef CIRCA_SECURE
#endif

#if !defined(CIRCA_ALLOC)
  #define CIRCA_ALLOC CIRCA_ALLOC_D
#endif

#if !defined(CIRCA_DBG) && !defined(CIRCA_NO_DBG)
  #if CIRCA_DBG_D
    #define CIRCA_DBG
  #endif
#elif defined(CIRCA_DBG) && defined(CIRCA_NO_DBG)
  #undef CIRCA_DBG
#endif

#define _circa_ static inline

  //              //////////////////////////////////////////////////////////////
 // Debug Macros //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

#ifdef CIRCA_DBG
  #define circa_dbg(...) __VA_ARGS__
#else
  #define circa_dbg(...)
#endif

#define circa_error(...) circa_error_(__VA_ARGS__, __FILE__, __LINE__, __func__)

#define circa_error_(MSG, FNAME, LINE, FUNC) circa_dbg({                       \
  printf("\nError in file '%s', on line %i:\n", FNAME, LINE);                    \
  printf("  In function '%s': '%s'\n", FUNC, MSG);                             \
  exit(1);                                                                     \
})

#define circa_assert(...) circa_assert_(__VA_ARGS__, #__VA_ARGS__, __FILE__, __LINE__, __func__)

#define circa_assert_(COND, CONDSTR, FNAME, LINE, FUNC) circa_dbg({            \
  if (!(COND)) {                                                               \
    circa_error_(CONDSTR, FNAME, LINE, FUNC);                                  \
  }                                                                            \
})

  //                  //////////////////////////////////////////////////////////
 // Type Definitions //////////////////////////////////////////////////////////
//                  //////////////////////////////////////////////////////////

struct ArenaData {
  size_t cap;
  bool *in_use;
  char data[];
};

#define Arena(T) T*
typedef Arena(void) Arena;

struct SeqData {
  size_t cap, len;
  char data[];
};

#define Seq(T) T*
typedef Seq(void) Seq;

struct StrData {
  size_t cap, len;
  char data[];
};

typedef char *Str;

  //                           /////////////////////////////////////////////////
 // Arena Function Prototypes /////////////////////////////////////////////////
//                           /////////////////////////////////////////////////

  //
 // Accessors
//

_circa_ struct ArenaData *arena(Arena a);

  //
 // Allocators
//

#define arena_new(T, C) (Arena(T)) arena_new_(sizeof(T), (C))
_circa_ Arena arena_new_(size_t siz, size_t cap);

#define arena_rsz(T, A, C) (A) = (Arena(T)) arena_rsz_(sizeof(T), (A), (C))
_circa_ Arena arena_rsz_(size_t siz, Arena a, size_t cap);

#define arena_del(T, A) arena_del_(sizeof(T), (A))
_circa_ Arena arena_del_(size_t siz, Arena a);

  //
 // Array Ops
//

#define arena_take(T, A) ((T*) arena_take_(sizeof(T), &(A)))
_circa_ void* arena_take_(size_t siz, Arena *a);

#define arena_give(T, A, P) (A) = (Arena(T)) arena_give_(sizeof(T), (A), (P))
_circa_ Arena arena_give_(size_t siz, Arena a, void *ptr);

  //                              //////////////////////////////////////////////
 // Sequence Function Prototypes //////////////////////////////////////////////
//                              //////////////////////////////////////////////

  //
 // Accessors
//

_circa_ struct SeqData *seq(Seq s);

  //
 // Allocators
//

#define seq_new(T, C) (Seq(T)) seq_new_(sizeof(T), (C));
_circa_ Seq seq_new_(size_t siz, size_t cap);

#define seq_lit(T, ...) (Seq(T)) seq_lit_(sizeof(T), sizeof((T[]){__VA_ARGS__}) / sizeof(T), &(T[]){__VA_ARGS__})
_circa_ Seq seq_lit_(size_t siz, size_t len, void *lits);

#define seq_from(T, S) (Seq(T)) seq_from_(sizeof(T), (S));
_circa_ Seq seq_from_(size_t siz, Seq s);

#define seq_wrap(T, P, L) (Seq(T)) seq_wrap_(sizeof(T), (P), (L))
_circa_ Seq seq_wrap_(size_t siz, void *ptr, size_t len);

#define seq_rsz(T, S, C) (S) = (Seq(T)) seq_rsz_(sizeof(T), (S), (C))
_circa_ Seq seq_rsz_(size_t siz, Seq s, size_t cap);

#define seq_rqr(T, S, C) (S) = (Seq(T)) seq_rqr_(sizeof(T), (S), (C))
_circa_ Seq seq_rqr_(size_t siz, Seq s, size_t cap);

#define seq_shr(T, S) (S) = (Seq(T)) seq_shr_(sizeof(T), (S))
_circa_ Seq seq_shr_(size_t siz, Seq s);

#define seq_del(T, S) (S) = seq_del_(sizeof(T), (S))
_circa_ Seq seq_del_(size_t siz, Seq s);

  //
 // Sequence Ops
//

#define seq_cpy(T, DST, SRC) (DST) = seq_cpy_(sizeof(T), (DST), (SRC))
_circa_ Seq seq_cpy_(size_t siz, Seq dst, Seq src);

#define seq_cat(T, DST, SRC) (DST) = seq_cat_(sizeof(T), (DST), (SRC))
_circa_ Seq seq_cat_(size_t siz, Seq dst, Seq src);

#define seq_rvs(T, S) (S) = seq_rvs_(sizeof(T), (S))
_circa_ Seq seq_rvs_(size_t siz, Seq s);

  //
 // Array Ops
//

#define seq_set(T, S, A, V) (S) = seq_set_(sizeof(T), (S), (A), &(T){V})
_circa_ Seq seq_set_(size_t siz, Seq s, size_t addr, void *val);

#define seq_get(T, S, A) *((Seq(T)) seq_get_(sizeof(T), S, A))
_circa_ void *seq_get_(size_t siz, Seq s, size_t addr);

  //
 // Stack Ops
//

#define seq_push(T, S, V) (S) = (Seq(T)) seq_push_(sizeof(T), (S), &(T){V})
_circa_ Seq  seq_push_(size_t siz, Seq s, void *val);

#define seq_push_ext(T, S, V, P) (S) = (Seq(T)) seq_push_ext_(sizeof(T), (S), &(T){V}, (P))
_circa_ Seq seq_push_ext_(size_t siz, Seq s, void *val, size_t pre);

#define seq_tos(T, S) *((Seq(T)) seq_tos_(sizeof(T), S))
_circa_ void *seq_tos_(size_t siz, Seq s);

#define seq_pop(T, S) *((Seq(T)) seq_pop_(sizeof(T), (S)))
_circa_ void *seq_pop_(size_t siz, Seq s);

#define seq_dup(T, S) (S) = (Seq(T)) seq_dup_(sizeof(T), (S))
_circa_ Seq seq_dup_(size_t siz, Seq s);

  //
 // Processing
//

#define seq_do(T, S, F) \
{ \
  for (int I = 0; I < seq(S)->len; I++) { \
    F(seq_get(T, S, I)); \
  } \
}

#define seq_apply(T, S, F) \
{ \
  for (int I = 0; I < seq(S)->len; I++) { \
    seq_set(T, S, I, F(seq_get(T, S, I))); \
  } \
}

#define seq_map(T, A, F, B) \
{ \
  seq_rqr(T, B, seq(A)->len); \
  seq(B)->len = seq(A)->len; \
  for (int I = 0; I < seq(A)->len; I++) { \
    seq_set(T, B, I, F(seq_get(T, A, I))); \
  } \
}

#define seq_filter(T, A, F, B) \
{ \
  seq_rqr(T, B, seq(A)->len); \
  seq(B)->len = 0; \
  for (int I = 0; I < seq(A)->len; I++) { \
    if (F(seq_get(T, A, I))) { \
      seq_push(T, B, seq_get(T, A, I)); \
    } \
  } \
}

#define seq_reduce(T, A, F, B) \
{ \
  for (int I = 0; I < seq(A)->len; I++) { \
    B = F(B, seq_get(T, A, I)); \
  } \
}

  //                            ////////////////////////////////////////////////
 // String Function Prototypes ////////////////////////////////////////////////
//                            ////////////////////////////////////////////////

  //
 // Accessors
//

_circa_ struct StrData *str(Str s);

  //
 // Allocators
//

#define str_new(C) str_new_((C))
_circa_ Str str_new_(size_t cap);

#define str_lit(CS) str_lit_((CS))
_circa_ Str str_lit_(const char *restrict cs);

#define str_from(S) str_from_((S))
_circa_ Str str_from_(Str s);

#define str_rsz(S, C) (S) = str_rsz_((S), (C))
_circa_ Str str_rsz_(Str s, size_t cap);

#define str_rqr(S, C) (S) = str_rqr_((S), (C))
_circa_ Str str_rqr_(Str s, size_t cap);

#define str_shr(S) (S) = str_shr_((S))
_circa_ Str str_shr_(Str s);

#define str_del(S) (S) = str_del_((S))
_circa_ Str str_del_(Str s);

  //
 // String Ops
//

#define str_cpy(DST, SRC) (DST) = str_cpy_((DST), (SRC))
_circa_ Str str_cpy_(Str dst, Str src);

#define str_cat(DST, SRC) (DST) = str_cat_((DST), (SRC))
_circa_ Str str_cat_(Str dst, Str src);

#define str_rvs(S) (S) = str_rvs_((S))
_circa_ Str str_rvs_(Str s);

  //
 // Evaluation Ops
//

#define str_eq(A, B) str_eq_((A), (B))
_circa_ bool str_eq_(Str a, Str b);

#define str_eq_len(A, B, L) str_eq_len_((A), (B), (L))
_circa_ bool str_eq_len_(Str a, Str b, size_t len);

#define str_eq_lit(S, C) str_eq_lit_((S), (C))
_circa_ bool str_eq_lit_(Str s, const char *restrict c);

  //
 // Array Ops
//

#define str_set(S, A, CH) (S) = str_set_((S), (A), (CH))
_circa_ Str str_set_(Str s, size_t addr, char ch);

#define str_get(S, A) str_get_((S), (A))
_circa_ char str_get_(Str s, size_t addr);

  //
 // Stack Ops
//

#define str_push(S, CH) (S) = str_push((S), (CH))
_circa_ Str str_push_(Str s, char ch);

#define str_tos(S) str_tos_((S))
_circa_ char str_tos_(Str s);

#define str_pop(S) str_pop_((S))
_circa_ char str_pop_(Str s);

  //
 // IO Ops
//

#define str_readfile(S, F) (S) = str_readfile_((S), (F))
_circa_ Str str_readfile_(Str s, char *filename);

#include "src/arena.h"
#include "src/seq.h"
#include "src/str.h"

#endif
