/*
** seq.c | The Circa Library Set | Dynamic Sequences
** https://github.com/davidgarland/circa
*/

#include "../h/seq.h"

/*
** Accessors
*/

Seq seq_set_(size_t siz, Seq s, size_t a, void *v) {
  if (!siz || !s || !v)
    return s;
  s = seq_require_(siz, s, a + 1);
  if (seq(s)->len < a + 1)
    seq(s)->len = a + 1;
  memcpy(((char*) s) + a * siz, v, siz);
  return s;
}

bool seq_has_(size_t siz, Seq s, size_t a) {
  if (!siz || !s)
    return false;
  return (a < seq(s)->len);
}

void *seq_get_(size_t siz, Seq s, size_t a) {
  if (!siz | !s)
    return NULL;
  return (a < seq(s)->len) ? ((char*) s) + a * siz : NULL;
}

/*
** Allocators
*/

Seq seq_alloc_(size_t siz, size_t cap) {
  if (!siz || !cap)
    return NULL;
  struct seq_data *sd = malloc(sizeof(*sd) + cap * siz);
  if (!sd)
    return NULL;
  sd->cap = cap;
  sd->len = 0;
  return sd->data;
}

Seq seq_wrap_(size_t siz, size_t a, void *v) {
  if (!siz || !a || !v)
    return NULL;
  Seq s = seq_alloc_(siz, a);
  memcpy(s, v, a * siz);
  return s;
}

Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  if (!siz || !s || !cap)
    return NULL;
  struct seq_data *sd = seq(s);
  sd = realloc(sd, sizeof(*sd) + cap * siz);
  if (!sd)
    return NULL;
  sd->cap = cap;
  return sd->data;
}

Seq seq_require_(size_t siz, Seq s, size_t cap) {
  if (!siz || !s || !cap)
    return NULL;
  return (seq(s)->cap < cap) ? seq_realloc_(siz, s, cap) : s;
}

Seq seq_free_(Seq s) {
  if (s)
    free(seq(s));
  return NULL;
}

/*
** Stack Operations
*/

Seq seq_push_(size_t siz, Seq s, void *v) {
  if (!siz || !s || !v)
    return s;
  seq_set_(siz, s, seq(s)->len, v); // TODO remove call for speed
  return s;
}

void *seq_pop_(size_t siz, Seq s, size_t n) {
  if (!siz || !s || !seq(s)->len)
    return s;
  seq(s)->len -= n;
  return ((char*) s) + seq(s)->len * siz;
}
