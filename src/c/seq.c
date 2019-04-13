/*
** seq.c | The Circa Library Set | Dynamic Sequences
** https://github.com/davidgarland/circa
*/

#include "slice.h"
#include "seq.h"

/*
** Accessors
*/

void seq_clear_(size_t siz, Seq s) {
  ce_guard (!siz || !s) {
    CE = CE_ARG;
    return;
  }
  memset(s, 0, siz * seq(s)->len);
  seq(s)->len = 0;
}

Seq seq_set_(size_t siz, Seq s, size_t a, void *v) {
  ce_guard (!siz || !s || !v)
    return (CE = CE_ARG, s);
  s = seq_require_(siz, s, a + 1);
  if (seq(s)->len < a + 1)
    seq(s)->len = a + 1;
  memcpy(((char*) s) + a * siz, v, siz);
  return s;
}

bool seq_has_(size_t siz, Seq s, size_t a) {
  ce_guard (!siz || !s)
    return false;
  return (a < seq(s)->len);
}

void *seq_get_(size_t siz, Seq s, size_t a) {
  ce_guard (!siz | !s)
    return (CE = CE_ARG, NULL);
  if (a >= seq(s)->len)
    return (CE = CE_OOB, NULL);
  return ((char*) s) + a * siz;
}

/*
** Allocators
*/

Seq seq_alloc_(size_t siz, size_t cap) {
  ce_guard (!siz || !cap)
    return (CE = CE_ARG, NULL);
  struct seq_data *sd = malloc(sizeof(*sd) + (cap + 1) * siz);
  if (!sd)
    return (CE = CE_OOM, NULL);
  sd->cap = cap;
  sd->len = 0;
  return sd->data;
}

Seq seq_wrap_(size_t siz, size_t a, void *v) {
  ce_guard (!siz || !a || !v)
    return (CE = CE_ARG, NULL);
  Seq s = seq_alloc_(siz, a);
  if (CE)
    return NULL;
  memcpy(s, v, a * siz);
  seq(s)->len = a;
  return s;
}

Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  ce_guard (!siz || !s || !cap)
    return (CE = CE_ARG, s);
  struct seq_data *sd = seq(s);
  sd = realloc(sd, sizeof(*sd) + (cap + 1) * siz);
  if (!s)
    return (CE = CE_OOM, s);
  sd->cap = cap;
  return sd->data;
}

Seq seq_require_(size_t siz, Seq s, size_t cap) {
  ce_guard (!siz || !s || !cap)
    return (CE = CE_ARG, s);
  return (seq(s)->cap < cap) ? seq_realloc_(siz, s, cap) : s;
}

Seq seq_free_(Seq s) {
  if (s)
    free(seq(s));
  return NULL;
}

/*
** Sequence Operations
*/

Seq seq_cpy_(size_t siz, Seq a, Seq b) {
  ce_guard (!siz || !a || !b)
    return (CE = CE_ARG, a);
  const size_t b_len = seq(b)->len;
  a = seq_require_(siz, a, b_len);
  if (CE)
    return a;
  seq(a)->len = b_len;
  memcpy(a, b, b_len * siz);
  return a;
}

Seq seq_cpy_slice_(size_t siz, Seq a, Seq b, Slice s) {
  ce_guard (!siz || !a || !b || (s.ri < s.le) || (s.ri > seq(b)->len))
    return (CE = CE_ARG, a);
  const size_t s_len = s.ri - s.le + 1;
  a = seq_require_(siz, a, s_len);
  if (CE)
    return a;
  seq(a)->len = s_len;
  memcpy(a, ((char*) b) + (s.le * siz), s_len * siz);
  return a;
}

/*
** Stack Operations
*/

Seq seq_push_(size_t siz, Seq s, void *v) {
  ce_guard (!siz || !s || !v)
    return (CE = CE_ARG, s);
  s = seq_set_(siz, s, seq(s)->len, v); // TODO remove call for speed
  return s;
}

void *seq_pop_(size_t siz, Seq s, size_t n) {
  ce_guard (!siz || !s)
    return (CE = CE_ARG, s);
  if (!seq(s)->len)
    return (CE = CE_OOB, s);
  seq(s)->len -= n;
  return ((char*) s) + (seq(s)->len - (n ? 0 : 1)) * siz;
}

void *seq_pull_(size_t siz, Seq s) {
  ce_guard (!siz || !s)
    return (CE = CE_ARG, s);
  if (!seq(s)->len)
    return (CE = CE_OOB, s);
  void *save = ((char*) s) + seq(s)->cap * siz;
  memcpy(save, s, siz);
  memmove(s, ((char*) s) + siz, seq(s)->len-- * siz);
  return save;
}

/*
** Comparison Operations
*/

bool seq_cmp_(size_t siz, Seq a, Seq b) {
  ce_guard(!siz || !a || !b)
    return (CE = CE_ARG, false);
  const size_t a_len = seq(a)->len;
  if (a_len != seq(b)->len)
    return false;
  if (memcmp(a, b, a_len * siz))
    return false;
  return true;
}

bool seq_cmp_len_(size_t siz, Seq a, Seq b, size_t len) {
  ce_guard (!siz || !a || !b)
    return (CE = CE_ARG, false);
  if (len > seq(a)->len || len > seq(b)->len)
    return (CE = CE_OOB, false);
  if (memcmp(a, b, len * siz))
    return false;
  return true;
}

bool seq_cmp_slice_(size_t siz, Seq a, Slice sa, Seq b, Slice sb) {
  ce_guard (!siz || !a || !b)
    return (CE = CE_ARG, false);
  if (!slice_in_len(sa, 0, seq(a)->len))
    return (CE = CE_ARG, false);
  if (!slice_in_len(sb, 0, seq(b)->len))
    return (CE = CE_ARG, false);
  for (size_t i = sa.le, j = sb.le; (i <= sa.ri) && (j <= sb.ri); i++, j++)
    if (memcmp(((char*) a) + siz * i, ((char*) b) + siz * j, siz))
      return false;
  return true;
}
