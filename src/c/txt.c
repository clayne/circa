/*
** txt.c | The Circa Library Set | Dynamic Text
** https://github.com/davidgarland/circa
*/

#include "txt.h"

/*
** String Functions
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

/* VPRINTF() macro courtesy of @craigbarnes. */

#ifdef __GNUC__
  #define VPRINTF(X) __attribute__((__format__(__printf__, (X), 0)))
#else
  #define VPRINTF(X)
#endif

static inline VPRINTF(2)
Txt txt_fmt__(Txt t, const char *fmt, va_list ap) {
  va_list ap2;
  va_copy(ap2, ap);
  size_t len = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  txt_require(t, len + 1);
  size_t written = vsnprintf(t, len + 1, fmt, ap);
  txt(t)->len = written;
  if (written != len)
    CE = CE_FMT;
  return t;
}

Txt txt_fmt_(Txt t, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  t = txt_fmt__(t, fmt, ap);
  va_end(ap);
  return t;
}

static inline VPRINTF(2)
Txt txt_cat_fmt__(Txt t, const char *fmt, va_list ap) {
  va_list ap2;
  va_copy(ap2, ap);
  size_t len = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  txt_require(t, txt(t)->len + len + 1);
  size_t written = vsnprintf(t + txt(t)->len, len + 1, fmt, ap);
  txt(t)->len += written;
  if (written != len)
    CE = CE_FMT;
  return t;
}

Txt txt_cat_fmt_(Txt t, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  t = txt_cat_fmt__(t, fmt, ap);
  va_end(ap);
  return t;
}

/*
** Comparison Functions
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
** IO Functions
*/

Txt txt_read_(Txt t, FILE *fp) {
  ce_guard (!t || !fp)
    return (CE = CE_ARG, t);
  fseek(fp, 0, SEEK_END); // TODO: Error handling.
  size_t len = (size_t) ftell(fp);
  rewind(fp); // TODO: Consider using fseek instead.
  txt_require(t, len + 1);
  if (CE)
    return t;
  fread(t, len, 1, fp);
  rewind(fp); // TODO: Consider using fseek instead.
  txt(t)->len = len;
  t[len] = '\0';
  return t;
}

Txt txt_cat_read_(Txt t, FILE *fp) {
  ce_guard (!t || !fp)
    return (CE = CE_ARG, t);
  fseek(fp, 0, SEEK_END); // TODO: Error handling.
  size_t len = (size_t) ftell(fp);
  rewind(fp); // TODO: Consider using fseek instead.
  txt_require(t, seq(t)->len + len + 1);
  if (CE)
    return t;
  fread(t + seq(t)->len, len, 1, fp);
  rewind(fp);
  txt(t)->len += len;
  t[txt(t)->len] = '\0';
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
