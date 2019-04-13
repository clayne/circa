/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "txt.h"

/*
** String Operations
*/

Txt txt_cpy_(Txt t, char *c, size_t len) {
  ce_guard (!t || !c || !len)
    return (CE = CE_ARG, t);
  t = txt_require_(t, len + 1);
  if (CE)
    return t;
  txt(t)->len = len;
  memcpy(t, c, len);
  t[len] = '\0';
  return t;
}

Txt txt_cpy_slice_(Txt t, char *c, Slice s) {
  ce_guard (!t || !c || (s.ri < s.le))
    return (CE = CE_ARG, t);
  const size_t s_len = s.ri - s.le + 1;
  t = txt_require_(t, s_len + 1);
  if (CE)
    return t;
  txt(t)->len = s_len;
  memcpy(t, c + s.le, s_len);
  t[s_len] = '\0';
  return t;
}

/*
** Comparison Operations
*/

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
  if (CE)
    return NULL;
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
