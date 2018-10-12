/*
** tests/test.c | Unit testing suite for Circa.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Internal */

#include "../core.h"
#include "../types.h"
#include "../bits.h"
#include "../seq.h"
#include "../str.h"
#include "../dict.h"

/* Vendored */

#include <snow/snow.h>

/*
** Tests
*/

Seq(int) sq;

describe(Seq) {
  subdesc(Allocators) {
    it("seq_new") {
      sq = seq_new(int, 5);
      assert(sq != NULL, "The sequence shouldn't be NULL after allocation.");
      assert(seq(sq)->cap == 5, "The sequence capacity should be equal to 5.");
      assert(seq(sq)->len == 0, "The sequence length should be zero.");
    }

    it("seq_rsz") {
      seq_rsz(sq, 10);
      assert(sq != NULL, "The sequence shouldn't be NULL after resizing.");
      assert(seq(sq)->cap == 10, "The sequence capacity should be equal to 10.");
      assert(seq(sq)->len == 0, "The sequence length should be zero.");
    }

    it("seq_rqr") {
      seq_rqr(sq, 5);
      assert(sq != NULL, "The sequence shouldn't be NULL after resizing.");
      assert(seq(sq)->cap == 10, "The sequence capacity should be equal to 10.");
      assert(seq(sq)->len == 0, "The sequence length should be zero.");
      seq_rqr(sq, 15);
      assert(sq != NULL, "The sequence shouldn't be NULL after resizing.");
      assert(seq(sq)->cap == 15, "The sequence capacity should be equal to 15.");
      assert(seq(sq)->len == 0, "The sequence length should be zero.");
    }

    it("seq_del") {
      seq_del(sq);
      assert(sq == NULL);
      seq_del(sq);
      assert(sq == NULL);
    }
  }
}

Str st;

describe(Str) {
  subdesc(Allocators) {
    it("str_new") {
      st = str_new(5);
      assert(st != NULL, "The string shouldn't be NULL after allocation.");
      assert(str(st)->cap == 5, "The string capacity should be equal to 5.");
      assert(str(st)->len == 0, "The string length should be zero.");
    }

    it("str_rsz") {
      str_rsz(st, 10);
      assert(st != NULL, "The string shouldn't be NULL after resizing.");
      assert(str(st)->cap == 10, "The string capacity should be equal to 10.");
      assert(str(st)->len == 0, "The string length should be zero.");
    }

    it("str_rqr") {
      str_rqr(st, 5);
      assert(st != NULL, "The string shouldn't be NULL after resizing.");
      assert(str(st)->cap == 10, "The string capacity should be equal to 10.");
      assert(str(st)->len == 0, "The string length should be zero.");
      str_rqr(st, 15);
      assert(st != NULL, "The string shouldn't be NULL after resizing.");
      assert(str(st)->cap == 15, "The string capacity should be equal to 15.");
      assert(str(st)->len == 0, "The string length should be zero.");
    }

    it("str_del") {
      str_del(st);
      assert(st == NULL, "The string should be NULL after the first deletion.");
      str_del(st);
      assert(st == NULL, "The string should be NULL after the second deletion.");
    }
  }
}

Dict(int) d;

describe(Dict) {
  subdesc(Allocators) {
    it("dict_new") {
      d = dict_new(int, 5);
      assert(d != NULL, "The dictionary shouldn't be NULL after allocation.");
      assert(dict(d)->cap == u32_primegt(5), "The dictionary capacity should be equal to u32_primegt(5).");
      assert(dict(d)->len == 0, "The dictionary length should be zero.");
    }
    it("dict_del") {
      dict_del(d);
      assert(d == NULL, "The dictionary should be NULL after the first deletion.");
      dict_del(d);
      assert(d == NULL, "The dictionary should be NULL after the second deletion.");
    }
  }
}

snow_main();
