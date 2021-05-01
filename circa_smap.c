#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_smap.h"
  #define circa_static
#endif

circa_static
circa_err C4(smap, K, V, alloc)(C3(smap, K, V) *const restrict sm, register const size_t len) {
  circa_nullck(sm);
  circa_zerock(len);
  return C4(smap, K, V, alloc_cap)(sm, len, 2);
}

circa_static
circa_err C4(smap, K, V, alloc_cap)(C3(smap, K, V) *const restrict sm, register const size_t len, register const size_t cap) {
  circa_nullck(sm);
  circa_zerock(len);
  circa_zerock(cap);
  sm->amaps = aligned_alloc(128, len * sizeof(C3(amap, K, V)));
  for (size_t i = 0; i < len; i++) {
    C4(map, K, V, alloc_cap)(&sm->amaps[i].map, cap);
    mtx_init(&sm->amaps[i].lock, mtx_plain);
  }
  sm->len = len;
  return CE_NONE;
}

circa_static
circa_err C4(smap, K, V, free)(C3(smap, K, V) *const restrict sm) {
  circa_nullck(sm);
  for (size_t i = 0; i < sm->len; i++) {
    C4(map, K, V, free)(&sm->amaps[i].map);
    mtx_destroy(&sm->amaps[i].lock);
  }
  free(sm->amaps);
  return CE_NONE;
}

circa_static
circa_err C4(smap, K, V, set)(C3(smap, K, V) *const restrict sm, K *const restrict k, const V *const restrict v) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  return C4(smap, K, V, set_with_hash)(sm, k, v, C2(K, hash)(k));
}

circa_static
circa_err C4(smap, K, V, set_with_hash)(C3(smap, K, V) *const restrict sm, K *const restrict k, const V *const restrict v, register const size_t hash) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  register const size_t pos = (hash >> (sizeof(size_t) << 2)) & (sm->len - 1);
  mtx_lock(&sm->amaps[pos].lock);
  circa_err e = C4(map, K, V, set_with_hash)(&sm->amaps[pos].map, k, v, hash);
  mtx_unlock(&sm->amaps[pos].lock);
  return e;
}

circa_static
circa_err C4(smap, K, V, get)(C3(smap, K, V) *const restrict sm, K *const restrict k, V *const restrict v) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  return C4(smap, K, V, get_with_hash)(sm, k, v, C2(K, hash)(k));
}

circa_static
circa_err C4(smap, K, V, get_with_hash)(C3(smap, K, V) *const restrict sm, K *const restrict k, V *const restrict v, register const size_t hash) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  register const size_t pos = (hash >> (sizeof(size_t) << 2)) & (sm->len - 1);
  mtx_lock(&sm->amaps[pos].lock);
  circa_err e = C4(map, K, V, get_with_hash)(&sm->amaps[pos].map, k, v, hash);
  mtx_unlock(&sm->amaps[pos].lock);
  return e;
}

circa_static
circa_err C4(smap, K, V, try_set)(C3(smap, K, V) *const restrict sm, K *const restrict k, V *const restrict v) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  return C4(smap, K, V, try_set_with_hash)(sm, k, v, C2(K, hash)(k));
}

circa_static
circa_err C4(smap, K, V, try_set_with_hash)(C3(smap, K, V) *const restrict sm, K *const restrict k, V *const restrict v, register const size_t hash) {
  circa_nullck(sm);
  circa_nullck(k);
  circa_nullck(v);
  register const size_t pos = (hash >> (sizeof(size_t) << 2)) & (sm->len - 1);
  mtx_lock(&sm->amaps[pos].lock);
  V *mv = C4(map, K, V, lookup_with_hash)(&sm->amaps[pos].map, k, hash);
  if (!mv) {
    circa_err e = C4(map, K, V, set)(&sm->amaps[pos].map, k, v);
    circa_if_oomck(e) return e;
  } else {
    *v = *mv;
  }
  mtx_unlock(&sm->amaps[pos].lock);
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
