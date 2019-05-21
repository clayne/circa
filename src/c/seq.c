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
  circa_guard (!siz || !s || !v) {
    circa_log("threw CE_ARG");
    return (CE = CE_ARG, NULL);
  }

  // TODO: seq_require_
  memcpy(s + siz * a, v, siz);

  return s;
}

CIRCA
void *seq_get_(size_t siz, Seq s, size_t a) {
  circa_guard (!siz || !s) {
    circa_log("threw CE_ARG");
    return (CE = CE_ARG, NULL);
  }

  return ((char*) s) + siz * a;
}

/*
** Allocators
*/

CIRCA CIRCA_ALLOCS
Seq seq_alloc_(size_t siz, size_t cap) {
  circa_guard(!siz || !cap) {
    circa_log("threw CE_ARG");
    return (CE = CE_ARG, NULL);
  }

  SeqData *sd = CIRCA_MALLOC(sizeof(*sd) + cap * siz);
  
  if (!sd) {
    circa_log("threw CE_OOM");
    return (CE = CE_OOM, NULL);
  }
  
  seq(sd)->cap = cap;
  seq(sd)->len = 0;
  
  return sd->data;
}

CIRCA CIRCA_RETURNS
Seq seq_realloc_(size_t siz, Seq s, size_t cap) {
  circa_guard(!siz || !s || !cap) {
    circa_log("threw CE_ARG");
    return (CE = CE_ARG, s);
  }
  
  SeqData *sd = CIRCA_REALLOC(seq(s), sizeof(*sd) + cap * siz);
  
  if (!sd) {
    circa_log("threw CE_OOM");
    return (CE = CE_OOM, s);
  }

  seq(sd)->cap = cap;

  return sd->data;
}
