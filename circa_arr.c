#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_arr.h"
  #define circa_static
#endif

circa_static
circa_err C3(arr, T, alloc)(C2(arr, T) *const restrict a) {
  circa_nullck(a);
  a->data = malloc(2 * sizeof(T));
  circa_oomck(a->data);
  a->cap = 2;
  a->len = 0;
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, prealloc)(C2(arr, T) *const restrict a, register const size_t cap) {
  circa_nullck(a);
  circa_oneck(cap);
  a->data = malloc(cap * sizeof(T));
  circa_oomck(a->data);
  a->cap = cap;
  a->len = 0;
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, amort)(C2(arr, T) *const restrict a) {
  circa_nullck(a);
  if_unlikely (a->len == a->cap) {
    a->cap += (a->cap >> 1);
    a->data = realloc(a->data, a->cap * sizeof(T));
    circa_oomck(a->data);
  }
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, free)(C2(arr, T) *const restrict a) {
  circa_nullck(a);
  free(a->data);
  return CE_NONE;
}

circa_static
T *C3(arr, T, lookup)(C2(arr, T) *const restrict a, register const size_t idx) {
  // TODO bounds + null checking
  return a->data + idx;
}

circa_static
circa_err C3(arr, T, set_v)(C2(arr, T) *const restrict a, register const size_t idx, T v) {
  circa_nullck(a);
  a->data[idx] = v;
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, set_r)(C2(arr, T) *const restrict a, register const size_t idx, T *const restrict v) {
  circa_nullck(a);
  circa_oobck(idx < a->len);
  circa_nullck(v);
  return CE_NONE;
}

circa_static
T C3(arr, T, get_v)(C2(arr, T) *const restrict a, register const size_t idx) {
  // TODO: bounds + null checking
  return a->data[idx];
}

circa_static
circa_err C3(arr, T, get_r)(C2(arr, T) *const restrict a, register const size_t idx, T *const restrict v) {
  circa_nullck(a);
  circa_oobck(idx < a->len);
  circa_nullck(v);
  *v = a->data[idx];
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, snoc_v)(C2(arr, T) *const restrict a, register const T v) {
  circa_nullck(a);
  register const circa_err r = C3(arr, T, amort)(a);
  circa_if_oomck (r) circa_throw(r, "Ran out of memory.");
  a->data[a->len++] = v;
  return CE_NONE;
}

circa_static
circa_err C3(arr, T, snoc_r)(C2(arr, T) *const restrict a, T *const restrict v) {
  circa_nullck(a);
  circa_nullck(v);
  register const circa_err r = C3(arr, T, amort)(a);
  circa_if_oomck (r) circa_throw(r, "Ran out of memory.");
  a->data[a->len++] = *v;
  return CE_NONE;
}

circa_static
T *C3(arr, T, reserve)(C2(arr, T) *const restrict a) {
  register const circa_err r = C3(arr, T, amort)(a);
  circa_if_oomck(r) return NULL;
  return a->data + a->len++;
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
