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
      assert(seq(xs)->cap >= 6, "Sequence capacity should increase after larger requirement.");
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

Txt t;

describe (Txt) {
  subdesc (Allocators) {
    it ("txt_alloc") {
      t = txt_alloc(10);
      assert(!CE, "No error should be thrown after alocation.");
      assert(t, "Text shouldn't be NULL after allocation.");
      assert(txt(t)->cap == 10, "Text capacity should be set after alloc.");
    }

    it ("txt_realloc") {
      txt_realloc(t, 5);
      assert(!CE, "No error should be thrown after reallocation.");
      assert(t, "Text shouldn't be NULL after reallocation.");
      assert(txt(t)->cap == 5, "Text capacity should be set after realloc.");
    }

    it ("txt_require") {
      txt_require(t, 3);
      assert(!CE, "No error should be thrown after requirement.");
      assert(t, "Text shouldn't be NULL after requirement.");
      assert(txt(t)->cap == 5, "Text capacity shouldn't change after smaller requirement.");
      txt_require(t, 6);
      assert(!CE, "No error should be thrown after requirement.");
      assert(t, "Text shouldn't be NULL after requirement.");
      assert(txt(t)->cap >= 6, "Text capacity should increase after larger requirement.");
    }

    it ("txt_free") {
      txt_free(t);
      assert(!CE, "No error should be thrown after freeing.");
      assert(!t, "Text should be NULL after freeing.");
      txt_free(t);
      assert(!CE, "No error should be thrown after double freeing.");
      assert(!t, "Text should be NULL after double freeing.");
    }
  }
}

snow_main();
