/*
** txt.h | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_TXT_H
#define CIRCA_TXT_H

#include "core.h"
#include "slice.h"
#include "seq.h"

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpadded"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wcast-align"
#endif

/*
** Dynamic text, much like the dynamic sequences, use "fat pointers" in order to
** store their data. This means that the pointer the end programmer carries
** around is actually to the `data` field in the structure, and getting the
** other fields is done using negative pointer indexing. What this allows us to
** do is have a string whose capacity and length are passed around with it
** cleanly while still maintaining compatibility with C string functions.
*/

struct txt_data {
  size_t cap;
  size_t len;
  char data[];
};

/*
** For convenience and style, we define a `Txt` type that's just a wrapper over
** `char *`.
*/

typedef char *Txt;

/*
** Then we need some accessor function prototypes, to allow us to view the
** underlying structure and to safely index the string by respecting its
** capacity and length. Because `Txt` isn't generic, we don't need quite as
** many macros as `Seq` does.
*/

static inline struct txt_data *txt(Txt t);

static inline void txt_clear(Txt t);

#define txt_set(T, A, V) (T) = txt_set_((T), (A), (V))
static inline Txt txt_set_(Txt t, size_t a, char v);

static inline bool txt_has(Txt t, size_t a);

static inline char txt_get(Txt t, size_t a);

/*
** Then some allocators.. nothing especially noteworthy here.
*/

static inline Txt txt_alloc(size_t cap);

#define txt_realloc(T, C) (T) = txt_realloc_((T), (C))
static inline Txt txt_realloc_(Txt t, size_t cap);

#define txt_require(T, C) (T) = txt_require_((T), (C))
static inline Txt txt_require_(Txt t, size_t cap);

static inline Txt txt_wrap(char *c, size_t len);

#define txt_free(T) (T) = txt_free_(T)
static inline Txt txt_free_(Txt t);

/*
** Next we have some stack functions, which allows us to easily append
** characters to a string or remove them one by one quickly.
**
** `txt_pop_` is only made into two macros because it would be inefficient to
** have another function just for checking the top of the stack-- instead we
** just have an argument specifiying how many elements to discard.
*/

#define txt_push(T, V) (T) = txt_push_((T), (V))
static inline Txt txt_push_(Txt t, char v);

#define txt_tos(T) txt_pop_((T), 0)
#define txt_pop(T) txt_pop_((T), 1)
static inline char txt_pop_(Txt t, size_t n);

/*
** Then there are our main string functions; comparison, upper, lower, checking
** for nil or empty, etc.
*/

bool txt_cmp(Txt a, Txt b);
bool txt_cmp_len(Txt a, Txt b, size_t len);
bool txt_cmp_slice(Txt a, Slice sa, Txt b, Slice sb);
bool txt_cmp_slice_lit(Txt t, Slice s, char *c);

/*
** And finally there are some basic IO functions, which just take a function
** pointer for flexibility and speed reasons.
*/

#define txt_read(T, F) (T) = txt_read_((T), (F))
Txt txt_read_(Txt t, FILE *fp);

void txt_write(Txt t, FILE *fp);

/*
** Accessors
*/

static inline
struct txt_data *txt(Txt t) {
  return ((struct txt_data*) t) - 1;
}

static inline
void txt_clear(Txt t) {
  return seq_clear_(1, t);
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
** Allocators
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
** Stack Operations
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
** And finally some iteration macros.
*/

#define txt_foreach_iso(T, V) \
for (size_t I = 0, J = 0; I < txt(T)->len; I++, J = 0) \
for (char V = txt_get(T, I); J != 1; J = 1)
#define txt_foreach(T, V) txt_foreach_iso(T, V)

#ifdef __clang__
  #pragma clang diagnostic pop // -Wcast-align
  #pragma clang diagnostic pop // -Wpadded
#endif

#endif // CIRCA_TXT_H
