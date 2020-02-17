/*
** txt.c | The Circa Library Set | Dynamic text.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Standard */

#include <stdarg.h>

/* Circa */

#include "../h/core.h"
#include "../h/debug.h"
#include "../h/bits.h"
#include "../h/txt.h"

/*
** Accessors
*/

CIRCA
void txt_set_(Txt t, size_t a, char c) {
  circa_guard (!t || !c) {
    circa_throw(CE_ARG);
    return;
  }
  if (a >= txt(t)->len) {
    circa_throw(CE_OOB);
    return;
  }
  t[a] = c;
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
Txt txt_set_len_(Txt t, size_t len) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), t);
  if (len == txt(t)->len)
    return t;
  if (len < txt(t)->len) {
    memset(t + len, 0, txt(t)->len - len);
  } else {
    if (len >= txt(t)->cap)
      t = txt_realloc_(t, len + 1);
    memset(t + txt(t)->len, 0, len - txt(t)->len);
  }
  txt(t)->len = len;
  return t;
}

CIRCA CIRCA_RETURNS
Txt txt_shrink_(Txt t) {
  circa_guard (!t)
    return (circa_throw(CE_ARG), t);

  SeqData *sd = CIRCA_REALLOC(txt(t), sizeof(*sd) + txt(t)->len + 1);

  if (!sd)
    return (circa_throw(CE_OOM), t);
  
  sd->cap = sd->len + 1;

  return sd->data;
}

CIRCA
void txt_clear_(Txt t) {
  circa_guard (!t) {
    circa_throw(CE_ARG);
    return;
  }

  txt(t)->len = 0;
  memset(t, 0, txt(t)->cap);
}

CIRCA CIRCA_RETURNS
Txt txt_free_(Txt t) {
  if (t) {
    memset(txt(t), 0, sizeof(*txt(t)) + txt(t)->cap);
    CIRCA_FREE(txt(t));
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
  dst = txt_set_len_(dst, txt(src)->len);
  if (CE) {
    circa_log("txt_set_len failed.");
    return dst;
  }
  memcpy(dst, src, txt(src)->len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cpy_lit_(Txt dst, char *src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  const size_t len = strlen(src);
  dst = txt_set_len_(dst, len);
  if (CE) {
    circa_log("txt_require failed.");
    return dst;
  }
  memcpy(dst, src, len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cpy_slice_(Txt dst, Txt src, Slice slice) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  const size_t len = slice.ri - slice.le + 1;
  dst = txt_set_len_(dst, len);
  if (CE) {
    circa_log("txt_require failed.");
    return dst;
  }
  memcpy(dst, src + slice.le, len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cpy_slice_lit_(Txt dst, char *src, Slice slice) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  const size_t len = slice.ri - slice.le + 1;
  dst = txt_set_len_(dst, len);
  if (CE) {
    circa_log("txt_require failed.");
    return dst;
  }
  memcpy(dst, src + slice.le, len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_(Txt dst, Txt src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  size_t len = txt(dst)->len + txt(src)->len;
  dst = txt_set_len_(dst, len);
  memcpy(dst + txt(dst)->len, src, txt(src)->len);
  return dst;
}

CIRCA CIRCA_RETURNS
Txt txt_cat_lit_(Txt dst, char *src) {
  circa_guard (!dst || !src)
    return (circa_throw(CE_ARG), dst);
  size_t src_len = strlen(src);
  size_t len = txt(dst)->len + src_len;
  dst = txt_set_len_(dst, len);
  memcpy(dst + txt(dst)->len, src, src_len);
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
  t = txt_set_len_(t, len);
  size_t written = vsnprintf(t, len + 1, fmt, ap);
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
  t = txt_set_len_(t, txt(t)->len + len);
  size_t written = vsnprintf(t + txt(t)->len, len + 1, fmt, ap);
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
  t = txt_set_len_(t, txt(t)->len + 1);
  t[txt(t)->len - 1] = c;
  return t;
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
  t = txt_set_len_(t, len);
  if (CE) {
    circa_log("call to txt_set_len failed");
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
  t = txt_set_len_(t, txt(t)->len + len);
  if (CE) {
    circa_log("call to txt_set_len failed");
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
