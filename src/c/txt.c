/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "../h/txt.h"

/*
** String Operations
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
