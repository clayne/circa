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
} C2(hatc, T);

circa_static circa_err C3(hatc, T, alloc)(C2(hatc, T) *);
circa_static circa_err C3(hatc, T, free)(C2(hatc, T) *);

circa_static T *C3(hatc, T, lookup)(C2(hatc, T) *, size_t);
circa_static circa_err C3(hatc, T, set_v)(C2(hatc, T) *, size_t, T);
circa_static circa_err C3(hatc, T, set_r)(C2(hatc, T) *, size_t, T *);
circa_static T C3(hatc, T, get_v)(C2(hatc, T) *, size_t);
circa_static circa_err C3(hatc, T, get_r)(C2(hatc, T) *, size_t, T *);

circa_static circa_err C3(hatc, T, snoc_v)(C2(hatc, T) *, T);
circa_static circa_err C3(hatc, T, snoc_r)(C2(hatc, T) *, T *);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "hatc.c"
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
