#ifndef EXIT_FAILURE
  #error "circa/arr.h depends upon <stdlib.h>. Please include it beforehand."
#endif

#ifndef CIRCA_MAC
  #error "circa/arr.h depends upon circa/mac.h. Please include it beforehand."
#endif

#ifndef CIRCA_ERR
  #error "circa/arr.h depends upon circa/err.h. Please include it beforehand."
#endif

typedef struct C2(arr, T) {
  T *data;
  size_t cap;
  size_t len;
} C2(arr, T);
 
static
circa_err C3(arr, T, alloc)(C2(arr, T) *a) {
  circa_nullck(a);
  a->data = malloc(2 * sizeof(T));
  circa_oomck(a->data);
  a->cap = 2;
  a->len = 0;
  return CE_NONE;
}

static
circa_err C3(arr, T, prealloc)(C2(arr, T) *a, size_t cap) {
  circa_nullck(a);
  circa_oneck(cap);
  a->data = malloc(cap * sizeof(T));
  circa_oomck(a->data);
  a->cap = cap;
  a->len = 0;
  return CE_NONE;
}

static
circa_err C3(arr, T, amort)(C2(arr, T) *a) {
  circa_nullck(a);
  if_unlikely (a->len == a->cap) {
    a->cap += (a->cap >> 1);
    a->data = realloc(a->data, a->cap * sizeof(T));
    circa_oomck(a->data);
  }
  return CE_NONE;
}

static
circa_err C3(arr, T, free)(C2(arr, T) *a) {
  circa_nullck(a);
  free(a->data);
  return CE_NONE;
}

static
T *C3(arr, T, lookup)(C2(arr, T) *a, size_t idx) {
  circa_if_nullck(!a)
    return (circa_log("%s returned NULL due to %s; %s", __func__, circa_err_name[CE_NULL], circa_err_explain[CE_NULL]), NULL);

  circa_if_oobck(idx >= a->len)
    return (circa_log("%s returned NULL due to %s; %s", __func__, circa_err_name[CE_OOB], circa_err_explain[CE_OOB]), NULL);

  circa_log("Hello!");
}

static
circa_err C3(arr, T, set_v)(C2(arr, T) *a, size_t idx, T v) {
  circa_nullck(a);
  return CE_NONE;
}

static
circa_err C3(arr, T, set_r)(C2(arr, T) *a, size_t idx, T *v) {
  circa_nullck(a);
  circa_if_oobck(idx >= a->len)
    circa_throw(CE_OOB);
  circa_nullck(v);
  return CE_NONE;
}

static
T C3(arr, T, get_v)(C2(arr, T) *a, size_t idx) {
  circa_if_nullck(!a)
    return (circa_log("%s returned binary-zeroed value due to %s; %s", __func__, circa_err_name[CE_NULL], circa_err_explain[CE_NULL]), (T) {0});
  circa_if_oobck(idx >= a->len)
    return (circa_log("%s returned binary-zeroed value due to %s; %s", __func__, circa_err_name[CE_OOB], circa_err_explain[CE_OOB]), (T) {0});
  return a->data[idx];
}

static
circa_err C3(arr, T, get_r)(C2(arr, T) *a, size_t idx, T *v) {
  circa_nullck(a);
  circa_if_oobck(idx >= a->len)
    circa_throw(CE_OOB);
  circa_nullck(v);
  *v = a->data[idx];
  return CE_NONE;
}

static
circa_err C3(arr, T, snoc_v)(C2(arr, T) *a, T v) {
  circa_nullck(a);
  circa_err r = C3(arr, T, amort)(a);
  circa_if_oomck (r) return r;
  a->data[a->len++] = v;
  return CE_NONE;
}

static
circa_err C3(arr, T, snoc_r)(C2(arr, T) *a, T *v) {
  circa_nullck(a);
  circa_nullck(v);
  circa_err r = C3(arr, T, amort)(a);
  circa_if_oomck (r) return r;
  a->data[a->len++] = *v;
  return CE_NONE;
}

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
