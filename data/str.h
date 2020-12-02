/*
** Description
**   An implementation of dynamic ASCII strings with amortized growth.
** Dependencies
**   <stdlib.h>
*/

/*
** Type Definitions
*/

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} str;

/*
** Function Prototypes
*/

static void str_alloc(str *);
static void str_prealloc(str *, size_t);
static void str_require(str *, size_t);
static void str_amort(str *);
static inline void str_free(str *);

static inline char *str_lookup(str *, size_t);
static inline void str_set_v(str *, size_t, char);
static inline void str_set_r(str *, size_t, char *);
static inline char str_get_v(str *, size_t);
static inline void str_get_r(str *, size_t, char *);

static inline void str_snoc_v(str *, char);
static inline void str_snoc_r(str *, char *);

static circa_err str_read(str *, FILE *);
static circa_err str_write(str *, FILE *);

/*
** Function Definitions
*/

static
void str_alloc(str *s) {
  assert(s);
  s->data = malloc(2);
  s->data[0] = '\0';
  s->len = 0;
  s->cap = 2;
}

static
void str_prealloc(str *s, size_t cap) {
  assert(s);
  assert(cap > 1);
  s->data = malloc(cap);
  s->data[0] = '\0';
  s->len = 0;
  s->cap = 2;
}

static
void str_require(str *s, size_t cap) {
  assert(s);
  if (cap > s->cap) {
    s->cap = cap;
    s->data = realloc(s->data, cap);
  }
}

static
void str_amort(str *s) {
  assert(s);
  if (s->len == (s->cap - 1)) {
    s->cap += (s->cap >> 1);
    s->data = realloc(s->data, s->cap);
  }
}

static inline
void str_free(str *s) {
  assert(s);
  free(s->data);
}

static inline
char *str_lookup(str *s, size_t i) {
  assert(s);
  assert(i < s->len);
  return s->data + i;
}

static inline
void str_set_v(str *s, size_t i, char c) {
  assert(s);
  assert(i < s->len);
  assert(c);
  s->data[i] = c;
}

static inline
void str_set_r(str *s, size_t i, char *c) {
  assert(s);
  assert(i < s->len);
  assert(c);
  assert(*c);
  s->data[i] = *c;
}

static inline
char str_get_v(str *s, size_t i) {
  assert(s);
  assert(i < s->len);
  return s->data[i];
}

static inline
void str_get_r(str *s, size_t i, char *c) {
  assert(s);
  assert(i < s->len);
  assert(c);
  *c = s->data[i];
}

static inline
void str_snoc_v(str *s, char c) {
  assert(s);
  assert(c);
  str_amort(s);
  s->data[s->len++] = c;
  s->data[s->len] = '\0';
}

static inline
void str_snoc_r(str *s, char *c) {
  assert(s);
  assert(c);
  assert(*c);
  str_amort(s);
  s->data[s->len++] = *c;
  s->data[s->len] = '\0';
}

static
void str_cat(str *a, str *b) {
  assert(a);
  assert(b);
  str_require(a, a->len + b->len + 1);
  char *c = b->data;
  while (*c)
    a->data[a->len++] = *(c++);
  a->data[a->len] = '\0';
}

static
void str_cat_n(str *a, str *b, size_t n) {
  assert(a);
  assert(b);
  assert(n <= b->len);
  str_require(a, a->len + n + 1);
  for (size_t i = 0; i < n; i++)
    a->data[a->len++] = b->data[i];
}

static
void str_cat_cstr(str *s, char *c) {
  assert(s);
  assert(c);
  while (*c)
    str_snoc_v(s, *(c++));
}

static
void str_cat_cstr_n(str *s, char *c, size_t n) {
  assert(s);
  assert(c);
  assert(n <= strlen(c));
  str_require(s, s->len + n + 1);
  char *stop = c + n + 1;
  while (c != stop)
    s->data[s->len++] = *(c++);
}

static
void str_cat_cstr_len(str *s, char *c, size_t len) {
  assert(s);
  assert(c);
  assert(len == strlen(c));
  str_require(s, s->len + len + 1);
  while (*c)
    s->data[s->len++] = *(c++);
  s->data[s->len] = '\0';
}

static
void str_fmt_va(str *s, const char *fmt, va_list ap) {
  assert(s);
  assert(fmt);
  va_list ap2;
  va_copy(ap2, ap);
  size_t len = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  str_require(s, len + 1);
  size_t written = vsnprintf(s->data, len + 1, fmt, ap);
  assert(written == len);
  s->len = len;
}

static
void str_fmt(str *s, const char *fmt, ...) {
  assert(s);
  assert(fmt);
  va_list ap;
  va_start(ap, fmt);
  str_fmt_va(s, fmt, ap);
  va_end(ap);
}

static
void str_cat_fmt_va(str *s, const char *fmt, va_list ap) {
  assert(s);
  assert(fmt);
  va_list ap2;
  va_copy(ap2, ap);
  size_t len = vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  str_require(s, s->len + len + 1);
  size_t written = vsnprintf(s->data + s->len, len + 1, fmt, ap);
  assert(written == len);
  s->len += len;
}

static
void str_cat_fmt(str *s, const char *fmt, ...) {
  assert(s);
  assert(fmt);
  va_list ap;
  va_start(ap, fmt);
  str_cat_fmt_va(s, fmt, ap);
  va_end(ap);
}

static
circa_err str_read(str *s, FILE *fp) {
  assert(s);
  assert(fp);

  if (fseek(fp, 0, SEEK_END))
    return CIRCA_ERR_READ;

  size_t len = ftell(fp);

  if (fseek(fp, 0, SEEK_SET))
    return CIRCA_ERR_READ;

  str_require(s, len + 1);

  if (fread(s->data, 1, len, fp) != len)
    return CIRCA_ERR_READ;

  s->data[len] = '\0';
  s->len = len;

  return CIRCA_ERR_NONE;
}

static
circa_err str_write(str *s, FILE *fp) {
  assert(s);
  assert(fp);
  if (fputs(s->data, fp) == EOF)
    return CIRCA_ERR_WRITE;
  return CIRCA_ERR_NONE;
}
