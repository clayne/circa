/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "../h/txt.h"

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
