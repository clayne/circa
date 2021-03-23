#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef struct {
  K *keys;      // the array of map keys
  V *vals;      // the array of map values
  size_t *psls; // the array of probe sequence lengths
  size_t *hshs; // the array of key hashes (currently unimplemented)
  size_t cap;   // the capacity (amount allocated)
  size_t len;   // the length (amount used)
} C3(map, K, V);

circa_static circa_err C4(map, K, V, alloc)(C3(map, K, V) *const restrict);
circa_static circa_err C4(map, K, V, free)(C3(map, K, V) *const restrict);

circa_static circa_err C4(map, K, V, set)(C3(map, K, V) *const restrict, const K *const restrict, const V *const restrict); 
circa_static circa_err C4(map, K, V, set_with_hash)(C3(map, K, V) *const restrict, const K *const restrict, const V *const restrict, register const size_t);

circa_static V *C4(map, K, V, lookup)(C3(map, K, V) *const restrict, const K *const restrict);
circa_static V *C4(map, K, V, lookup_with_hash)(C3(map, K, V) *const restrict, const K *const restrict, register const size_t);

circa_static circa_err C4(map, K, V, get)(C3(map, K, V) *const restrict, const K *const restrict, V *const restrict);
circa_static circa_err C4(map, K, V, get_with_hash)(C3(map, K, V) *const restrict, const K *const restrict, V *const restrict, register const size_t);

circa_static bool C4(map, K, V, has)(C3(map, K, V) *const restrict, const K *const restrict);

circa_static circa_err C4(map, K, V, del)(C3(map, K, V) *const restrict, const K *const restrict);

#ifndef map_foreach
  #define map_foreach(M, KN, VN) \
    for (size_t I = 0, J = 0; I < M.cap; I++, J = 0) \
      if (M.psls[I]) \
        for (__auto_type KN = M.keys[I]; J != 1; J = 1) \
          for (__auto_type VN = M.vals[I]; J != 1; J = 1)
#endif

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_map.c"
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
