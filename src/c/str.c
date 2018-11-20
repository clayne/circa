/*
** str.c | The Circa Library Set | Dynamically allocated strings.
** https://github.com/davidgarland/circa
*/

#include "../h/str.h"

/*
** Accessors
*/

/* Access the underlying structure of a string. */

struct str_data *str_(Str s, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return ((struct str_data*) s) - 1;
}

/* Set the value at an index from a string. */

_circa_rets_
Str str_set_(Str s, size_t a, char c, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(c != '\0', fname, line);
  }
  s = str_rqr_(s, a + 2, fname, line);
  if (a >= str(s)->len)
    str(s)->len = a + 1;
  s[a] = c;
  s[a + 1] = '\0';
  return s;
}

/* Get the value at an index from a string. */

char str_get_(Str s, size_t a, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(a < str(s)->len, fname, line);
  }
  return s[a];
}

/*
** Allocators
*/

/* Allocates a new string. */

_circa_alcs_
Str str_new_(size_t cap, CIRCA_ARGS) {
  {
    circa_assert(cap > 0, fname, line);
  }
  struct str_data *restrict s = NULL;
  while (s == NULL)
    s = calloc(sizeof(*s) + cap, 1);
  s->cap = cap;
  return s->data;
}

/* Allocates a new string holding a C string literal. */

_circa_alcs_
Str str_lit_(const char *const restrict cs, CIRCA_ARGS) {
  {
    circa_assert(cs != NULL, fname, line);
  }
  register const size_t len = strlen(cs);
  Str s = str_new(len + 1);
  memcpy(s, cs, len + 1);
  return s;
}

/* Creates a string from a normal array. */

_circa_alcs_
Str str_wrap_(char *cs, size_t len, CIRCA_ARGS) {
  {
    circa_assert(cs != NULL, fname, line);
  }
  Str s = str_new(len + 1);
  memcpy(s, cs, len + 1);
  return s;
}

/* Resizes a string. */

_circa_rets_
Str str_rsz_(Str s, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  struct str_data *const restrict sp = str(s);
  struct str_data *restrict sp2 = NULL;
  if (cap < sp->cap) // Zero-initialize any freed memory.
    memset(s + cap, 0, sp->cap - cap);
  while (sp2 == NULL)
    sp2 = realloc(sp, sizeof(*sp) + cap);
  // TODO: Debug this.
  //if (cap > sp->cap) // Zero-initialize any new memory.
  //  memset(sp2->data + sp->cap, 0, cap - sp->cap);
  sp2->cap = cap;
  return sp2->data;
}

/* Requires that a string be able to hold a given capacity. */

_circa_rets_
Str str_rqr_(Str s, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  return (cap > str(s)->cap) ? str_rsz_(s, cap, fname, line) : s;
}

/* Shrinks a string as much as possible. */

_circa_rets_
Str str_shr_(Str s, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return str_rsz_(s, str(s)->len + 1, fname, line);
}

/* Deletes a string. */

_circa_rets_
Str str_del_(Str s, CIRCA_ARGS) {
  if (s) {
    memset(str(s), 0, sizeof(*str(s)) + str(s)->cap);
    free(str(s));
  }
  return NULL;
}

/*
** Stack Ops
*/

/* Pushes a value on top of a string. */

_circa_rets_
Str str_push_(Str s, char c, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
  }
  s = str_set_(s, str(s)->len, c, fname, line);
  return s;
}

/* Pops a value off of a string. */

char str_pop_(Str s, size_t n, CIRCA_ARGS) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(str(s)->len > 0, fname, line);
  }
  const char c = s[str(s)->len - 1];
  str(s)->len -= n;
  return c;
}

/*
** String Ops 
*/

// TODO
