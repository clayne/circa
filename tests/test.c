/*
** test.c | The Circa Library Set | Main Test Suite
**
*/

#include <circa.h>
#include <snow/snow.h>

Seq(int) s;

describe (Seq) {
  subdesc (Allocators) {
    it ("seq_alloc") {
      s = seq_alloc(int, 5);
      assert(s, "Sequence shouldn't be NULL after allocation.");
      assert(seq(s)->cap == 5, "Sequence capacity should be set after alloc.");
    }
    it ("seq_realloc") {
      seq_realloc(s, 10);
      assert(s, "Sequence shouldn't be NULL after reallocation.");
      assert(seq(s)->cap == 10, "Sequence capacity should be changed after realloc.");
    }
  }
}

snow_main();
