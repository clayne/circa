   /////////////////////////////////////////////////////////////////////////////
  // seq.h | The Circa Core Library for C | Dynamic Sequences /////////////////
 // Author: David Garland (https://github.com/davidgarland/circa) ////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

#include "core/core.h"

  //////////////////////////////////////////////////////////////////////////////
 // Accessors /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
SeqFields seq(Seq s) {
  {
    circa_assert(s != NULL);
  }
  return (SeqFields) (((char*) s) - SeqSize);
}

  //////////////////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Seq seq_new_(size_t siz, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(cap > 0);
  }
  SeqFields s = NULL;
  while (s == NULL) {
    s = malloc(SeqSize + (siz * cap));
  }
  s->cap = cap;
  s->len = 0;
  return s->data;
}

_circa_
Seq seq_rsz_(size_t siz, Seq s, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  SeqFields s2 = NULL;
  while (s2 == NULL) {
    s2 = realloc(s, SeqSize + (siz * cap));
  }
  s2->cap = cap;
  return s2->data;
}

_circa_
Seq seq_rqr_(size_t siz, Seq s, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
  }
  if (cap > seq(s)->cap) {
    s = seq_rsz_(siz, s, cap);
  }
  return s;
}

_circa_
void seq_del_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
  }
  free(seq(s));
}

  //////////////////////////////////////////////////////////////////////////////
 // Array Ops /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Seq seq_set_(size_t siz, Seq s, size_t addr, void *val) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(val != NULL);
  }
  s = seq_rqr_(siz, s, addr + 1);
  seq(s)->len = (addr >= seq(s)->len) ? addr + 1 : seq(s)->len;
  memcpy(((char*) s) + (siz * addr), val, siz);
  return s;
}

_circa_
void *seq_get_(size_t siz, Seq s, size_t addr) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(addr < seq(s)->len);
  }
  return (void*) ((char*) s) + (siz * addr);
}

  //////////////////////////////////////////////////////////////////////////////
 // Stack Ops /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Seq seq_push_(size_t siz, Seq s, void *val) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(val != NULL);
  }
  return seq_set_(siz, s, seq(s)->len, val);
}

_circa_
void *seq_tos_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(seq(s)->len > 0);
  }
  return seq_get_(siz, s, seq(s)->len - 1);
}

_circa_
void *seq_pop_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(seq(s)->len > 0);
  }
  return seq_get_(siz, s, --seq(s)->len);
}

#endif // CIRCA_SEQ_H
