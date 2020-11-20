/*
** Description
**   An implementation of hashed array trees with size-doubling sub-blocks.
** Macro Parameters
**   T : Type = The type that the hashed array tree stores.
** Dependencies
**   <stdlib.h>
**   "circa/macro/cat.h"
**   "circa/macro/gnu.h"
**   "circa/algo/bits.h"
** Source
**   https://github.com/davidgarland/circa
*/

/*
** Type Definitions
*/

typedef struct {
  T **l;
  T **m;
  T **h;
  size_t m_cap;
  size_t l_len;
  size_t m_len;
  size_t h_len;
  size_t len;
} C2(hatd, T);

typedef struct {
  size_t fst;
  size_t snd;
} C3(hatd, T, idx);

/*
** Function Prototypes
*/

static circa_if_gnu(inline) circa_gnu_attr(const) C3(hatd, T, idx) C3(hatd, T, locate)(size_t);

static void C3(hatd, T, alloc)(C2(hatd, T) *);
static void C3(hatd, T, free)(C2(hatd, T) *);

static inline T *C3(hatd, T, lookup)(C2(hatd, T) *, size_t);
static inline void C3(hatd, T, set)(C2(hatd, T) *, size_t, T *);
static inline T C3(hatd, T, get)(C2(hatd, T) *, size_t);

static void C3(hatd, T, snoc)(C2(hatd, T) *, T *);

/*
** Function Implementations
*/

static circa_if_gnu(inline) circa_gnu_attr(const)
C3(hatd, T, idx) C3(hatd, T, locate)(size_t idx) {
  register const size_t w = idx + 1;
  register const size_t l = size_t_bsr(w);
  return (C3(hatd, T, idx)) {l, w - (1 << l)};
}

static
void C3(hatd, T, alloc)(C2(hatd, T) *h) {
  h->l = NULL;
  h->m = malloc(sizeof(*h->m));
  h->h = malloc(2 * sizeof(*h->h));
  h->m_cap = 1;
  h->l_len = 0;
  h->m_len = 0;
  h->h_len = 0;
  h->len = 0;
}

static
void C3(hatd, T, free)(C2(hatd, T) *h) {
  for (size_t i = 0; i < h->m_len; i++)
    free(h->m[i]);
  free(h->l);
  free(h->m);
  free(h->h);
}

static inline
T *C3(hatd, T, lookup)(C2(hatd, T) *h, size_t i) {
  assert(i < h->len);
  C3(hatd, T, idx) idx = C3(hatd, T, locate)(i);
  return h->m[idx.fst] + idx.snd;
}

static inline
void C3(hatd, T, set)(C2(hatd, T) *h, size_t i, T *x) {
  assert(i < h->len);
  C3(hatd, T, idx) idx = C3(hatd, T, locate)(i);
  h->m[idx.fst][idx.snd] = *x;
}

static inline
T C3(hatd, T, get)(C2(hatd, T) *h, size_t i) {
  assert(i < h->len);
  C3(hatd, T, idx) idx = C3(hatd, T, locate)(i);
  return h->m[idx.fst][idx.snd];
}

static
void C3(hatd, T, snoc)(C2(hatd, T) *h, T *x) {
  C3(hatd, T, idx) idx = C3(hatd, T, locate)(h->len);
  if (unlikely(idx.fst >= h->m_len)) {
    if (unlikely(idx.fst >= h->m_cap)) {
      h->m_cap <<= 1;
      free(h->l);
      h->l = h->m;
      h->l_len = h->m_len;
      h->m = h->h;
      h->h = malloc((h->m_cap << 1) * sizeof(*h->h));
      h->h_len = 0;
    }
    h->m[h->m_len] = malloc((1 << h->m_len) * sizeof(T));
    h->m_len++;
    if (likely((h->m_len - h->h_len) > 0)) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++;
    }
    if (likely((h->m_len - h->h_len) > 0)) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++;
    }
  }
  h->m[idx.fst][idx.snd] = *x;
  h->len++;
}
