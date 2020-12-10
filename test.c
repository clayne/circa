/*
** Description
**   A little test file to check that all the stuff in Circa compiles and runs
**   sanely.
*/

#include <threads.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "macro/cat.h"
#include "macro/gnu.h"

#include "misc/error.h"
#include "misc/threadpool.h"

#include "math/imath.h"
#include "math/simd.h"

#include "data/str.h"

#define T int
#define C 3
  #include "data/vec.h"
  #include "data/hatc.h"
  #include "data/hatd.h"
#undef C
#undef T

static
void test_imath() {
  size_t bits = 8 * sizeof(size_t);
  for (size_t i = 0; i < bits; i++) {
    assert(size_t_ctz(1ULL << i) == i);
    assert(size_t_clz(1ULL << i) == bits - i - 1);
    assert(size_t_bsr(1ULL << i) == i);
    assert(size_t_pop(1ULL << i) == 1);
  }
  size_t x = 1;
  for (size_t i = 0; i < bits; i++) {
    assert(size_t_ctz(x) == 0);
    assert(size_t_clz(x) == bits - i - 1);
    assert(size_t_bsr(x) == i);
    assert(size_t_pop(x) == i + 1);
    x |= (x << 1);
  }
}

static
void test_vec() {
  vec_int v;
  vec_int_alloc(&v);
  for (int i = 0; i < 100; i++)
    vec_int_snoc_v(&v, i);
  assert(v.len == 100);
  for (int i = 0; i < 100; i++)
    assert(vec_int_get_v(&v, i) == i);
  vec_int_free(&v);
}

static
void test_hatc() {
  hatc_int h;
  hatc_int_alloc(&h);
  for (int i = 0; i < 100; i++)
    hatc_int_snoc_v(&h, i);
  assert(h.len == 100);
  for (int i = 0; i < 100; i++)
    assert(hatc_int_get_v(&h, i) == i);
  hatc_int_free(&h);
}

static
void test_hatd() {
  hatd_int h;
  hatd_int_alloc(&h);
  for (int i = 0; i < 100; i++)
    hatd_int_snoc_v(&h, i);
  assert(h.len == 100);
  for (int i = 0; i < 100; i++)
    assert(hatd_int_get_v(&h, i) == i);
  hatd_int_free(&h);
}

int test_threadpool_fn(void *arg) {
  *(size_t*) arg = thread_info.id.idx;
  return 0;
}

static
void test_threadpool() {
  size_t num_threads = 8;
  threadpool_alloc(num_threads + 1);
  thread_id threads[num_threads];
  size_t results[num_threads];
  for (size_t i = 0; i < num_threads; i++)
    threadpool_spawn(threads + i, test_threadpool_fn, results + i);
  for (size_t i = 0; i < num_threads; i++)
    threadpool_join(threads[i], NULL);
  for (size_t i = 0; i < num_threads; i++)
    printf("%zu\n", results[i]);
  threadpool_free();
}

int main() {
  test_imath();
  test_vec();
  test_hatc();
  test_hatd();
  test_threadpool();
  return 0;
}
