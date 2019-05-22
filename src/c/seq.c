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

#include <core.h>
#include <debug.h>
#include <seq.h>

/*
** Acessors
*/

CIRCA
Seq seq_set_(size_t siz, Seq s, size_t a, void *v) {
  circa_guard (!siz || !s || !v)
    return (circa_throw(CE_ARG), NULL);
  s = seq_require_(siz, s, a + 1);
  memcpy(s + siz * a, v, siz);
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
    printf("siz, s, cap: %zu, %zu, %zu", siz, (size_t) s, cap);
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

  return (cap > seq(s)->cap) ? seq_realloc_(siz, s, cap + CIRCA_SEQ_PREALLOC) : s;
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
** Stack Operations
*/

CIRCA CIRCA_RETURNS
Seq seq_push_(size_t siz, Seq s, void *v) {
  circa_guard (!siz || !s || !v)
    return (circa_throw(CE_ARG), s);
  return seq_set_(siz, s, seq(s)->len, v);
}

CIRCA CIRCA_RETURNS
void *seq_pop_(size_t siz, Seq s, size_t n) {
  circa_guard (!siz || !s)
    return (circa_throw(CE_ARG), s);
  if (!seq(s)->len)
    return (circa_throw(CE_OOB), s);
  seq(s)->len -= n;
  return ((char*) s) + (seq(s)->len - (n ? 0 : 1)) * siz;
}
