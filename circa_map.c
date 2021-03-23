#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_map.h"
  #define circa_static
#endif

circa_static
circa_err C4(map, K, V, alloc)(C3(map, K, V) *const restrict m) {
  circa_nullck(m);
  m->keys = malloc(2 * sizeof(K));
  circa_oomck(m->keys);
  m->vals = malloc(2 * sizeof(V));
  circa_if_oomck(!m->vals) {
    free(m->keys);
    circa_throw(CE_OOM, "Out of memory.");
  }
  m->psls = calloc(2, sizeof(size_t));
  circa_if_oomck(!m->psls) {
    free(m->keys);
    free(m->vals);
    circa_throw(CE_OOM, "Out of memory.");
  }
  m->cap = 2;
  m->len = 0;
  return CE_NONE;
}

static
circa_err C4(map, K, V, alloc_cap)(C3(map, K, V) *const restrict m, register const size_t cap) {
  circa_nullck(m);
  circa_oneck(cap);
  m->keys = malloc(cap * sizeof(K));
  circa_oomck(m->keys);
  m->vals = malloc(cap * sizeof(V));
  circa_if_oomck(!m->vals) {
    free(m->keys);
    circa_throw(CE_OOM, "Out of memory.");
  }
  m->psls = calloc(cap, sizeof(size_t));
  circa_if_oomck(!m->psls) {
    free(m->keys);
    free(m->vals);
    circa_throw(CE_OOM, "Out of memory.");
  }
  m->cap = cap;
  m->len = 0;
  return CE_NONE;
}


circa_static
circa_err C4(map, K, V, free)(C3(map, K, V) *m) {
  circa_nullck(m);
  free(m->keys);
  free(m->vals);
  free(m->psls);
  return CE_NONE;
}

circa_static
circa_err C4(map, K, V, rehash)(C3(map, K, V) *m) {
  circa_nullck(m);
  C3(map, K, V) tmp;
  register const circa_err e = C4(map, K, V, alloc_cap)(&tmp, m->cap << 1);
  circa_if_oomck (e) return e;
  for (size_t i = 0; i < m->cap; i++) {
    if (m->psls[i] != 0) {
      register const circa_err e = C4(map, K, V, set)(&tmp, m->keys + i, m->vals + i);
      circa_if_oomck (e) return e;
    }
  }
  C4(map, K, V, free)(m);
  *m = tmp;
  return CE_NONE;
}

circa_static
circa_err C4(map, K, V, set_with_hash)(C3(map, K, V) *const restrict m, const K *const restrict k, const V *const restrict v, size_t hash) {
  circa_nullck(m);
  circa_nullck(k);
  circa_nullck(v);
  K      swp_key = *k, tmp_key;
  V      swp_val = *v, tmp_val;
  register size_t swp_psl = 1, tmp_psl;
  register const size_t initial_pos = hash & (m->cap - 1);
  register size_t i = initial_pos;
  register bool found = false;
loop:
  if (m->psls[i] == 0) { m->len++; goto found; } // if empty spot, take
  if (C2(K, cmp)(m->keys + i, &swp_key) == EQ) goto found; // if match, overwrite
  if (m->psls[i] < swp_psl) { // if rich, steal from
    tmp_key = m->keys[i];
    tmp_val = m->vals[i];
    tmp_psl = m->psls[i];
    m->keys[i] = swp_key; // TODO: this swapping is a bit verbose. is a
    m->vals[i] = swp_val; // "C2(T, swp)" function in order?
    m->psls[i] = swp_psl;
    swp_key = tmp_key;
    swp_val = tmp_val;
    swp_psl = tmp_psl;
  }
  swp_psl++;
  i++;
  i &= (m->cap - 1); // if overflow, wrap back around
  if (i == initial_pos) goto not_found;
  goto loop;
found:
  m->psls[i] = swp_psl;
  m->keys[i] = swp_key;
  m->vals[i] = swp_val;
  return CE_NONE;
not_found:;
  circa_err e = C4(map, K, V, rehash)(m);
  circa_if_oomck(e) return e;
  return C4(map, K, V, set_with_hash)(m, &swp_key, &swp_val, C2(K, hash)(&swp_key));
}

circa_static
circa_err C4(map, K, V, set)(C3(map, K, V) *const restrict m, const K *const restrict k, const V *const restrict v) {
  circa_nullck(m);
  circa_nullck(k);
  circa_nullck(v);
  return C4(map, K, V, set_with_hash)(m, k, v, C2(K, hash)(k));
}

circa_static
circa_err C4(map, K, V, get_with_hash)(C3(map, K, V) *const restrict m, const K *const restrict k, V *const restrict v, register const size_t hash) {
  circa_nullck(m);
  circa_nullck(k);
  circa_nullck(v);
  V *const restrict r = C4(map, K, V, lookup_with_hash)(m, k, hash);
  circa_oobck(r);
  *v = *r;
  return CE_NONE;
}

circa_static
circa_err C4(map, K, V, get)(C3(map, K, V) *const restrict m, const K *const restrict k, V *const restrict v) {
  circa_nullck(m);
  circa_nullck(k);
  circa_nullck(v);
  return C4(map, K, V, get_with_hash)(m, k, v, C2(K, hash)(k));
}

circa_static
V *C4(map, K, V, lookup)(C3(map, K, V) *const restrict m, const K *const restrict k) {
  circa_if_oomck (!m) return NULL;
  circa_if_oomck (!k) return NULL;
  return C4(map, K, V, lookup_with_hash)(m, k, C2(K, hash)(k));
}

circa_static
V *C4(map, K, V, lookup_with_hash)(C3(map, K, V) *const restrict m, const K *const restrict k, register const size_t hash) {
  circa_if_oomck (!m) return NULL;
  circa_if_oomck (!k) return NULL;
  register const size_t initial_pos = hash & (m->cap - 1);
  register size_t i = initial_pos;
  if (m->psls[i] == 0) return NULL; // early out if the initial pos is untouched
  if (m->psls[i] == 1) if (C2(K, cmp)(m->keys + i, k) == EQ) return m->vals + i; // early out if first is a match
  register size_t psl = 0;
  do {
    psl++;
    i++;
    i &= (m->cap - 1);
    if (m->psls[i] == 0) return NULL; // early out if we hit an unoccupied node
    if (m->psls[i] < psl) return NULL; // early out using the robin hood invariant
    if (C2(K, cmp)(m->keys + i, k) == EQ) return m->vals + i; // end when we find the key
  } while (i != initial_pos);
  return NULL;
}

circa_static
bool C4(map, K, V, has)(C3(map, K, V) *const restrict m, const K *const restrict k) {
  return C4(map, K, V, lookup)(m, k) != NULL;
}

circa_static
circa_err C4(map, K, V, del)(C3(map, K, V) *const restrict m, const K *const restrict k) {
  circa_nullck(m);
  circa_nullck(k);
  // TODO implement
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
