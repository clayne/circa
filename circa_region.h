#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef struct C3(region, T, node) {
  struct C3(region, T, node) *next;
  size_t len;
  T data[];
} C3(region, T, node);

typedef struct C2(region, T) {
  C3(region, T, node) *head;
} C2(region, T);

circa_static circa_err C3(region, T, alloc)(C2(region, T) *const restrict);
circa_static circa_err C3(region, T, free)(C2(region, T) *const restrict);

circa_static T *C3(region, T, new)(C2(region, T) *const restrict);
circa_static T *C3(region, T, new_v)(C2(region, T) *const restrict, register const T);
circa_static T *C3(region, T, new_r)(C2(region, T) *const restrict, const T *const restrict);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_region.c"
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
