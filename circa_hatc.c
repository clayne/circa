#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_hatc.h"
  #define circa_static
#endif

static const size_t C3(hatc, T, siz) = 1 << C;
static const size_t C3(hatc, T, mask) = (1 << C) - 1;

typedef struct {
  size_t fst;
  size_t snd;
} C3(hatc, T, idx);

circa_static
circa_err C3(hatc, T, alloc)(C2(hatc, T) *const restrict h) {
  circa_nullck(h);
  h->l = NULL;
  h->m = malloc(sizeof(*h->m));
  circa_oomck(h->m);
  h->h = malloc(sizeof(*h->h) << 1);
  circa_if_oomck(!h->h) {
    free(h->m);
    circa_throw(CE_OOM, "Out of memory.");
  }
  h->m_cap = 1;
  h->l_len = 0;
  h->m_len = 0;
  h->h_len = 0;
  h->len = 0;
  return CE_NONE;
}

circa_static
circa_err C3(hatc, T, free)(C2(hatc, T) *const restrict h) {
  circa_nullck(h);
  free(h->l);
  free(h->h);
  for (size_t i = 0; i < h->m_len; i++)
    free(h->m[i]);
  free(h->m);
  return CE_NONE;
}

static
C3(hatc, T, idx) C3(hatc, T, index)(register const size_t i) {
  return (C3(hatc, T, idx)) {
    i >> C,
    i & C3(hatc, T, mask)
  };
}

circa_static
T *C3(hatc, T, lookup)(C2(hatc, T) *const restrict h, register const size_t i) {
  circa_if_nullck (!h) return NULL;
  circa_if_oobck (i >= h->len) return NULL;
  register const C3(hatc, T, idx) idx = C3(hatc, T, index)(i);
  return h->m[idx.fst] + idx.snd;
}

circa_static
circa_err C3(hatc, T, set_v)(C2(hatc, T) *const restrict h, register const size_t i, register const T x) {
  circa_nullck(h);
  circa_oobck(i < h->len);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(i);
  h->m[idx.fst][idx.snd] = x;
  return CE_NONE;
}

circa_static
circa_err C3(hatc, T, set_r)(C2(hatc, T) *const restrict h, register const size_t i, T *const restrict x) {
  circa_nullck(h);
  circa_oobck(i < h->len);
  circa_nullck(x);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(i);
  h->m[idx.fst][idx.snd] = *x;
  return CE_NONE;
}

circa_static
T C3(hatc, T, get_v)(C2(hatc, T) *const restrict h, register const size_t i) {
  // TODO: null checking, out of bounds checking
  C3(hatc, T, idx) idx = C3(hatc, T, index)(i);
  return h->m[idx.fst][idx.snd];
}

circa_static
circa_err C3(hatc, T, get_r)(C2(hatc, T) *const restrict h, register const size_t i, T *const restrict x) {
  circa_nullck(h);
  circa_oobck(i < h->len);
  circa_nullck(x);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(i);
  *x = h->m[idx.fst][idx.snd];
}

circa_static
circa_err C3(hatc, T, snoc_v)(C2(hatc, T) *const restrict h, register const T x) {
  circa_nullck(h);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(h->len);
  if (idx.fst == h->m_len) {
    if (idx.fst == h->m_cap) {
      h->m_cap <<= 1;
      free(h->l);
      h->l = h->m;
      h->l_len = h->m_len;
      h->m = h->h;
      h->h = malloc((h->m_cap << 1) * sizeof(*h->h));
      h->h_len = 0;
    }
    h->m[h->m_len] = malloc(C3(hatc, T, siz) * sizeof(T));
    // TODO: OOM checking & rollback here
    h->m_len++;
    if ((h->m_len - h->h_len) > 0) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++;
    }
    if ((h->m_len - h->h_len) > 0) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++; 
    }
  }
  h->m[idx.fst][idx.snd] = x;
  h->len++;
  return CE_NONE;
}

circa_static
circa_err C3(hatc, T, snoc_r)(C2(hatc, T) *const restrict h, T *const restrict x) {
  circa_nullck(h);
  circa_nullck(x);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(h->len);
  if (idx.fst == h->m_len) {
    if (idx.fst == h->m_cap) {
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
    if ((h->m_len - h->h_len) > 0) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++;
    }
    if ((h->m_len - h->h_len) > 0) {
      h->h[h->h_len] = h->m[h->h_len];
      h->h_len++; 
    }
  }
  h->m[idx.fst][idx.snd] = *x;
  h->len++;
  return CE_NONE;
}

circa_static
T C3(hatc, T, pop_v)(C2(hatc, T) *const restrict h) {
  // TODO: null checking, bounds checking
  C3(hatc, T, idx) idx = C3(hatc, T, index)(h->len - 1);
  printf("%zu, %zu\n", idx.fst, idx.snd);
  T x = h->m[idx.fst][idx.snd];
  if (idx.fst < (h->m_len - 1)) {
    free(h->m[h->m_len - 1]);
    if (idx.fst < (h->m_cap >> 1)) {
      h->m_cap >>= 1;
      free(h->h);
      h->h = h->m;
      h->h_len = h->m_len;
      h->m = h->l;
      h->l = malloc(size_t_max(1, h->m_cap >> 1) * sizeof(*h->l));
      // TODO: OOM checking & rollback here
      h->l_len = 0;
    }
    h->m_len--;
    if ((h->m_cap >> 1) > h->l_len) {
      h->l[h->l_len] = h->m[h->l_len];
      h->l_len++;
    }
    if ((h->m_cap >> 1) > h->l_len) {
      h->l[h->l_len] = h->m[h->l_len];
      h->l_len++;
    }
  }
  h->len--;
  return x;
}

circa_static
circa_err C3(hatc, T, pop_r)(C2(hatc, T) *const restrict h, T *const restrict x) {
  circa_nullck(h);
  circa_oobck(h->len > 0);
  circa_nullck(x);
  C3(hatc, T, idx) idx = C3(hatc, T, index)(h->len - 1);
  *x = h->m[idx.fst][idx.snd];
  if (idx.fst < (h->m_len - 1)) {
    free(h->m[h->m->len - 1]);
    if (idx.fst < (h->m_cap >> 1)) {
      h->m_cap >>= 1;
      free(h->h);
      h->h = h->m;
      h->h_len = h->m_len;
      h->m = h->l;
      h->l = malloc(size_t_max(1, h->m_cap >> 1) * sizeof(*h->l));
      // TODO: OOM checking & rollback here
      h->l_len = 0;
    }
    h->m_len--;
    if ((h->m_cap >> 1) > h->l_len) {
      h->l[h->l_len] = h->m[h->l_len];
      h->l_len++;
    }
    if ((h->m_cap >> 1) > h->l_len) {
      h->l[h->l_len] = h->m[h->l_len];
      h->l_len++;
    }
  }
  h->len--;
  return CE_NONE;
}


#undef circa_static

/*
** Copyright 2020 David Garland
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/
