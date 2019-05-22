/*
** test.c | The Circa Library Set | Snow unit testing.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include <circa.h>

/* Vendored */

#include <snow/snow.h>

Seq(int) xs;

describe (Seq) {
  subdesc (Allocators) {
    it ("seq_alloc") {
      xs = seq_alloc(int, 10);
      assert(!CE, "No error should be thrown after allocation.");
      assert(xs, "Sequence shouldn't be NULL after allocation.");
      assert(seq(xs)->cap == 10, "Sequence capacity should be set after alloc.");
    }

    it ("seq_realloc") {
      seq_realloc(xs, 5);
      assert(!CE, "No error should be thrown after reallocation.");
      assert(xs, "Sequence shouldn't be NULL after reallocation.");
      assert(seq(xs)->cap == 5, "Sequence capacity should be set after realloc.");
    }

    it ("seq_require") {
      seq_require(xs, 3);
      assert(!CE, "No error should be thrown after requirement.");
      assert(xs, "Sequence shouldn't be NULL after requirement.");
      assert(seq(xs)->cap == 5, "Sequence capacity shouldn't change after smaller requirement.");
      seq_require(xs, 6);
      assert(!CE, "No error should be thrown after requirement.");
      assert(xs, "Sequence shouldn't be NULL after requirement.");
      assert(seq(xs)->cap > 5, "Sequence capacity should increase after larger requirement.");
    }

    it ("seq_free") {
      seq_free(xs);
      assert(!CE, "No error should be thrown after freeing.");
      assert(!xs, "Sequence should be NULL after freeing.");
      seq_free(xs);
      assert(!CE, "No error should be thrown after double freeing.");
      assert(!xs, "Sequence should remain NULL after double freeing.");
    }
  }
}

snow_main();
