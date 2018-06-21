   //                                               ////////////////////////////
  // circa.h | The Circa Header File               ////////////////////////////
 // Source: https://github.com/davidgarland/circa ////////////////////////////
//                                               ////////////////////////////

#ifndef CIRCA_H
#define CIRCA_H

  //              //////////////////////////////////////////////////////////////
 // Dependencies //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

  //     //
 // STL //
//     //

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

  //    //
 // OS //
//    //

#ifdef _WIN32
  #include <windows.h>
#endif

  //        ////////////////////////////////////////////////////////////////////
 // Config ////////////////////////////////////////////////////////////////////
//        ////////////////////////////////////////////////////////////////////

  //            //
 // Allocation //
//            //

#define CIRCA_ALLOC_D  0    // How much "extra" memory to allocate.

  //                   //
 // Security & Safety //
//                   //

#define CIRCA_SECURE_D true // Enable security. (Memset freed memory to 0, etc.)
#define CIRCA_SAFE_D   true // Enable safety features. (Memset allocated memory to 0, etc.)

  //             //
 // Compilation //
//             //

#define CIRCA_STATIC_D true // Set Circa's functions to static linkage.
#define CIRCA_GNU_D    true // Enable use of GNU C extensions.

  //           //
 // Debugging //
//           //

#define CIRCA_DBG_D true // Enable debugging features (Assertions, errors, etc.)

  //                 ///////////////////////////////////////////////////////////
 // Config Handling ///////////////////////////////////////////////////////////
//                 ///////////////////////////////////////////////////////////

#if !defined(CIRCA_ALLOC)
  #if CIRCA_ALLOC_D >= 0
    #define CIRCA_ALLOC CIRCA_ALLOC_D
  #else
    #define CIRCA_ALLOC 0
  #endif
#endif

#if !defined(CIRCA_SECURE) && !defined (CIRCA_NO_SECURE) && CIRCA_SECURE_D
  #define CIRCA_SECURE
#elif defined(CIRCA_SECURE) && defined(CIRCA_NO_SECURE)
  #undef CIRCA_SECURE
#endif

#if !defined(CIRCA_SAFE) && !defined(CIRCA_NO_SAFE) && CIRCA_SAFE_D
  #define CIRCA_SAFE
#elif defined(CIRCA_SAFE) && defined(CIRCA_NO_SAFE)
  #undef CIRCA_SAFE
#endif

#if !defined(CIRCA_STATIC) && !defined(CIRCA_NO_STATIC) && CIRCA_STATIC_D
  #define CIRCA_STATIC
#elif defined(CIRCA_STATIC) && defined(CIRCA_NO_STATIC)
  #undef CIRCA_STATIC
#endif

#if !defined(CIRCA_GNU) && !defined(CIRCA_NO_GNU) && CIRCA_GNU_D
  #define CIRCA_GNU
#elif defined(CIRCA_GNU) && defined(CIRCA_NO_GNU)
  #undef CIRCA_GNU
#endif

#if !defined(CIRCA_DBG) && !defined(CIRCA_NO_DBG) && !defined(NDEBUG) && CIRCA_DBG_D
  #define CIRCA_DBG
#elif defined(CIRCA_DBG) && defined(CIRCA_NO_DBG)
  #undef CIRCA_DBG
#endif

  //                  //////////////////////////////////////////////////////////
 // Attribute Macros //////////////////////////////////////////////////////////
//                  //////////////////////////////////////////////////////////

#ifdef CIRCA_GNU
  #define _circa_attr_(...) __attribute__((__VA_ARGS__))
#else
  #define _circa_attr_(...)
#endif

#define _circa_rets_  _circa_attr_(warn_unused_result)
#define _circa_alcs_ _circa_rets_ _circa_attr_(malloc)

#ifdef CIRCA_STATIC
  #define _circa_static_ static inline
#else
  #define _circa_static_
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

#define circa_error(...) circa_error_(__VA_ARGS__, circa_dbg, __FILE__, __LINE__, __func__)

#define circa_error_(MSG, DBG, FNAME, LINE, FUNC) DBG({                        \
  printf("\nError in file '%s', on line %i:\n", FNAME, LINE);                  \
  printf("  In function '%s': '%s'\n", FUNC, MSG);                             \
  exit(1);                                                                     \
})

#define circa_assert(...) circa_assert_(__VA_ARGS__, circa_dbg, #__VA_ARGS__, __FILE__, __LINE__, __func__)

#define circa_assert_(COND, DBG, CONDSTR, FNAME, LINE, FUNC) DBG({             \
  if (!(COND)) {                                                               \
    circa_error_(CONDSTR, DBG, FNAME, LINE, FUNC);                             \
  }                                                                            \
})

  //                  //////////////////////////////////////////////////////////
 // Type Definitions //////////////////////////////////////////////////////////
