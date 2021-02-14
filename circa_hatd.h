#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

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

circa_static circa_err C3(hatd, T, alloc)(C2(hatd, T) *const restrict);
circa_static circa_err C3(hatd, T, free)(C2(hatd, T) *const restrict);

circa_static T *C3(hatd, T, lookup)(C2(hatd, T) *const restrict, register const size_t);
circa_static circa_err C3(hatd, T, set_v)(C2(hatd, T) *const restrict, register const size_t, register const T);
circa_static circa_err C3(hatd, T, set_r)(C2(hatd, T) *const restrict, register const size_t, T *const restrict);
circa_static T C3(hatd, T, get_v)(C2(hatd, T) *const restrict, register const size_t);
circa_static circa_err C3(hatd, T, get_r)(C2(hatd, T) *const restrict, register const size_t, T *const restrict);

circa_static circa_err C3(hatd, T, snoc_v)(C2(hatd, T) *const restrict, register const T);
circa_static circa_err C3(hatd, T, snoc_r)(C2(hatd, T) *const restrict, T *const restrict);

circa_static T *C3(hatd, T, reserve_r)(C2(hatd, T) *const restrict);
circa_static size_t C3(hatd, T, reserve_i)(C2(hatd, T) *const restrict);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_hatd.c"
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
