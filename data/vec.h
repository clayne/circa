/*
** Description
**   An implementation of vectors with amortized growth.
** Macro Parameters
**   T : Type = The type that the vector stores.
** Dependencies
**   <stdlib.h>
**   "circa/macro/cat.h"
**   "circa/macro/gnu.h"
** Source
**   https://github.com/davidgarland/circa
*/

/*
** Type Definitions
*/

typedef struct {
  T *data;
  size_t len;
  size_t cap;
} C2(vec, T);

/*
** Function Prototypes
*/

static inline void C3(vec, T, alloc)(C2(vec, T) *v);
static void C3(vec, T, amort)(C2(vec, T) *v);
static inline void C3(vec, T, free)(C2(vec, T) *v);

static inline T *C3(vec, T, lookup)(C2(vec, T) *, size_t);
static inline void C3(vec, T, set)(C2(vec, T) *, size_t, T *);
static inline T C3(vec, T, get)(C2(vec, T) *, size_t);
static inline void C3(vec, T, snoc)(C2(vec, T) *, T *);

/*
** Function Implementations
*/

static inline
void C3(vec, T, alloc)(C2(vec, T) *v) {
  assert(v);
  v->data = malloc(2 * sizeof(T));
  assert(v->data);
  v->len = 0;
  v->cap = 2;
}

static
void C3(vec, T, amort)(C2(vec, T) *v) {
  assert(v);
  if (unlikely(v->len == v->cap)) {
    v->cap += v->cap >> 1;
    v->data = realloc(v->data, v->cap * sizeof(T));
  }
}

static inline
void C3(vec, T, free)(C2(vec, T) *v) {
  assert(v);
  free(v->data);
}

static inline
T *C3(vec, T, lookup)(C2(vec, T) *v, size_t idx) {
  assert(v);
  assert(idx < v->len);
  return v->data + idx;
}

static inline
void C3(vec, T, set)(C2(vec, T) *v, size_t idx, T *x) {
  assert(v);
  assert(idx < v->len);
  assert(x);
  v->data[idx] = *x;
}

static inline
T C3(vec, T, get)(C2(vec, T) *v, size_t idx) {
  assert(v);
  assert(idx < v->len);
  return v->data[idx];
}

static inline
void C3(vec, T, snoc)(C2(vec, T) *v, T *x) {
  assert(v);
  assert(x);
  C3(vec, T, amort)(v);
  v->data[v->len++] = *x;
}