//                  //////////////////////////////////////////////////////////

  //        //
 // Seq(T) //
//        //

#define Seq(T) T*
typedef Seq(void) Seq;

struct SeqData {
  size_t cap, len;
  char data[];
};

  //     //
 // Str //
//     //

typedef char *Str;

struct StrData {
  size_t cap, len;
  char data[];
};

  //        //
 // Ctx(T) //
//        //

#ifdef _WIN32
  typedef DWORD WINAPI (*TaskPtr)(void*);
#else
  typedef void*(*TaskPtr)(void*);
#endif

typedef struct {
  Seq args;
  Seq rets;
} *Ctx;

  //          //
 // Arena(T) //
//          //

#define Arena(T) T*
typedef Arena(void) Arena;

struct ArenaData {
  size_t cap;
  bool *in_use;
  char data[];
};

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

#define seq_new(T, C) seq_new_(sizeof(T), (C));
_circa_ _circa_alcs_ Seq seq_new_(size_t siz, size_t cap);

#define seq_lit(T, ...) seq_lit_(sizeof(T), sizeof((T[]){__VA_ARGS__}) / sizeof(T), &(T[]){__VA_ARGS__})
_circa_ _circa_alcs_ Seq seq_lit_(size_t siz, size_t len, void *lits);

#define seq_from(S) seq_from_(sizeof(*(S)), (S));
_circa_ _circa_alcs_ Seq seq_from_(size_t siz, Seq s);

#define seq_wrap(P, L) seq_wrap_(sizeof(*(P)), (P), (L))
_circa_ _circa_alcs_ Seq seq_wrap_(size_t siz, void *ptr, size_t len);

#define seq_rsz(S, C) ((S) = seq_rsz_(sizeof(*(S)), (S), (C)), (S))
_circa_ _circa_rets_ Seq seq_rsz_(size_t siz, Seq s, size_t cap);

#define seq_rqr(S, C) ((S) = seq_rqr_(sizeof(*(S)), (S), (C)), (S))
_circa_ _circa_rets_ Seq seq_rqr_(size_t siz, Seq s, size_t cap);

#define seq_shr(S) ((S) = seq_shr_(sizeof(*(S)), (S)), (S))
_circa_ _circa_rets_ Seq seq_shr_(size_t siz, Seq s);

#define seq_del(S) (S) = seq_del_(sizeof(*(S)), (S))
_circa_ _circa_rets_ Seq seq_del_(size_t siz, Seq s);

  //
 // Sequence Ops
//

#define seq_cpy(DST, SRC) ((DST) = seq_cpy_(sizeof(*(DST)), (DST), (SRC)), (DST))
_circa_ _circa_rets_ Seq seq_cpy_(size_t siz, Seq dst, Seq src);

#define seq_cat(DST, SRC) ((DST) = seq_cat_(sizeof(*(DST)), (DST), (SRC)), (DST))
_circa_ _circa_rets_ Seq seq_cat_(size_t siz, Seq dst, Seq src);

#define seq_rvs(S) ((S) = seq_rvs_(sizeof(*(S)), (S)), (S))
_circa_ _circa_rets_ Seq seq_rvs_(size_t siz, Seq s);

  //
 // Array Ops
//

#define seq_set(S, A, V) ((S) = seq_set_(sizeof(*(S)), (S), (A), &(typeof(*(S))){V}), (S))
_circa_ _circa_rets_ Seq seq_set_(size_t siz, Seq s, size_t addr, void *val);

#define seq_get(S, A) (*((Seq(typeof(*(S)))) seq_get_(sizeof(*(S)), (S), (A))))
_circa_ _circa_rets_ void *seq_get_(size_t siz, Seq s, size_t addr);

  //
 // Stack Ops
//

#define seq_push(S, V) ((S) = seq_push_(sizeof(*(S)), (S), &(typeof(*(S))){V}), (S))
_circa_ _circa_rets_ Seq  seq_push_(size_t siz, Seq s, void *val);

#define seq_push_ext(S, V, P) ((S) = seq_push_ext_(sizeof(*S), (S), &(typeof(*(S)){V}, (P))), (S))
_circa_ _circa_rets_ Seq seq_push_ext_(size_t siz, Seq s, void *val, size_t pre);

#define seq_tos(S) (*((Seq(typeof(*(S)))) seq_tos_(sizeof(*(S)), (S))))
_circa_ _circa_rets_ void *seq_tos_(size_t siz, Seq s);

#define seq_pop(S) (*((Seq(typeof(*(S)))) seq_pop_(sizeof(*S), (S))))
_circa_ _circa_rets_ void *seq_pop_(size_t siz, Seq s);

#define seq_dup(S) ((S) = seq_dup_(sizeof(*(S)), (S)), (S))
_circa_ _circa_rets_ Seq seq_dup_(size_t siz, Seq s);

  //
 // Processing
