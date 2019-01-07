/*
** seq.c | The Circa Library Set | Dynamic Sequences
** https://github.com/davidgarland/circa
*/

#include "../h/seq.h"

/*
** Accessors
*/

void seq_clear_(size_t siz, Seq s) {
  if (!siz || !s) {
    CE = CE_ARG;
    return;
  }
  memset(s, 0, siz * seq(s)->len);
  seq(s)->len = 0;
}

Seq seq_set_(size_t siz, Seq s, size_t a, void *v) {
  if (!siz || !s || !v)
    return (CE = CE_ARG, s);
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
    return (CE = CE_ARG, NULL);
  if (a >= seq(s)->len)
    return (CE = CE_OOB, NULL);
  return ((char*) s) + a * siz;
}

/*
** Allocators
*/

Seq seq_alloc_(size_t siz, size_t cap) {
  if (!siz || !cap)
    return (CE = CE_ARG, NULL);
  struct seq_data *sd = malloc(sizeof(*sd) + cap * siz);
  if (!sd)
    return (CE = CE_OOM, NULL);
  sd->cap = cap;
  sd->len = 0;
  return sd->data;
}

Seq seq_wrap_(size_t siz, size_t a, void *v) {
  if (!siz || !a || !v)
    return (CE = CE_ARG, NULL);
  Seq s = seq_alloc_(siz, a);
  memcpy(s, v, a * siz);
  return s;
}

Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  if (!siz || !s || !cap)
    return (CE = CE_ARG, s);
  struct seq_data *sd = seq(s);
  sd = realloc(sd, sizeof(*sd) + cap * siz);
  if (!sd)
    return (CE = CE_OOM, s);
  sd->cap = cap;
  return sd->data;
}

Seq seq_require_(size_t siz, Seq s, size_t cap) {
  if (!siz || !s || !cap)
    return (CE = CE_ARG, s);
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
    return (CE = CE_ARG, s);
  s = seq_set_(siz, s, seq(s)->len, v); // TODO remove call for speed
  return s;
}

void *seq_pop_(size_t siz, Seq s, size_t n) {
  if (!siz || !s || !seq(s)->len)
    return (CE = CE_ARG, s);
  seq(s)->len -= n;
  return ((char*) s) + (seq(s)->len - (n ? 0 : 1)) * siz;
}
