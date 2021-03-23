#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef struct {
  _Alignas(128) C3(map, K, V) map;
  mtx_t lock;
} C3(amap, K, V);

typedef struct {
  _Alignas(128) C3(amap, K, V) *amaps;
  _Alignas(128) size_t len;
} C3(smap, K, V);

circa_static circa_err C4(smap, K, V, alloc)(C3(smap, K, V) *const restrict, register const size_t);
circa_static circa_err C4(smap, K, V, alloc_cap)(C3(smap, K, V) *const restrict, register const size_t, register const size_t);
circa_static circa_err C4(smap, K, V, free)(C3(smap, K, V) *const restrict);

circa_static circa_err C4(smap, K, V, set)(C3(smap, K, V) *const restrict, const K *const restrict, const V *const restrict);
circa_static circa_err C4(smap, K, V, set_with_hash)(C3(smap, K, V) *const restrict, const K *const restrict, const V *const restrict, register const size_t);

circa_static circa_err C4(smap, K, V, get)(C3(smap, K, V) *const restrict, const K *const restrict, V *const restrict);
circa_static circa_err C4(smap, K, V, get_with_hash)(C3(smap, K, V) *const restrict, const K *const restrict, V *const restrict, register const size_t);

circa_static circa_err C4(smap, K, V, try_set)(C3(smap, K, V) *const restrict, const K *const restrict, V *const restrict);
circa_static circa_err C4(smap, K, V, try_set_with_hash)(C3(smap, K, V) *const restrict, const K *const restrict, V *const restrict, register const size_t);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_smap.c"
#endif

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
