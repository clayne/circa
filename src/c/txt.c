/*
** txt.c | The Circa Library Set | Dynamic text.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include "../h/txt.h"

/*
** Accessors
*/

CIRCA CIRCA_RETURNS
Txt txt_set_(Txt t, size_t a, char c) {
  circa_guard (!t || !c)
    return (circa_throw(CE_ARG), t);
  t = txt_require_(t, a + 1);
  t[a] = c;
  return t;
}

CIRCA
char txt_get_(Txt t, size_t a) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), '\0');
  if (a > txt(t)->len)
    return (circa_throw(CE_OOB), '\0');
  return t[a];
}

/*
** Allocators
*/

CIRCA CIRCA_ALLOCS
Txt txt_alloc_(size_t cap) {
  SeqData *sd = calloc(sizeof(*sd) + cap, 1);
  if (!sd)
    return (circa_throw(CE_OOM), NULL);
  sd->cap = cap;
  return sd->data;
}

CIRCA CIRCA_RETURNS
Txt txt_realloc_(Txt t, size_t cap) {
  circa_guard (!t || !cap)
    return (circa_throw(CE_ARG), t);
  if (cap < txt(t)->cap)
    memset(t + cap, 0, txt(t)->cap - cap);
  SeqData *sd = realloc(txt(t), sizeof(*sd) + cap);
  if (!sd)
    return (circa_throw(CE_OOM), t);
  if (cap > sd->cap)
    memset(sd->data + sd->cap - 1, 0, cap - sd->cap);
  sd->cap = cap;
  return sd->data;
}

CIRCA CIRCA_RETURNS
Txt txt_require_(Txt t, size_t cap) {
  circa_guard (!t || !cap)
    return (circa_throw(CE_ARG), t);
  
  t = (cap > txt(t)->cap) ? txt_realloc_(t, cap + CIRCA_TXT_PREALLOC) : t;

  if (CE)
    circa_log("call to txt_realloc failed.");

  return t;
}

CIRCA CIRCA_RETURNS
Txt txt_free_(Txt t) {
  if (t) {
    memset(txt(t), 0, sizeof(*txt(t)) + txt(t)->cap);
    free(txt(t));
  }

  return NULL;
}
