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
