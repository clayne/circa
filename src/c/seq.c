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
#include "../h/seq.h"

/*
** Acessors
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
  circa_guard(!siz || !cap)
    return (circa_throw(CE_ARG), NULL);

  SeqData *sd = CIRCA_MALLOC(sizeof(*sd) + cap * siz);
  
  if (!sd)
    return (circa_throw(CE_OOM), NULL);
  
  sd->cap = cap;
  sd->len = 0;
  
  return sd->data;
}

CIRCA CIRCA_RETURNS
Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  circa_guard (!siz || !s || !cap) {
    return (circa_throw(CE_ARG), s);
  }

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
    free(seq(s));
  }

  return NULL;
}

/*
** Sequence Operations
*/

CIRCA CIRCA_RETURNS
Seq seq_cpy_(size_t siz, Seq dst, Seq src) {
  circa_guard (!siz || !dst || !src)
    return (circa_throw(CE_ARG), dst);
  dst = seq_require_(siz, dst, seq(src)->len);
  memcpy(dst, src, seq(src)->len * siz);
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
    tmp = malloc(siz);
    if (!tmp)
      return (circa_throw(CE_OOM), s);
  #endif

  memcpy(tmp, s, siz);
  memmove(s, ((char*) s) + siz, seq(s)->len-- * siz);
  void *save = ((char*) s) + seq(s)->len * siz;
  memcpy(save, tmp, siz);

  #ifndef CIRCA_VLA
    free(tmp);
  #endif

  return save;
}
