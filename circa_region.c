#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_region.h"
  #define circa_static
#endif

static const size_t C3(region, T, siz) = 1 << C;

circa_static
circa_err C3(region, T, alloc)(C2(region, T) *const restrict r) {
  circa_nullck(r);
  C3(region, T, node) *node = malloc(sizeof(*node) + C3(region, T, siz) * sizeof(T));
  circa_oomck(node);
  node->len = 0;
  node->next = NULL;
  r->head = node;
  return CE_NONE;
}

circa_static
circa_err C3(region, T, free)(C2(region, T) *const restrict r) {
  circa_nullck(r);
  C3(region, T, node) *node = r->head;
  while (node != NULL) {
    C3(region, T, node) *next = node->next;
    free(node);
    node = next;
  }
  return CE_NONE;
}

circa_static
T *C3(region, T, new)(C2(region, T) *const restrict r) {
  circa_if_nullck (!r) return NULL;
  C3(region, T, node) *node = r->head;
  if_unlikely (node->len == C3(region, T, siz)) {
    C3(region, T, node) *new_node = malloc(sizeof(*new_node) + C3(region, T, siz) * sizeof(T));
    circa_if_oomck (!new_node) return NULL;
    new_node->len = 0;
    new_node->next = node;
    r->head = node = new_node;
  }
  return node->data + node->len++;
}

circa_static
T *C3(region, T, new_v)(C2(region, T) *const restrict r, register const T v) {
  circa_if_nullck (!r) return NULL;
  T *const restrict x = C3(region, T, new)(r);
  *x = v;
  return x;
}

circa_static
T *C3(region, T, new_r)(C2(region, T) *const restrict r, const T *const restrict v) {
  circa_if_nullck (!r) return NULL;
  circa_if_nullck (!v) return NULL;
  T *const restrict x = C3(region, T, new)(r);
  *x = *v;
  return x;
}

#undef circa_static

/*
** Copyright 2021 David Garland
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
