/*
** seq.c | The Circa Library Set | Dynamic sequences.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Standard */

#include <string.h>

/* Circa */

#include "../h/core.h"
#include "../h/debug.h"
#include "../h/bits.h"
#include "../h/seq.h"

/*
** Accessors
*/

CIRCA
Seq seq_set_(size_t siz, Seq s, size_t a, void *v) {
  circa_guard (!siz || !s || !v)
    return (circa_throw(CE_ARG), NULL);
  s = seq_require_(siz, s, a + 1);
  memcpy(((char*) s) + siz * a, v, siz);
  if (seq(s)->len < a + 1)
    seq(s)->len = a + 1;
  return s;
}

CIRCA CIRCA_RETURNS
Seq seq_ins_(size_t siz, Seq s, size_t a, void *v) {
  circa_guard (!siz || !s || !v)
    return (circa_throw(CE_ARG), s);
  register const size_t len = usz_max(seq(s)->len, a) + 1;
  if (len > seq(s)->len + 1)
    memset(((char*) s) + siz * seq(s)->len, 0, (len - seq(s)->len + 1) * siz);
  seq(s)->len = len;
  s = seq_require_(siz, s, len);
  memmove(((char*) s) + siz * (a + 1), ((char*) s) + siz * a, siz * (len - a));
  memcpy(((char*) s) + siz * a, v, siz);
  return s;
}

CIRCA
bool seq_del_(size_t siz, Seq s, size_t a) {
  circa_guard (!siz || !s)
    return (circa_throw(CE_ARG), s);
  if (a >= seq(s)->len)
    return false;
  memmove(
    ((char*) s) + siz * a,
    ((char*) s) + siz * (a + 1),
    siz * (seq(s)->len - (a + 1))
  );
  seq(s)->len--;
  return true;
}

CIRCA
void *seq_get_(size_t siz, Seq s, size_t a) {
  circa_guard (!siz || !s)
    return (circa_throw(CE_ARG), NULL);
  return ((char*) s) + siz * a;
}

/*
** Allocators
*/

CIRCA CIRCA_ALLOCS
Seq seq_alloc_(size_t siz, size_t cap) {
  circa_guard (!siz || !cap)
    return (circa_throw(CE_ARG), NULL);

  SeqData *sd = CIRCA_MALLOC(sizeof(*sd) + cap * siz);
  
  if (!sd)
    return (circa_throw(CE_OOM), NULL);
  
  sd->cap = cap;
  sd->len = 0;
  
  return sd->data;
}

CIRCA CIRCA_ALLOCS
Seq seq_wrap_(size_t siz, void *v, size_t cap) {
  circa_guard (!siz || !v || !cap)
    return (circa_throw(CE_ARG), NULL);
  Seq s = seq_alloc_(siz, cap);
  memcpy(s, v, cap * siz);
  seq(s)->len = cap;
  return s;
}

CIRCA CIRCA_RETURNS
Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  circa_guard (!siz || !s || !cap)
    return (circa_throw(CE_ARG), s);

  if (cap < seq(s)->cap)
    memset(((char*) s) + cap * siz, 0, seq(s)->cap - cap);

  SeqData *sd = CIRCA_REALLOC(seq(s), sizeof(*sd) + cap * siz);
 
  if (!sd)
    return (circa_throw(CE_OOM), s);

  sd->cap = cap;
  sd->len = sd->len > cap ? cap : sd->len;

  return sd->data;
}

CIRCA CIRCA_RETURNS
Seq seq_require_(size_t siz, Seq s, size_t cap) {
  circa_guard (!siz || !s || !cap)
    return (circa_throw(CE_ARG), s);

  s = (cap > seq(s)->cap) ? seq_realloc_(siz, s, cap + CIRCA_SEQ_PREALLOC) : s;
  
  if (CE)
    circa_log("call to seq_realloc failed.");
  
  return s;
}

CIRCA CIRCA_RETURNS
Seq seq_free_(size_t siz, Seq s) {
  circa_guard (!siz)
    circa_throw(CE_ARG);

  if (s) {
    memset(seq(s), 0, sizeof(*seq(s)) + seq(s)->cap * siz);
    CIRCA_FREE(seq(s));
  }

  return NULL;
}

