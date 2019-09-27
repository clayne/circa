/*
** txt.c | The Circa Library Set | Dynamic text.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include "../h/core.h"
#include "../h/debug.h"
#include "../h/bits.h"
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

CIRCA CIRCA_RETURNS
Txt txt_ins_(Txt t, size_t a, char c) {
  circa_guard (!t || !c)
    return (circa_throw(CE_ARG), t);
  register const size_t len = usz_max(txt(t)->len, a) + 1;
  if (len > txt(t)->len + 1)
    return (circa_throw(CE_OOB), t);
  txt(t)->len = len;
  t[txt(t)->len] = '\0';
  t = txt_require_(t, len);
  memmove(t + a + 1, t + a, len - a);
  t[a] = c;
  return t;
}

CIRCA
bool txt_del_(Txt t, size_t a) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), t);
  if (a >= txt(t)->len)
    return false;
  memmove(t + a, t + a + 1, txt(t)->len - (a + 1));
  txt(t)->len--;
  t[txt(t)->len] = '\0';
  return true;
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
  circa_guard (!cap)
    return (circa_throw(CE_ARG), NULL);
  SeqData *sd = CIRCA_CALLOC(sizeof(*sd) + cap, 1);
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
  SeqData *sd = CIRCA_REALLOC(txt(t), sizeof(*sd) + cap);
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

  if (cap <= txt(t)->cap)
    return t;

  SeqData *sd = CIRCA_REALLOC(txt(t), sizeof(*sd) + cap + CIRCA_TXT_PREALLOC);

  if (!sd)
    return (circa_throw(CE_OOM), t);

  sd->cap = cap + CIRCA_TXT_PREALLOC;

  return sd->data;
}

CIRCA CIRCA_RETURNS
Txt txt_shrink_(Txt t) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), t);

  SeqData *sd = CIRCA_REALLOC(txt(t), sizeof(*sd) + txt(t)->len + 1);

  if (!sd)
    return (circa_throw(CE_OOM), t); // Imagine running out of memory on a shrink

  sd->cap = sd->len + 1;

  return sd->data;
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
** String Operations
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
  dst[len] = '\0';
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cpy_slice_lit_(Txt dst, char *src, Slice slice) {
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
  dst[len] = '\0';
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_(Txt dst, Txt src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  size_t len = txt(dst)->len + txt(src)->len;
  dst = txt_require_(dst, len + 1);
  memcpy(dst + txt(dst)->len, src, txt(src)->len);
  txt(dst)->len = len;
  dst[len] = '\0';
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_lit_(Txt dst, char *src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  size_t src_len = strlen(src);
  size_t len = txt(dst)->len + src_len;
  dst = txt_require_(dst, len + 1);
  memcpy(dst + txt(dst)->len, src, src_len);
  txt(dst)->len = len;
  dst[len] = '\0';
  return dst;
}

#ifdef CIRCA_GNU
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
  t = txt_require_(t, len + 1);
  size_t written = vsnprintf(t, len + 1, fmt, ap);
  txt(t)->len = written;
  if (written != len)
    CE = CE_FMT;
  return t;
}

CIRCA CIRCA_RETURNS
Txt txt_fmt_(Txt t, const char *fmt, ...) {
  circa_guard (!t || !fmt)
    return (circa_throw(CE_ARG), t);
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
  t = txt_require_(t, txt(t)->len + len + 1);
  size_t written = vsnprintf(t + txt(t)->len, len + 1, fmt, ap);
  txt(t)->len += written;
  if (written != len)
    CE = CE_FMT;
  return t;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_fmt_(Txt t, const char *fmt, ...) {
  circa_guard (!t || !fmt)
    return (circa_throw(CE_ARG), t);
  va_list ap;
  va_start(ap, fmt);
  t = txt_cat_fmt__(t, fmt, ap);
  va_end(ap);
  return t;
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
  t[txt(t)->len] = '\0';
  return t[txt(t)->len - (n ? 0 : 1)];
}

CIRCA
char txt_pull_(Txt t) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), '\0');
  if (!txt(t)->len)
    return (circa_throw(CE_OOB), '\0');
  char tmp = t[0];
  memmove(t, t + 1, txt(t)->len--);
  t[txt(t)->len] = '\0';
  return tmp;
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

/*
** IO Operations
*/

CIRCA CIRCA_RETURNS
Txt txt_read_(Txt t, FILE *fp) {
  circa_guard (!t || !fp)
    return (circa_throw(CE_ARG), t);
  
  /* Go to the end of the file. */
  if (fseek(fp, 0, SEEK_END)) {
    circa_log("failed to seek end of file");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Get the length of the file. */
  size_t len = (size_t) ftell(fp);

  /* Rewind the file. */
  if (fseek(fp, 0, SEEK_SET)) {
    circa_log("failed to rewind file");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Allocate enough text space to store the file contents. */
  t = txt_require_(t, len + 1);
  if (CE) {
    circa_log("call to txt_require failed");
    return t;
  }
  
  /* Read the file into the text. */
  if (fread(t, 1, len, fp) != len) {
    circa_log("fread bytes did not match length");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Rewind the file. */
  if (fseek(fp, 0, SEEK_SET)) {
    circa_log("failed to rewind file");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Set the length and null-terminate. */
  txt(t)->len = len;
  t[len] = '\0';
  
  return t;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_read_(Txt t, FILE *fp) {
  circa_guard (!t || !fp)
    return (circa_throw(CE_ARG), t);
  
  /* Go to the end of the file. */
  if (fseek(fp, 0, SEEK_END)) {
    circa_log("failed to seek end of file");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Get the length of the file. */
  size_t len = (size_t) ftell(fp);

  /* Rewind the file. */
  if (fseek(fp, 0, SEEK_SET)) {
    circa_log("failed to rewind file");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Allocate enough text space to store the file contents. */
  t = txt_require_(t, txt(t)->len + len + 1);
  if (CE) {
    circa_log("call to txt_require failed");
    return t;
  }
  
  /* Read the file into the text. */
  if (fread(t + txt(t)->len, 1, len, fp) != len) {
    circa_log("fread bytes did not match length");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Rewind the file. */
  if (fseek(fp, 0, SEEK_SET)) {
    circa_log("failed to rewind");
    return (circa_throw(CE_FILE_READ), t);
  }

  /* Set the length and null-terminate. */
  txt(t)->len += len;
  t[txt(t)->len] = '\0';
  
  return t;
}

CIRCA
void txt_write_(Txt t, FILE *fp) {
  circa_guard (!t || !fp) {
    CE = CE_ARG;
    return;
  }
  fputs(t, fp);
  if (!fseek(fp, 0, SEEK_SET)) {
    CE = CE_FILE_WRITE;
    return;
  }
}
