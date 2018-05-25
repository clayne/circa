   /////////////////////////////////////////////////////////////////////////////
  // str.h | The Circa Core Library for C | Dynamic Strings ///////////////////
 // Author: David Garland (https://github.com/davidgarland/circa) ////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef CIRCA_STR_H
#define CIRCA_STR_H

#include "core/core.h"

  //////////////////////////////////////////////////////////////////////////////
 // Accessors /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
StrFields str(Str s) {
  {
    circa_assert(s != NULL);
  }
  return (StrFields) (s - StrSize);
}

  //////////////////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Str str_new_(size_t cap) {
  {
    circa_assert(cap > 0);
  }
  StrFields s = NULL;
  while (s == NULL) {
    s = (StrFields) malloc(StrSize + cap);
  }
  s->cap = cap;
  s->len = 0;
  return s->data;
}

_circa_
Str str_rsz_(Str s, size_t cap) {
  {
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  StrFields s2 = NULL;
  while (s2 == NULL) {
    s2 = (StrFields) realloc(str(s), StrSize + cap);
  }
  s2->cap = cap;
  return s2->data;
}

_circa_
Str str_rqr_(Str s, size_t cap) {
  {
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  if (cap > str(s)->cap) {
    s = str_rsz_(s, cap);
  }
  return s;
}

_circa_
Str str_shr_(Str s) {
  {
    circa_assert(s != NULL);
  }
  const StrFields sf = str(s);
  const size_t len = sf->len;
  if (sf->cap > len) {
    s = str_rsz_(s, len);
  }
  return s;
}

_circa_
void str_del(Str s) {
  free(str(s));
}

  //////////////////////////////////////////////////////////////////////////////
 // Array Ops /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Str str_set_(Str s, size_t addr, char c) {
  {
    circa_assert(s != NULL);
  }
  str_rqr_(s, addr + 1);
  s[addr] = c;
  return s;
}

_circa_
char str_get_(Str s, size_t addr) {
  {
    circa_assert(s != NULL);
    circa_assert(addr < seq(s)->len);
  }
  return s[addr];
}

  //////////////////////////////////////////////////////////////////////////////
 // Stack Ops /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

_circa_
Str str_push_(Str s, char c) {
  {
    circa_assert(s != NULL);
  }
  return str_set_(s, seq(s)->len, c);
}

_circa_
char str_pop_(Str s) {
  {
    circa_assert(s != NULL);
  }
  return str_get_(s, --seq(s)->len);
}

#endif // CIRCA_STR_H
