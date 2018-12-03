/*
** test.c | The Circa Library Set | Sanity Test
** https://github.com/davidgarland/circa
*/

#include <circa.h>

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wused-but-marked-unused"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wbad-function-cast"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-variable-declarations"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wstrict-prototypes"
#endif

#include <snow/snow.h>

Str st;

describe(Str) {
  subdesc(Allocators) {
    it("str_new") {
      st = str_new(5);
      assert(st != NULL, "The string should not be null after allocation.");
      assert(str(st)->cap == 5, "The string capacity should be 5.");
      assert(str(st)->len == 0, "The string length should be 0.");
    }
    
    it("str_rsz") {
      str_rsz(st, 10);
      assert(st != NULL, "The string should not be null after resizing.");
      assert(str(st)->cap == 10, "The string capacity should be 10.");
      assert(str(st)->len == 0, "The string length should be 0.");
      str_rsz(st, 5);
      assert(st != NULL, "The string should not be null after resizing.");
      assert(str(st)->cap == 5, "The string capacity should be 5.");
      assert(str(st)->len == 0, "The string length should be 0.");
    }

    it("str_rqr") {
      str_rqr(st, 3);
      assert(st != NULL, "The string should not be null after requirement.");
      assert(str(st)->cap == 5, "The string capacity should stay 5.");
      assert(str(st)->len == 0, "The string length should be 0.");
      str_rqr(st, 10);
      assert(st != NULL, "The string should not be null after requirement.");
      assert(str(st)->cap == 10, "The string capacity should be 10.");
      assert(str(st)->len == 0, "The string length should be 0.");
    }

    it("str_del") {
      str_del(st);
      assert(st == NULL, "The string should be null after deletion.");
      str_del(st);
      assert(st == NULL, "The string should be null after second deletion.");
    }
  }
}

Seq(int) sq;

describe(Seq) {
  subdesc(Allocators) {
    it("seq_new") {
      sq = seq_new(int, 5);
      assert(sq != NULL, "The sequence should not be null after allocation.");
      assert(seq(sq)->cap == 5, "The sequence capacity should be 5.");
      assert(seq(sq)->len == 0, "The sequence length should be 0.");
    }

    it("seq_rsz") {
      seq_rsz(sq, 10);
      assert(sq != NULL, "The sequence should not be null after resizing.");
      assert(seq(sq)->cap == 10, "The sequence capacity should be 10.");
      assert(seq(sq)->len == 0, "The sequence length should be 0.");
      seq_rsz(sq, 5);
      assert(sq != NULL, "The sequence should not be null after resizing.");
      assert(seq(sq)->cap == 5, "The sequence capacity should be 5.");
      assert(seq(sq)->len == 0, "The sequence length should be 0.");
    }

    it("seq_rqr") {
      seq_rqr(sq, 3);
      assert(sq != NULL, "The sequence should not be null after requirement.");
      assert(seq(sq)->cap == 5, "The sequence capacity should stay 5.");
      assert(seq(sq)->len == 0, "The sequence length should be 0.");
      seq_rqr(sq, 10);
      assert(sq != NULL, "The sequence should not be null after requirement.");
      assert(seq(sq)->cap == 10, "The sequence capacity should be 10.");
      assert(seq(sq)->len == 0, "The sequence length should be 0.");
    }

    it("seq_del") {
      seq_del(sq);
      assert(sq == NULL, "The sequence should be null after deletion.");
      seq_del(sq);
      assert(sq == NULL, "The sequence should be null after second deletion.");
    }
  }
}

/*
Dict(int) d;

describe(Dict) {
  subdesc(Allocators) {
    it("dict_new") {
      d = dict_new(int, 5);
      assert(d != NULL, "The dictionary should not be null after allocation.");
      assert(dict(d)->cap == usz_primegt(5), "The dictionary capacity should be usz_primegt(5).");
      assert(dict(d)->len == 0, "The dictionary length should be 0.");
    }

    it("dict_rsz") {
      dict_rsz(d, 10);
      assert(d != NULL, "The dictionary should not be null after resizing.");
      assert(dict(d)->cap == usz_primegt(10), "The dictionary capacity should be usz_primegt(10).");
      assert(dict(d)->len == 0, "The dictionary length should be 0.");
      dict_rsz(d, 5);
      assert(d != NULL, "The dictionary should not be null after resizing.");
      assert(dict(d)->cap == usz_primegt(5), "The dictionary capacity should be usz_primegt(5).");
      assert(dict(d)->len == 0, "The dictionary length should be 0.");
    }

    it("dict_rqr") {
      dict_rqr(d, 3);
      assert(d != NULL, "The dictionary should not be null after requirement.");
      assert(dict(d)->cap == usz_primegt(5), "The dictionary capacity should stay usz_primegt(5).");
      assert(dict(d)->len == 0, "The dictionary length should be 0.");
      dict_rqr(d, 15);
      assert(d != NULL, "The dictionary should not be null after requirement.");
      assert(dict(d)->cap == usz_primegt(15), "The dictionary capacity should be usz_primegt(15).");
      assert(dict(d)->len == 0, "The dictionary length should be 0.");
    }

    it("dict_del") {
      dict_del(d);
      assert(d == NULL, "The dictionary should be null after deletion.");
      dict_del(d);
      assert(d == NULL, "The dictionary should be null after second deletion.");
    }
  }

  subdesc(Accessors) {
    it("dict_set") {
      d = dict_new(int, 1);
      dict_set_lit(d, "oranges", 5);
      assert(dict(d)->len == 1);
      dict_set_lit(d, "apples", 2);
      assert(dict(d)->len == 2);
      dict_set_lit(d, "peaches", 8);
      assert(dict(d)->len == 3);
      dict_set_lit(d, "mangos", 10);
      assert(dict(d)->len == 4);
      dict_set_lit(d, "bananas", 12);
      assert(dict(d)->len == 5);
    }

    it("dict_get") {
      assert(dict_get(d, "oranges") == 5);
      assert(dict_get(d, "apples") == 2);
      assert(dict_get(d, "peaches") == 8);
      assert(dict_get(d, "mangos") == 10);
      assert(dict_get(d, "bananas") == 12);
    }
  }

  subdesc(Cleanup) {
    it("dict_del") {
      dict_del(d);
    }
  }
}
*/

snow_main();