//

#define seq_do(S, F) \
do { \
  for (int I = 0; I < seq(S)->len; I++) { \
    F(seq_get(S, I)); \
  } \
} while(0)

#define seq_apply(S, F) \
do { \
  for (int I = 0; I < seq(S)->len; I++) { \
    seq_set(S, I, F(seq_get(S, I))); \
  } \
} while(0)

#define seq_map(A, F, B) \
do { \
  seq_rqr(B, seq(A)->len); \
  seq(B)->len = seq(A)->len; \
  for (int I = 0; I < seq(A)->len; I++) { \
    seq_set(B, I, F(seq_get(A, I))); \
  } \
} while(0)

#define seq_filter(A, F, B) \
do { \
  seq_rqr(B, seq(A)->len); \
  seq(B)->len = 0; \
  for (int I = 0; I < seq(A)->len; I++) { \
    if (F(seq_get(A, I))) { \
      seq_push(B, seq_get(A, I)); \
    } \
  } \
} while(0)

#define seq_reduce(A, F, B) \
do { \
  for (int I = 0; I < seq(A)->len; I++) { \
    B = F(B, seq_get(A, I)); \
  } \
} while(0)

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
_circa_ _circa_alcs_ Str str_new_(size_t cap);

#define str_lit(CS) str_lit_((CS))
_circa_ _circa_alcs_ Str str_lit_(const char *restrict cs);

#define str_from(S) str_from_((S))
_circa_ _circa_alcs_ Str str_from_(Str s);

#define str_rsz(S, C) (S) = str_rsz_((S), (C))
_circa_ _circa_rets_ Str str_rsz_(Str s, size_t cap);

#define str_rqr(S, C) (S) = str_rqr_((S), (C))
_circa_ _circa_rets_ Str str_rqr_(Str s, size_t cap);

#define str_shr(S) (S) = str_shr_((S))
_circa_ _circa_rets_ Str str_shr_(Str s);

#define str_del(S) (S) = str_del_((S))
_circa_ _circa_rets_ Str str_del_(Str s);

  //
 // String Ops
//

#define str_cpy(DST, SRC) (DST) = str_cpy_((DST), (SRC))
_circa_ _circa_rets_ Str str_cpy_(Str dst, Str src);

#define str_cat(DST, SRC) (DST) = str_cat_((DST), (SRC))
_circa_ _circa_rets_ Str str_cat_(Str dst, Str src);

#define str_rvs(S) (S) = str_rvs_((S))
_circa_ _circa_rets_ Str str_rvs_(Str s);

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
_circa_ _circa_rets_ Str str_set_(Str s, size_t addr, char ch);

#define str_get(S, A) str_get_((S), (A))
_circa_ char str_get_(Str s, size_t addr);

  //
 // Stack Ops
//

#define str_push(S, CH) (S) = str_push((S), (CH))
_circa_ _circa_rets_ Str str_push_(Str s, char ch);

#define str_tos(S) str_tos_((S))
_circa_ char str_tos_(Str s);

#define str_pop(S) str_pop_((S))
_circa_ char str_pop_(Str s);

  //
 // IO Ops
//

#define str_readfile(S, F) (S) = str_readfile_((S), (F))
_circa_ _circa_rets_ Str str_readfile_(Str s, char *filename);


  //                             ///////////////////////////////////////////////
 // Context Function Prototypes ///////////////////////////////////////////////
//                             ///////////////////////////////////////////////

  //
 // Allocators
//

#define ctx_new(A, B) ctx_new_(sizeof(*(A)), sizeof(*(B)), (A), (B))
_circa_ _circa_alcs_ Ctx ctx_new_(size_t sarg, size_t sret, Seq args, Seq rets);

  //
 // Launching
//

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
_circa_ _circa_alcs_ Arena arena_new_(size_t siz, size_t cap);

#define arena_rsz(A, C) ((A) = arena_rsz_(sizeof(*(A)), (A), (C)), (A))
_circa_ _circa_rets_ Arena arena_rsz_(size_t siz, Arena a, size_t cap);

#define arena_del(A) (A) = arena_del_(sizeof(*(A)), (A))
_circa_ _circa_rets_ Arena arena_del_(size_t siz, Arena a);

  //
 // Array Ops
//

#define arena_take(A) ((typeof(*(A))*) arena_take_(sizeof(*(A)), &(A)))
_circa_ _circa_rets_ void* arena_take_(size_t siz, Arena *a);

#define arena_give(A, P) (A) = arena_give_(sizeof(*(A)), (A), (P))
_circa_ _circa_rets_ Arena arena_give_(size_t siz, Arena a, void *ptr);

#include "src/seq.h"
#include "src/str.h"
#include "src/ctx.h"
#include "src/arena.h"

#endif