/*
** Sequence Operations
*/

CIRCA CIRCA_RETURNS
Seq seq_cpy_(size_t siz, Seq dst, void *src, size_t cap) {
  circa_guard (!siz || !dst || !src || !cap)
    return (circa_throw(CE_ARG), dst);
  dst = seq_require_(siz, dst, cap);
  if (CE) {
    circa_log("seq_require failed.");
    return dst;
  }
  memcpy(dst, src, cap * siz);
  seq(dst)->len = cap;
  return dst;
}

CIRCA CIRCA_RETURNS
Seq seq_cpy_slice_(size_t siz, Seq dst, Seq src, Slice slice) {
  circa_guard (!siz || !dst || !src)
    return (circa_throw(CE_ARG), dst);
  const size_t len = slice.ri - slice.le + 1;
  dst = seq_require_(siz, dst, len);
  if (CE) {
    circa_log("seq_require failed.");
    return dst;
  }
  seq(dst)->len = len;
  memcpy(dst, ((char*) src) + (slice.le * siz), len * siz);
  return dst;
}

/*
** Stack Operations
*/

CIRCA CIRCA_RETURNS
Seq seq_push_(size_t siz, Seq s, void *v) {
  circa_guard (!siz || !s || !v)
    return (circa_throw(CE_ARG), s);
  return seq_set_(siz, s, seq(s)->len, v);
}

CIRCA
void *seq_pop_(size_t siz, Seq s, size_t n) {
  circa_guard (!siz || !s)
    return (circa_throw(CE_ARG), s);
  if (!seq(s)->len)
    return (circa_throw(CE_OOB), s);
  seq(s)->len -= n;
  return ((char*) s) + (seq(s)->len - (n ? 0 : 1)) * siz;
}

CIRCA
void *seq_pull_(size_t siz, Seq s) {
  circa_guard (!siz || !s)
    return (circa_throw(CE_ARG), s);
  if (!seq(s)->len)
    return (circa_throw(CE_OOB), s);

  #ifdef CIRCA_VLA
    char tmp[siz];
  #else
    char *tmp = NULL;
    tmp = CIRCA_MALLOC(siz);
    if (!tmp)
      return (circa_throw(CE_OOM), s);
  #endif

  memcpy(tmp, s, siz);
  memmove(s, ((char*) s) + siz, seq(s)->len-- * siz);
  void *save = ((char*) s) + seq(s)->len * siz;
  memcpy(save, tmp, siz);

  #ifndef CIRCA_VLA
    CIRCA_FREE(tmp);
  #endif

  return save;
}

/*
** Comparison Operations
*/

CIRCA
bool seq_cmp_(size_t siz, Seq a, Seq b) {
  circa_guard (!siz || !a || !b)
    return (circa_throw(CE_ARG), false);
  if (seq(a)->len != seq(b)->len)
    return false;
  return !memcmp(a, b, seq(a)->len * siz);
}

CIRCA
bool seq_cmp_len_(size_t siz, Seq a, Seq b, size_t len) {
  circa_guard (!siz || !a || !b || !len)
    return (circa_throw(CE_ARG), false);
  if (len > seq(a)->len || len > seq(b)->len)
    return false;
  return !memcmp(a, b, seq(a)->len * siz);
}

CIRCA
bool seq_cmp_slice_(size_t siz, Seq a, Slice sa, Seq b, Slice sb) {
  circa_guard (!siz || !a || !b)
    return (circa_throw(CE_ARG), false);
  if (!slice_in_len(sa, 0, seq(a)->len))
    return (circa_throw(CE_OOB), false);
  if (!slice_in_len(sb, 0, seq(b)->len))
    return (circa_throw(CE_OOB), false);
  if ((sa.ri - sa.le) != (sb.ri - sb.le))
    return false;
  for (size_t i = sa.le, j = sb.le; (i <= sa.ri) && (j <= sb.ri); i++, j++)
    if (memcmp(((char*) a) + siz * i, ((char*) b) + siz * j, siz))
      return false;
  return true;
}
