/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "../h/txt.h"

/*
** Accessors
*/

void txt_clear(Txt t) {
  ce_guard (!t) {
    CE = CE_ARG;
    return;
  }
  memset(t, 0, txt(t)->len);
  txt(t)->len = 0;
}

Txt txt_set_(Txt t, size_t a, char v) {
  ce_guard (!t || !v)
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
  ce_guard (!t)
    return false;
  return a < txt(t)->len;
}

char txt_get(Txt t, size_t a) {
  ce_guard (!t)
    return (CE = CE_ARG, '\0');
  if (a >= txt(t)->len)
    return (CE = CE_OOB, '\0');
  return t[a];
}

/*
** Allocators
*/

Txt txt_alloc(size_t cap) {
  ce_guard (!cap)
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
  ce_guard (!c)
    return (CE = CE_ARG, NULL);
  Txt t = txt_alloc(len);
  if (!t)
    return (CE = CE_OOM, NULL);
  memcpy(t, c, len);
  return t;
}

Txt txt_realloc_(Txt t, size_t cap) {
  ce_guard (!t || !cap)
    return (CE = CE_ARG, t);
  struct txt_data *td = txt(t);
  td = realloc(td, (sizeof(*td) + cap) / 8 * 8 + 8);
  if (!td)
    return (CE = CE_OOM, t);
  td->cap = cap;
  return td->data;
}

Txt txt_require_(Txt t, size_t cap) {
  ce_guard (!t || !cap)
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
  ce_guard (!t || !v)
    return (CE = CE_ARG, t);
  txt_set(t, txt(t)->len, v);
  return t;
}

char txt_pop_(Txt t, size_t n) {
  ce_guard (!t)
    return (CE = CE_ARG, '\0');
  if (!txt(t)->len)
    return (CE = CE_OOB, '\0');
  txt(t)->len -= n;
  return t[txt(t)->len];
}

/*
** String Operations
*/

bool txt_cmp(Txt a, Txt b) {
  ce_guard (!a || !b)
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
  ce_guard (!a || !b)
    return (CE = CE_ARG, false);
  if (len > txt(a)->len || len > txt(b)->len)
    return (CE = CE_OOB, false);
  for (size_t i = 0; i < len; i++)
    if (a[i] != b[i])
      return false;
  return true;
}

bool txt_cmp_slice(Txt a, Slice sa, Txt b, Slice sb) {
  ce_guard (!a || !b)
    return (CE = CE_ARG, false);
  if (!slice_in_len(sa, 0, txt(a)->len))
    return (CE = CE_ARG, false);
  if (!slice_in_len(sb, 0, txt(b)->len))
    return (CE = CE_ARG, false);
  for (size_t i = sa.le, j = sb.le; (i <= sa.ri) && (j <= sb.ri); i++, j++)
    if (a[i] != b[j])
      return false;
  return true;
}

bool txt_cmp_slice_lit(Txt t, Slice s, char *c) {
  ce_guard (!t || !c)
    return (CE = CE_ARG, false);
  // TODO: More checks.
  for (size_t i = s.le; i <= s.ri; i++)
    if (t[i] != c[i - s.le])
      return false;
  return true;
}

/*
** IO Operations
*/

Txt txt_read_(Txt t, FILE *fp) {
  ce_guard (!t || !fp)
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
  ce_guard (!t || !fp) {
    CE = CE_ARG;
    return;
  }
  fputs(t, fp);
  rewind(fp); // TODO: Consider using fseek instead.
}
