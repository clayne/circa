typedef struct {
  T **l;
  T **m;
  T **h;
  size_t m_cap;
  size_t l_len;
  size_t m_len;
  size_t h_len;
  size_t len;
} C2(hatc, T);

typedef struct {
  size_t fst;
  size_t snd;
} C3(hatc, T, idx);

static const size_t C3(hatc, T, siz) = 1 << C;
static const size_t C3(hatc, T, mask) = (1 << C) - 1;

static inline circa_gnu_attr(const) C3(hatc, T, idx) C3(hatc, T, locate)(size_t);

static void C3(hatc, T, alloc)(C2(hatc, T) *);
static void C3(hatc, T, free)(C2(hatc, T) *);

static inline T *C3(hatc, T, lookup)(C2(hatc, T) *, size_t);

static inline void C3(hatc, T, set_v)(C2(hatc, T) *, size_t, T);
static inline void C3(hatc, T, set_r)(C2(hatc, T) *, size_t, T *);
static inline T C3(hatc, T, get_v)(C2(hatc, T) *, size_t);
static inline void C3(hatc, T, get_r)(C2(hatc, T) *, size_t, T *x);

static void C3(hatc, T, snoc_v)(C2(hatc, T) *, T);
static void C3(hatc, T, snoc_r)(C2(hatc, T) *, T *);

static inline circa_gnu_attr(const)
C3(hatc, T, idx) C3(hatc, T, locate)(size_t idx) {
  return (C3(hatc, T, idx)) {
    idx >> C,
    idx & C3(hatc, T, mask)
  };
}

static
void C3(hatc, T, alloc)(C2(hatc, T) *h) {
  assert(h);
  h->l = NULL;
  h->m = malloc(1 * sizeof(*h->m));
  h->h = malloc(2 * sizeof(*h->h));
  h->m_cap = 1;
  h->l_len = 0;
  h->m_len = 0;
  h->h_len = 0;
  h->len = 0;
}

static
void C3(hatc, T, free)(C2(hatc, T) *h) {
  assert(h);
  for (size_t i = 0; i < h->m_len; i++)
    free(h->m[i]);
  free(h->l);
  free(h->m);
  free(h->h);
}

static inline
T *C3(hatc, T, lookup)(C2(hatc, T) *h, size_t i) {
  assert(h);
  assert(i < h->len);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(i);
  return h->m[idx.fst] + idx.snd;
}

static inline
void C3(hatc, T, set_v)(C2(hatc, T) *h, size_t i, T x) {
  assert(h);
  assert(i < h->len);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(i);
  h->m[idx.fst][idx.snd] = x;
}

static inline
void C3(hatc, T, set_r)(C2(hatc, T) *h, size_t i, T *x) {
  assert(h);
  assert(i < h->len);
  assert(x);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(i);
  h->m[idx.fst][idx.snd] = *x;
}

static inline
T C3(hatc, T, get_v)(C2(hatc, T) *h, size_t i) {
  assert(h);
  assert(i < h->len);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(i);
  return h->m[idx.fst][idx.snd];
}

static inline
void C3(hatc, T, get_r)(C2(hatc, T) *h, size_t i, T *x) {
  assert(h);
  assert(i < h->len);
  assert(x);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(i);
  *x = h->m[idx.fst][idx.snd];
}

static
void C3(hatc, T, snoc_v)(C2(hatc, T) *h, T x) {
  assert(h);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(h->len);
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
    h->m[h->m_len] = malloc(C3(hatc, T, siz) * sizeof(T));
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
  h->m[idx.fst][idx.snd] = x;
  h->len++;
}

static
void C3(hatc, T, snoc_r)(C2(hatc, T) *h, T *x) {
  assert(h);
  assert(x);
  C3(hatc, T, idx) idx = C3(hatc, T, locate)(h->len);
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
    h->m[h->m_len] = malloc(C3(hatc, T, siz) * sizeof(T));
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
