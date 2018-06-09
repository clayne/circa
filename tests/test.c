   //                                                                ///////////
  // test.c | Circa's test suite, using the Snow testing framework. ///////////
 // https://github.com/davidgarland/circa                          ///////////
//                                                                ///////////

#include "../lib/snow/snow/snow.h"
#include "../circa.h"

Arena(int) ar = NULL;
Seq(int) sq   = NULL;
Str st        = NULL;

describe(Arena, {
  subdesc(Allocators, {
    it("arena_new", {
      ar = arena_new(int, 5);
      assert(ar != NULL);
      assert(arena(ar)->cap == 5);
    });

    it("arena_rsz", {
      arena_rsz(int, ar, 10);
      assert(ar != NULL);
      assert(arena(ar)->cap == 10);
    });

    it("arena_del", {
      arena_del(int, ar);
    });
  });
});

describe(Seq,
  subdesc(Allocators, {
    it("seq_new", {
      sq = seq_new(int, 5);
      assert(sq != NULL);
      assert(seq(sq)->cap == 5);
    });

    it("seq_rsz", {
      seq_rsz(int, sq, 10);
      assert(sq != NULL);
      assert(seq(sq)->cap == 10);
    });

    it("seq_rqr", {
      seq_rqr(int, sq, 5);
      assert(sq != NULL);
      assert(seq(sq)->cap == 10);
      seq_rqr(int, sq, 15);
      assert(sq != NULL);
      assert(seq(sq)->cap == 15);
    });

    it("seq_del", {
      seq_del(int, sq);
      seq_del(int, sq);
      assert(sq == NULL);
    });
  });
);

describe(Str,
  subdesc(Allocators, {
    it("str_new", {
      st = str_new(5);
      assert(st != NULL);
      assert(str(st)->cap == 5);
    });

    it("str_rsz", {
      str_rsz(st, 10);
      assert(st != NULL);
      assert(str(st)->cap == 10);
    });

    it("str_rqr", {
      str_rqr(st, 5);
      assert(st != NULL);
      assert(str(st)->cap == 10);
      str_rqr(st, 15);
      assert(st != NULL);
      assert(str(st)->cap == 15);
    });

    it("str_del", {
      str_del(st);
      str_del(st);
      assert(st == NULL);
    });
  });
  subdesc(IO, {
    it("str_readfile", {
      str_del(st);
      st = str_new(1);
      str_readfile(st, "tests/assets/test.txt");
      assert(st != NULL);
      assert(str(st)->cap > 0);
      assert(str(st)->len > 0);
      str_del(st);
    });
  });
);

snow_main();
