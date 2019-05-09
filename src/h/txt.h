/*
** txt.h | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_TXT_H
#define CIRCA_TXT_H

/*
** Dependencies
*/

#include "core.h"
#include "slice.h"
#include "seq.h"

/*
** Type Definitions
*/

struct txt_data {
  size_t cap;
  size_t len;
  char data[];
};

typedef char *Txt;

/*
** Accessors
*/

static inline struct txt_data *txt(Txt t);

static inline void txt_clear(Txt t);

#define txt_set(T, A, V) (T) = txt_set_((T), (A), (V))
static inline Txt txt_set_(Txt t, size_t a, char v);

static inline bool txt_has(Txt t, size_t a);

static inline char txt_get(Txt t, size_t a);

/*
** Allocators
*/

static inline Txt txt_alloc(size_t cap);

#define txt_realloc(T, C) (T) = txt_realloc_((T), (C))
static inline Txt txt_realloc_(Txt t, size_t cap);

#define txt_require(T, C) (T) = txt_require_((T), (C))
static inline Txt txt_require_(Txt t, size_t cap);

#define txt_lit(C) txt_wrap((C), strlen((C)))
static inline Txt txt_wrap(char *c, size_t len);

#define txt_free(T) (T) = txt_free_(T)
static inline Txt txt_free_(Txt t);

/*
** String Functions
*/

#define txt_cpy_len(T, C, L) (T) = txt_cpy_((T), (C), (L))
#define txt_cpy_lit(T, C) txt_cpy_len(T, C, strlen(C))
#define txt_cpy(T, C) txt_cpy_len(T, C, txt(C)->len)
Txt txt_cpy_(Txt t, char *c, size_t len);

#define txt_cpy_slice(T, C, S) (T) = txt_cpy_slice_((T), (C), (S))
Txt txt_cpy_slice_(Txt t, char *c, Slice s);

/*
** Stack Functions
*/

#define txt_push(T, V) (T) = txt_push_((T), (V))
static inline Txt txt_push_(Txt t, char v);

#define txt_tos(T) txt_pop_((T), 0)
#define txt_pop(T) txt_pop_((T), 1)
static inline char txt_pop_(Txt t, size_t n);

/*
** Comparison Functions
*/

static inline bool txt_cmp(Txt a, Txt b);
static inline bool txt_cmp_len(Txt a, Txt b, size_t len);
static inline bool txt_cmp_slice(Txt a, Slice sa, Txt b, Slice sb);
bool txt_cmp_slice_lit(Txt t, Slice s, char *c);

/*
** IO Functions
*/

#define txt_read(T, F) (T) = txt_read_((T), (F))
Txt txt_read_(Txt t, FILE *fp);

#define txt_cat_read(T, F) (T) = txt_cat_read_((T), (F))
Txt txt_cat_read_(Txt t, FILE *fp);

void txt_write(Txt t, FILE *fp);

/*
** Accessors Implementation
*/

static inline
struct txt_data *txt(Txt t) {
  return ((struct txt_data*) t) - 1;
}

static inline
void txt_clear(Txt t) {
  seq_clear_(1, t);
}

static inline
Txt txt_set_(Txt t, size_t a, char v) {
  return seq_set_(1, t, a, &v);
}

static inline
bool txt_has_(Txt t, size_t a) {
  return a < txt(t)->len;
}

static inline
char txt_get(Txt t, size_t a) {
  void *p = seq_get_(1, t, a);
  return p ? *((char*) p) : '\0';
}

/*
** Allocators Implementation
*/

static inline
Txt txt_alloc(size_t cap) {
  return seq_alloc_(1, cap);
}

static inline
Txt txt_wrap(char *c, size_t len) {
  return seq_wrap_(1, len + 1, c);
}

static inline
Txt txt_realloc_(Txt t, size_t cap) {
  return seq_realloc_(1, t, cap);
}

static inline
Txt txt_require_(Txt t, size_t cap) {
  return seq_require_(1, t, cap);
}

static inline
Txt txt_free_(Txt t) {
  return seq_free_(t);
}

/*
** Stack Functions Implementation
*/

static inline
Txt txt_push_(Txt t, char v) {
  return seq_push_(1, t, &v);
}

static inline
char txt_pop_(Txt t, size_t n) {
  void *p = seq_pop_(1, t, n);
  return p ? *((char*) p) : '\0';
}

/*
** String Functions Implementation
*/

static inline
bool txt_cmp(Txt a, Txt b) {
  return seq_cmp_(1, a, b);
}

static inline
bool txt_cmp_len(Txt a, Txt b, size_t len) {
  return seq_cmp_len_(1, a, b, len);
}

static inline
bool txt_cmp_slice(Txt a, Slice sa, Txt b, Slice sb) {
  return seq_cmp_slice_(1, a, sa, b, sb);
}

/*
** Iterators
*/

#define txt_foreach_iso(T, V) \
for (size_t I = 0, J = 0; I < txt(T)->len; I++, J = 0) \
for (char V = txt_get(T, I); J != 1; J = 1)
#define txt_foreach(T, V) txt_foreach_iso(T, V)

#endif // CIRCA_TXT_H
