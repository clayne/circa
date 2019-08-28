/*
** txt.h | The Circa Library Set | Dynamic text.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_TXT_H
#define CIRCA_TXT_H

/*
** Dependencies
*/

/* Circa */

#include "seq.h"

/*
** Macros
*/

/* Macro given by @craigbarnes. */

#ifdef __GNUC__
  #define PRINTF(X) __attribute__((__format__(__printf__, (X), (X + 1))))
#else
  #define PRINTF(X)
#endif

/*
** Type Definitions
*/

typedef Seq(char) Txt;

/*
** Function Declarations
*/

/* Accessors */

static inline SeqData *txt(Txt t);

#define txt_set(S, A, V) (S) = txt_set_((S), (A), (V))
CIRCA CIRCA_RETURNS Txt txt_set_(Txt t, size_t a, char c);

#define txt_ins(S, A, V) (S) = txt_ins_((S), (A), (V))
CIRCA CIRCA_RETURNS Txt txt_ins_(Txt t, size_t a, char c);

#define txt_del(S, A) txt_del_((S), (A))
CIRCA bool txt_del_(Txt t, size_t a);

#define txt_get(S, A) txt_get_((S), (A))
CIRCA char txt_get_(Txt t, size_t a);

/* Allocators */

#define txt_alloc(C) txt_alloc_((C))
CIRCA CIRCA_ALLOCS Txt txt_alloc_(size_t cap);

#define txt_realloc(S, C) (S) = txt_realloc_(S, C)
CIRCA CIRCA_RETURNS Txt txt_realloc_(Txt t, size_t cap);

#define txt_require(S, C) (S) = txt_require_(S, C)
CIRCA CIRCA_RETURNS Txt txt_require_(Txt t, size_t cap);

#define txt_free(S) (S) = txt_free_((S))
CIRCA CIRCA_RETURNS Txt txt_free_(Txt t);

/* String Operations */

#define txt_cpy(DST, SRC) (DST) = txt_cpy_((DST), (SRC))
CIRCA CIRCA_RETURNS Txt txt_cpy_(Txt dst, Txt src);

#define txt_cpy_slice(DST, SRC, SLICE) (DST) = txt_cpy_slice_((DST), (SRC), (SLICE))
CIRCA CIRCA_RETURNS Txt txt_cpy_slice_(Txt dst, Txt src, Slice slice);

#define txt_cat(DST, SRC) (DST) = txt_cat_((DST), (SRC))
CIRCA CIRCA_RETURNS Txt txt_cat_(Txt dst, Txt src);

#define txt_cat_lit(DST, SRC) (DST) = txt_cat_lit_((DST), (SRC))
CIRCA CIRCA_RETURNS Txt txt_cat_lit_(Txt dst, char *src);

#define txt_fmt(S, FMT, ...) (S) = txt_fmt_((S), (FMT), __VA_ARGS__)
CIRCA CIRCA_RETURNS PRINTF(2) Txt txt_fmt_(Txt t, const char *fmt, ...);

#define txt_cat_fmt(S, FMT, ...) (S) = txt_cat_fmt_((S), (FMT), __VA_ARGS__)
CIRCA CIRCA_RETURNS PRINTF(2) Txt txt_cat_fmt_(Txt t, const char *fmt, ...);

/* Stack Operations */

#define txt_push(S, V) (S) = txt_push_((S), (V))
CIRCA CIRCA_RETURNS Txt txt_push_(Txt t, char c);

#define txt_tos(S) txt_pop_((S), 0)
#define txt_pop(S) txt_pop_((S), 1)
CIRCA char txt_pop_(Txt t, size_t n);

#define txt_pull(S) txt_pull_((S))
CIRCA char txt_pull_(Txt t);

/* Comparison Operations */

#define txt_cmp(A, B) txt_cmp_((A), (B))
CIRCA bool txt_cmp_(Txt a, Txt b);

#define txt_cmp_slice(A, SA, B, SB) txt_cmp_slice_((A), (SA), (B), (SB))
CIRCA bool txt_cmp_slice_(Txt a, Slice sa, Txt b, Slice sb);

/* IO Operations */

#define txt_read(S, FP) (S) = txt_read_((S), (FP))
CIRCA CIRCA_RETURNS Txt txt_read_(Txt t, FILE *fp);

#define txt_cat_read(S, FP) (S) = txt_cat_read_((S), (FP)) 
CIRCA CIRCA_RETURNS Txt txt_cat_read_(Txt t, FILE *fp);

#define txt_write(S, FP) txt_write_((S), (FP))
CIRCA void txt_write_(Txt t, FILE *fp);

/*
** Function Definitions
*/

/* Accessors */

static inline
SeqData *txt(Txt t) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), NULL);
  return seq(t);
}

/*
** Iterator Macros
*/

#define txt_foreach(S, V)                              \
for (size_t I = 0, J = 0; I < seq(S)->len; I++, J = 0) \
for (char V = txt_get(S, I); J != 1; J = 1)

#define txt_keep(S, F)               \
do {                                 \
  size_t LEN = txt(S)->len;          \
  char V;                            \
  for (size_t I = 0; I < LEN; I++) { \
    V = txt_get_iso(S, I);           \
    if (F(V)) txt_push(S, V);        \
  }                                  \
  txt(S)->len -= LEN;                \
  memcpy(S, S + LEN, txt(T)->len);   \
} while (0)

#define txt_apply(S, F)                    \
do {                                       \
  for (size_t I = 0; I < txt(S)->len; I++) \
    txt_set(S, I, F(txt_get(S, I)));       \
} while (0)

#define txt_do(S, F)                       \
do {                                       \
  for (size_t I = 0; I < txt(S)->len; I++) \
    F(txt_get(S, I));                      \
} while (0)

// TODO: txt_foldl and txt_foldr

/*
** Macro Undef
*/

#undef PRINTF

/*
** Header-Only Mode
*/

#ifdef CIRCA_HEADER_ONLY
  #include "../c/txt.c"
#endif

#endif // CIRCA_TXT_H
