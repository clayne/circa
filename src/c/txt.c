/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "../h/txt.h"

/*
** Accessors
*/

void txt_clear(Txt t) {
  if (!t) {
    CE = CE_ARG;
    return;
  }
  memset(t, 0, txt(t)->len);
  txt(t)->len = 0;
}

Txt txt_set_(Txt t, size_t a, char v) {
  if (!t || !v)
    return (CE = CE_ARG, t);
  t = txt_require_(t, a + 1);
  if (a + 1 > txt(t)->cap)
    txt(t)->cap = a + 1;
  if (a + 1 > txt(t)->len)
    txt(t)->len = a + 1;
  t[a] = v;
  return t;
}

bool txt_has(Txt t, size_t a) {
  if (!t)
    return false;
  return a < txt(t)->len;
}

char txt_get(Txt t, size_t a) {
  if (!t)
    return (CE = CE_ARG, '\0');
  if (a >= txt(t)->len)
    return (CE = CE_OOB, '\0');
  return t[a];
}

/*
** Allocators
*/

Txt txt_alloc(size_t cap) {
  if (!cap)
    return (CE = CE_ARG, NULL);
  struct txt_data *td = malloc((sizeof(*td) + cap + 1) / 8 * 8 + 8);
  if (!td)
    return (CE = CE_OOM, NULL);
  memset(td, 0, cap + 1);
  td->cap = cap;
  td->len = 0;
  return td->data;
}

Txt txt_wrap(char *c, size_t len) {
  if (!c)
    return (CE = CE_ARG, NULL);
  Txt t = txt_alloc(len);
  if (!t)
    return (CE = CE_OOM, NULL);
  memcpy(t, c, len);
  return t;
}

Txt txt_realloc_(Txt t, size_t cap) {
  if (!t || !cap)
    return (CE = CE_ARG, t);
  struct txt_data *td = txt(t);
  td = realloc(td, (sizeof(*td) + cap) / 8 * 8 + 8);
  if (!td)
    return (CE = CE_OOM, t);
  td->cap = cap;
  return td->data;
}

Txt txt_require_(Txt t, size_t cap) {
  if (!t || !cap)
    return (CE = CE_ARG, t);
  return (txt(t)->cap < cap) ? txt_realloc_(t, cap) : t;
}

Txt txt_free_(Txt t) {
  if (t)
    free(txt(t));
  return NULL;
}

/*
** Stack Operations
*/

Txt txt_push_(Txt t, char v) {
  if (!t || !v)
    return (CE = CE_ARG, t);
  txt_set(t, txt(t)->len, v);
  return t;
}

char txt_pop_(Txt t, size_t n) {
  if (!t)
    return (CE = CE_ARG, '\0');
  txt(t)->len -= n;
  return t[txt(t)->len];
}

/*
** String Operations
*/

bool txt_cmp(Txt a, Txt b) {
  if (!a || !b)
    return (CE = CE_ARG, false);
  const size_t a_len = txt(a)->len;
  if (a_len != txt(b)->len)
    return false;
  for (size_t i = 0; i < a_len; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

bool txt_cmp_len(Txt a, Txt b, size_t len) {
  if (!a || !b)
    return (CE = CE_ARG, false);
  if (len > txt(a)->len || len > txt(b)->len)
    return (CE = CE_OOB, false);
  for (size_t i = 0; i < len; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

/*
** IO Operations
*/

Txt txt_read_(Txt t, FILE *fp) {
  if (!t || !fp)
    return (CE = CE_ARG, t);
  fseek(fp, 0, SEEK_END); // TODO: Error handling.
  size_t cap = (size_t) ftell(fp);
  rewind(fp); // TODO: Consider using fseek instead.
  txt_require(t, cap + 1);
  txt(t)->len = cap;
  fread(t, cap, 1, fp);
  rewind(fp); // TODO: Consider using fseek instead.
  t[cap] = '\0';
  return t;
}

void txt_write(Txt t, FILE *fp) {
  if (!t || !fp) {
    CE = CE_ARG;
    return;
  }
  fputs(t, fp);
  rewind(fp); // TODO: Consider using fseek instead.
}
