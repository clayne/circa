/*
** tests/test.c | Unit testing suite for Circa.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Vendored */

#include "../lib/snow/snow/snow.h"

/* Internal */

#include "../core.h"
#include "../types.h"
#include "../bits.h"
#include "../seq.h"
#include "../str.h"
#include "../dict.h"

/*
** Tests
*/

Seq(int) sq;

describe(Seq,
  subdesc(Allocators, {
    it("seq_new", {
      sq = seq_new(int, 5);
      assert(sq != NULL);
      assert(seq(sq)->cap == 5);
      assert(seq(sq)->len == 0);
    });

    it("seq_rsz", {
      seq_rsz(sq, 10);
      assert(sq != NULL);
      assert(seq(sq)->cap == 10);
      assert(seq(sq)->len == 0);
    });

    it("seq_rqr", {
      seq_rqr(sq, 5);
      assert(sq != NULL);
      assert(seq(sq)->cap == 10);
      assert(seq(sq)->len == 0);
      seq_rqr(sq, 15);
      assert(sq != NULL);
      assert(seq(sq)->cap == 15);
      assert(seq(sq)->len == 0);
    });

    it("seq_del", {
      seq_del(sq);
      assert(sq == NULL);
      seq_del(sq);
      assert(sq == NULL);
    });
  });
);

Str st;

describe(Str,
  subdesc(Allocators, {
    it("str_new", {
      st = str_new(5);
      assert(st != NULL);
      assert(str(st)->cap == 5);
      assert(str(st)->len == 0);
    });

    it("str_rsz", {
      str_rsz(st, 10);
      assert(st != NULL);
      assert(str(st)->cap == 10);
      assert(str(st)->len == 0);
    });

    it("str_rqr", {
      str_rqr(st, 5);
      assert(st != NULL);
      assert(str(st)->cap == 10);
      assert(str(st)->len == 0);
      str_rqr(st, 15);
      assert(st != NULL);
      assert(str(st)->cap == 15);
      assert(str(st)->len == 0);
    });

    it("str_del", {
      str_del(st);
      assert(st == NULL);
      str_del(st);
      assert(st == NULL);
    });
  });
);

Dict(int) d;

describe(Dict,
  subdesc(Allocators, {
    it("dict_new", {
      d = dict_new(int, 5);
      assert(d != NULL);
      assert(dict(d)->cap == u32_primegt(5));
      assert(dict(d)->len == 0);
    });
    it("dict_del", {
      dict_del(d);
      assert(d == NULL);
      dict_del(d);
      assert(d == NULL); 
    });  
  });
);

snow_main();
