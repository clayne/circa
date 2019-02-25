/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "../h/txt.h"

/*
** String Operations
*/

Txt txt_cpy_(Txt a, Txt b) {
  ce_guard (!a || !b)
    return (CE = CE_ARG, a);
  const size_t b_len = txt(b)->len;
  a = txt_require_(a, b_len + 1);
  if (CE)
    return a;
  txt(a)->len = b_len;
  memcpy(a, b, b_len);
  a[b_len] = '\0';
  return a;
}

Txt txt_cpy_slice_(Txt a, Txt b, Slice s) {
  ce_guard (!a || !b || (s.ri < s.le) || (s.ri > txt(b)->len))
    return (CE = CE_ARG, a);
  const size_t s_len = s.ri - s.le + 1;
  a = txt_require_(a, s_len + 1);
  txt(a)->len = s_len;
  memcpy(a, b + s.le, s_len);
  a[s_len] = '\0';
  return a;
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
