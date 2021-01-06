#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
  #include "circa_map.h"
#endif

circa_static
circa_err C4(map, K, V, alloc)(C3(map, K, V) *const restrict m) {
  m->keys = malloc(2 * sizeof(K));
  circa_oomck(m->keys);
  m->vals = malloc(2 * sizeof(V));
  circa_if_oomck(!m->vals) {
    free(m->keys);
    circa_throw(CE_OOM);
  }
  m->psls = calloc(2, sizeof(size_t));
  circa_if_oomck(!m->psls) {
    free(m->keys);
    free(m->vals);
    circa_throw(CE_OOM);
  }
  m->cap = 2;
  m->len = 0;
  return CE_NONE;
}

circa_static
circa_err C4(map, K, V, free)(C3(map, K, V) *m) {
  circa_nullck(m);
  free(m->keys);
  free(m->vals);
  free(m->psls);
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
