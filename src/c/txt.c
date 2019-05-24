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
  t = txt_require_(t, a + 2);
  t[a] = c;
  if (txt(t)->len < a + 1)
    txt(t)->len = a + 1;
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

/*
** Sequence Operations
*/

CIRCA CIRCA_RETURNS
Txt txt_cpy_(Txt dst, Txt src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  dst = txt_require_(dst, txt(src)->len + 1);
  if (CE) {
    circa_log("txt_require failed.");
    return dst;
  }
  memcpy(dst, src, txt(src)->len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cpy_slice_(Txt dst, Txt src, Slice slice) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  const size_t len = slice.ri - slice.le + 1;
  dst = txt_require_(dst, len + 1);
  if (CE) {
    circa_log("txt_require failed.");
    return dst;
  }
  seq(dst)->len = len;
  memcpy(dst, src + slice.le, len);
  return dst;
}

/*
** Stack Operations
*/

CIRCA CIRCA_RETURNS
Txt txt_push_(Txt t, char c) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), t);
  t = txt_set_(t, txt(t)->len, c);
  return t;
}

CIRCA
char txt_pop_(Txt t, size_t n) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), '\0');
  if (!txt(t)->len)
    return (circa_throw(CE_OOB), '\0');
  txt(t)->len -= n;
  return t[txt(t)->len - (n ? 0 : 1)];
}

CIRCA
char txt_pull_(Txt t) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), '\0');
  if (!txt(t)->len)
    return (circa_throw(CE_OOB), '\0');
  char tmp;
  tmp = t[0];
  memmove(t, t + 1, txt(t)->len--);
  t[txt(t)->len] = tmp;
  return t[txt(t)->len];
}

/*
** Comparison Operations
*/

CIRCA
bool txt_cmp_(Txt a, Txt b) {
  circa_guard (!a || !b)
    return (circa_throw(CE_ARG), false);
  if (txt(a)->len != txt(b)->len)
    return false;
  return strncmp(a, b, seq(a)->len);
}

CIRCA
bool txt_cmp_slice_(Txt a, Slice sa, Txt b, Slice sb) {
  circa_guard (!a || !b)
    return (circa_throw(CE_ARG), false);
  if (!slice_in_len(sa, 0, txt(a)->len))
    return (circa_throw(CE_OOB), false);
  if (!slice_in_len(sa, 0, txt(b)->len))
    return (circa_throw(CE_OOB), false);
  for (size_t i = sa.le, j = sb.le; (i <= sa.ri) && (j <= sb.ri); i++, j++)
    if (a[i] != b[i])
      return false;
  return true;
}
