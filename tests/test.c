/*
** test.c | The Circa Library Set | Main Test Suite
** https://github.com/davidgarland/circa
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

    it ("seq_require") {
      seq_require(s, 5);
      assert(s, "Sequence shouldn't be NULL after requirement.");
      assert(seq(s)->cap == 10, "Sequence capacity shouldn't change after smaller requirement.");

      seq_require(s, 15);
      assert(s, "Sequence shouldn't be NULL after requirement.");
      assert(seq(s)->cap == 15, "Sequence capacity should change after being required to a larger amount.");
    }

    it ("seq_free") {
      seq_free(s);
      assert(!s, "Sequence should be NULL after free.");
      seq_free(s);
      assert(!s, "Sequence should remain NULL after double free.");
      //s = seq_alloc(int, 1);
      //assert(s, "Sequence shouldn't be NULL after allocating it again.");
    }
  }
}

Txt t;

describe (Txt) {
  subdesc (Allocators) {
    it ("txt_alloc") {
      t = txt_alloc(5);
      assert(t, "Text shouldn't be NULL after allocation.");
      assert(txt(t)->cap == 5, "Text capacity should be set after alloc.");
    }

    it ("txt_realloc") {
      txt_realloc(t, 10);
      assert(t, "Text shouldn't be NULL after reallocation.");
      assert(txt(t)->cap == 10, "Text capacity should be changed after realloc.");
    }

    it ("txt_require") {
      txt_require(t, 5);
      assert(t, "Text shouldn't be NULL after requirement.");
      assert(txt(t)->cap == 10, "Text capacity shouldn't be changed after smaller requirement.");
      
      txt_require(t, 15);
      assert(t, "Text shouldn't be NULL after requirement.");
      assert(txt(t)->cap == 15, "Text capacity should change after being required to a larger amount.");
    }

    it ("txt_free") {
      txt_free(t);
      assert(!t, "Text should be NULL after free.");
      txt_free(t);
      assert(!t, "Text should remain NULL after double free.");
      //t = txt_alloc(1);
      //assert(t, "Text shouldn't be NULL after allocating it again.");
    }
  }
}

Map(int, int) m;

describe (Map) {
  subdesc (Allocators) {
    it ("map_alloc") {
      m = map_alloc(int, int, 5);
      assert(m, "Map shouldn't be NULL after allocation.");
      assert(map(m)->cap == usz_primegt(5), "Map capacity should be set after alloc.");
    }

    it ("map_realloc") {
      map_realloc(m, 15);
      assert(m, "Map shouldn't be NULL after reallocation.");
      assert(map(m)->cap == usz_primegt(15), "Map capacity should be set after realloc.");
    }

    it ("map_require") {
      map_require(m, 5);
      assert(m, "Map shouldn't be NULL after requirement.");
      assert(map(m)->cap == usz_primegt(15), "Map capacity shouldn't change after lesser requirement.");
      map_require(m, 30);
      assert(m, "Map shouldn't be NULL after requirement.");
      assert(map(m)->cap == usz_primegt(30), "Map capacity should change after being required.");
    }

    it ("map_free") {
      map_free(m);
      assert(!m, "Map should be NULL after free.");
      map_free(m);
      assert(!m, "Map should remain NULL after double-free.");
    }
  }
}

SeqMap(int, int) sm;

describe (SeqMap) {
  subdesc (Allocators) {
    it ("seqmap_alloc") {
      sm = seqmap_alloc(int, int, 5);
      assert(sm, "SeqMap shouldn't be NULL after allocation.");
      assert(seqmap(sm)->cap == usz_primegt(5), "SeqMap capacity should be set after alloc.");
    }

    it ("seqmap_realloc") {
      seqmap_realloc(sm, 15);
      assert(sm, "SeqMap shouldn't be NULL after reallocation.");
      assert(seqmap(sm)->cap == usz_primegt(15), "SeqMap capacity should be set after requirement.");
    }

    it ("seqmap_require") {
      seqmap_require(sm, 5);
      assert(sm, "SeqMap shouldn't be NULL after requirement.");
      assert(seqmap(sm)->cap == usz_primegt(15), "SeqMap capacity shouldn't change after lesser requirement.");
      seqmap_require(sm, 30);
      assert(sm, "SeqMap shouldn't be NULL after requirement.");
      assert(seqmap(sm)->cap == usz_primegt(30), "SeqMap capacity should change after requirement.");
    }

    it ("seqmap_free") {
      seqmap_free(sm);
      assert(!sm, "SeqMap should be NULL after free.");
      seqmap_free(sm);
      assert(!sm, "SeqMap should be NULL after double-free.");
    }
  }
}

Dict(int) d;

describe (Dict) {
  subdesc (Allocators) {
    it ("dict_alloc") {
      d = dict_alloc(int, 5);
      assert(d, "Dict shouldn't be NULL after allocation.");
      assert(dict(d)->cap == usz_primegt(5), "Dict capacity should be set after alloc.");
    }

    it ("dict_realloc") {
      dict_realloc(d, 15);
      assert(d, "Dict shouldn't be NULL after reallocation.");
      assert(dict(d)->cap == usz_primegt(15), "Dict capacity should be set after reallocation.");
    }

    it ("dict_require") {
      dict_require(d, 5);
      assert(d, "Dict shouldn't be NULL after requirement.");
      assert(dict(d)->cap == usz_primegt(15), "Dict capacity shouldn't change after lesser requirement.");
      dict_require(d, 30);
      assert(d, "Dict shoudln't be NULL after requirement.");
      assert(dict(d)->cap == usz_primegt(30), "Dict capacity should change after requirement.");
    }

    it ("dict_free") {
      dict_free(d);
      assert(!d, "Dict should be NULL after free.");
      dict_free(d);
      assert(!d, "Dict should remain NULL after double-free.");
    }
  }
}

Set(int) e;

describe (Set) {
  subdesc (Allocators) {
    it ("set_alloc") {
      e = set_alloc(int, 5);
      assert(e, "Set shouldn't be NULL after allocation.");
      assert(set(e)->cap == usz_primegt(5), "Set capacity should be set after alloc.");
    }

    it ("set_realloc") {
      set_realloc(e, 15);
      assert(e, "Set shouldn't be NULL after reallocation.");
      assert(set(e)->cap == usz_primegt(15), "Set capacity should be set after reallocation.");
    }

    it ("set_require") {
      set_require(e, 5);
      assert(e, "Set shouldn't be NULL after requirement.");
      assert(set(e)->cap == usz_primegt(15), "Set capacity shouldn't change after lesser requirement.");
      set_require(e, 30);
      assert(e, "Set shouldn't be NULL after requirement.");
      assert(set(e)->cap == usz_primegt(30), "Set capacity should change after requirement.");
    }

    it ("set_free") {
      set_free(e);
      assert(!e, "Set should be NULL after free.");
      set_free(e);
      assert(!e, "Set should remain NULL after double-free.");
    }
  }
}

// TODO
// SeqSet(int) se;

snow_main();
