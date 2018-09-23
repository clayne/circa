/*
** str.h | Dynamically allocated Strings.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_STR_H
#define CIRCA_STR_H

/*
** Compatibility Checks
*/

#ifndef __STDC_VERSION__
  #error "[circa/str.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/str.h]: C99 is required."
#endif

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/*
** Dependencies
*/

/* Standard */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Internal */

#include "core.h"

/*
** Type Definitions
*/

typedef char *Str;

struct str_data {
  size_t cap;
  size_t len;
  char data[];
};

/*
** Function Prototypes
*/

/* Accessors */

#define str(S) str_((S), __FILE__, _circa_str_(__LINE__))
_circa_ struct str_data *str_(Str s, circa_msg fname, circa_msg line);

/* Allocators */

#define str_new(C) str_new_((C), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_alcs_ Str str_new_(size_t cap, circa_msg fname, circa_msg line);

#define str_lit(CS) str_lit_((CS), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_alcs_ Str str_lit_(const char *restrict cs, circa_msg fname, circa_msg line);

#define str_from(S) str_from_((S), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_alcs_ Str str_from_(Str s, circa_msg fname, circa_msg line);

#define str_wrap(S) str_wrap_((S), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_alcs_ Str str_wrap_(char *cs, circa_msg fname, circa_msg line);

#define str_wrap_len(S, L) str_wrap_len_((S), (L), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_alcs_ Str str_wrap_len_(char *cs, size_t len, circa_msg fname, circa_msg line);

#define str_rsz(S, C) (S) = str_rsz_((S), (C), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_rsz_(Str s, size_t cap, circa_msg fname, circa_msg line);

#define str_rqr(S, C) (S) = str_rqr_((S), (C), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_rqr_(Str s, size_t cap, circa_msg fname, circa_msg line);

#define str_rqr_ext(S, C, P) (S) = str_rqr_ext_((S), (C), (P), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_rqr_ext_(Str s, size_t cap, size_t pre, circa_msg fname, circa_msg line);

#define str_shr(S) (S) = str_shr_((S), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_shr_(Str s, circa_msg fname, circa_msg line);

#define str_del(S) (S) = str_del_((S), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_del_(Str s, circa_msg fname, circa_msg line);

/* Evaluators */

#define str_eq(A, B) str_eq_((A), (B), __FILE__, _circa_str_(__LINE__))
_circa_ bool str_eq_(Str a, Str b, circa_msg fname, circa_msg line);

#define str_eq_len(A, B, T, P) str_eq_len_((A), (B), (T), (P), __FILE__, _circa_str_(__LINE__))
_circa_ bool str_eq_len_(Str a, Str b, size_t start, size_t stop, circa_msg fname, circa_msg line);

#define str_eq_lit(S, C) str_eq_lit_((S), (C), __FILE__, _circa_str_(__LINE__))
_circa_ bool str_eq_lit_(Str s, const char *restrict c, circa_msg fname, circa_msg line);

#define str_eq_lit_len(S, C, T, P) str_eq_lit_len_((S), (C), (T), (P), __FILE__, _circa_str_(__LINE__))
_circa_ bool str_eq_lit_len_(Str s, const char *restrict c, size_t start, size_t stop, circa_msg fname, circa_msg line);

#define str_hash(S) str_hash_((S), __FILE__, _circa_str_(__LINE__))
_circa_ size_t str_hash_(Str s, circa_msg fname, circa_msg line);

/* Stack Operations */

#define str_push(S, C) (S) = str_push_((S), (C), __FILE__, _circa_str_(__LINE__))
_circa_ Str str_push_(Str s, char ch, circa_msg fname, circa_msg line);

#define str_tos(S) str_tos_((S), __FILE__, _circa_str_(__LINE__))
_circa_ char str_tos_(Str s, circa_msg fname, circa_msg line);

#define str_pop(S) str_pop_((S), __FILE__, _circa_str_(__LINE__))
_circa_ char str_pop_(Str s, circa_msg fname, circa_msg line);

/* String Operations */

#define str_cpy(DST, SRC) (DST) = str_cpy_((DST), (SRC), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_cpy_(Str dst, Str src, circa_msg fname, circa_msg line);

#define str_cat(DST, SRC) (DST) = str_cat_((DST), (SRC), __FILE__, _circa_str_(__LINE__))
_circa_ _circa_rets_ Str str_cat_(Str dst, Str src, circa_msg fname, circa_msg line);

#define str_rvs(S) str_rvs_((S))
_circa_ void str_rvs_(Str s, circa_msg fname, circa_msg line);

/* IO Operations */

#define str_read(S, F) (S) = str_read_((S), (F), __FILE__, _circa_str_(__LINE__))
_circa_ Str str_read_(Str s, FILE *fp, circa_msg fname, circa_msg line);

#define str_write(S, F) str_write_((S), (F), __FILE__, _circa_str_(__LINE__))
_circa_ void str_write_(Str s, FILE *fp, circa_msg fname, circa_msg line);

/*
** Accessors
*/

/*
** -- str_ --
** Description
**   Returns a pointer to the structure of a sequence.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String Structure Pointer (struct StrData *)
*/

_circa_
struct str_data *str_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  #if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-align"
  #endif
  return ((struct str_data*) s) - 1;
  #if defined(__GNUC__)
    #pragma GCC diagnostic pop
  #endif
}

/*
** -- str_set_ --
** Description
**   Sets a string's character at a given address.
** Arguments
**   s: String (Str)
**   addr: Address (size_t)
**   ch: Character (char)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String(Str)
*/

_circa_ _circa_rets_
Str str_set_(Str s, size_t addr, char ch, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(ch != '\0', fname, line);
  }
  s = str_rqr_(s, addr + 2, fname, line);
  if (addr >= str(s)->len) str(s)->len = addr + 2;
  s[addr] = ch;
  s[addr + 1] = '\0';
  return s;
}

/*
** -- str_get_ --
** Description
**   Gets a string's character at a given address.
** Arguments
**   s: String (Str)
**   addr: Address (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Character (char)
*/

_circa_
char str_get_(Str s, size_t addr, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(str(s)->len < 0, fname, line);
    circa_assert(addr < str(s)->len, fname, line);
  }
  return s[addr];
}


/* Allocators */

/*
** -- str_new_ --
** Description
**   Allocates a string of a given capacity.
** Arguments
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_new_(size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(cap > 0, fname, line);
  }
  struct str_data *s = NULL;
  while (s == NULL) s = malloc(sizeof(*s) + cap + 1);
  s->cap = cap;
  s->len = 0;
  return s->data;
}

/*
** -- str_lit_len_ --
** Description
**   Creates a string from a C string literal of a specified length.
** Arguments
**   cs: C String Literal (const char *restrict)
**   len: Length (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_lit_len_(const char *restrict cs, size_t len, circa_msg fname,
circa_msg line) {
  {
    circa_assert(cs != NULL, fname, line);
    circa_assert(len > 0, fname, line);
    circa_assert(strlen(cs) >= len, fname, line);
  }
  Str s = str_new_(len + 1, fname, line);
  str(s)->len = len;
  memcpy(s, cs, len);
  s[len] = '\0';
  return s;
}

/*
** -- str_lit_ --
** Description
**   Creates a string from a C string literal.
** Arguments
**   cs: C String Literal (const char *restrict)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_lit_(const char *restrict cs, circa_msg fname, circa_msg line) {
  {
    circa_assert(cs != NULL, fname, line);
  }
  const size_t cs_len = strlen(cs);
  Str s = str_new_(cs_len + 1, fname, line);
  str(s)->len = cs_len;
  memcpy(s, cs, cs_len + 1);
  return s;
}

/*
** -- str_from_ --
** Description
**   Creates a string from another.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_from_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  const size_t s_len = str(s)->len;
  Str s2 = str_new_(s_len + 1, fname, line);
  str(s2)->len = s_len;
  memcpy(s2, s, s_len + 1);
  return s2;
}

/*
** -- str_wrap_ --
** Description
**   Creates a new string from a C string.
** Arguments
**   cs: C String (char*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_wrap_(char *cs, circa_msg fname, circa_msg line) {
  {
    circa_assert(cs != NULL, fname, line);
  }
  const size_t cs_len = strlen(cs);
  Str s = str_new_(cs_len + 1, fname, line);
  str(s)->len = cs_len;
  memcpy(s, cs, cs_len + 1);
  return s;
}

/*
** -- str_wrap_len_ --
** Description
**   Creates a new string from a C string of a given length.
** Arguments
**   cs: C String (char*)
**   len: Length (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**  String (Str)
*/

_circa_ _circa_alcs_
Str str_wrap_len_(char *cs, size_t len, circa_msg fname, circa_msg line) {
  {
    circa_assert(cs != NULL, fname, line);
    circa_assert(len > 0, fname, line);
  }
  Str s = str_new_(len + 1, fname, line);
  str(s)->len = len;
  memcpy(s, cs, len);
  s[len] = '\0';
  return s;  
}

/*
** -- str_rsz_ --
** Description
**   Resizes a string to a given capacity.
** Arguments
**   s: String (Str)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_rsz_(Str s, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(cap > str(s)->len, fname, line);
  }
  #ifdef CIRCA_SECURE
    if (cap < str(s)->cap) {
      memset(s + cap, 0, (str(s)->cap - cap));
    }
  #endif
  struct str_data *s2 = NULL;
  while (s2 == NULL) s2 = realloc(str(s), sizeof(*s2) + cap + 1);
  s2->cap = cap;
  return s2->data;
}

/*
** -- str_rqr_ --
** Description
**   Requires that a string be able to hold a given capacity.
** Arguments
**   s: String (Str)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_rqr_(Str s, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return str_rqr_ext_(s, cap, 0, fname, line);
}

/* -- str_rqr_ext_ --
** Description
**   Requires that a string be able to give a holden capacity.
**   If it cannot, a given preallocation will also be added.
** Arguments
**   s: String (Str)
**   cap: Capacity (size_t)
**   pre: Preallocation (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String(Str)
*/

_circa_ _circa_rets_
Str str_rqr_ext_(Str s, size_t cap, size_t pre, circa_msg fname,
circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return (cap > str(s)->cap) ? str_rsz_(s, cap + pre, fname, line) : s;
}

/*
** -- str_shr_ --
** Description
**   Shrinks a string's capacity to its length plus a null terminator.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_shr_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(s[str(s)->len] == '\0', fname, line);
  }
  const size_t s_len = str(s)->len;
  s = str_rsz_(s, s_len + 1, fname, line);
  return s;
}

/*
** -- str_del_ --
** Description
**   Deletes a string.
** Arguments
**   s: String (Str)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_del_(Str s, circa_msg fname, circa_msg line) {
  if (s != NULL) {
    #ifdef CIRCA_SECURE
      memset(str(s), 0, sizeof(struct str_data) + str(s)->cap);
    #endif
    free(str(s));
  }
  return NULL;
}

/*
** Evaluators
*/

/*
** -- str_eq_ --
** Description
**   Checks two strings for equality.
** Arguments
**   a: String (Str)
**   b: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Equality (bool)
*/

_circa_
bool str_eq_(Str a, Str b, circa_msg fname, circa_msg line) {
  {
    circa_assert(a != NULL, fname, line);
    circa_assert(b != NULL, fname, line);
  }
  if (a == b) return true;
  const size_t a_len = str(a)->len;
  const size_t b_len = str(b)->len;
  if (a_len != b_len) return false;
  for (size_t i = 0; i < a_len; i++)
    if (a[i] != b[i]) return false;
  return true;
}

/*
** -- str_eq_len_ --
** Description
**   Checks two strings for equality within a given length.
** Arguments
**   a: String (Str)
**   b: String (Str)
**   start: Length Start (size_t)
**   stop: Length Stop (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Equality (bool)
*/

_circa_
bool str_eq_len_(Str a, Str b, size_t start, size_t stop, circa_msg fname,
circa_msg line) {
  {
    circa_assert(a != NULL, fname, line);
    circa_assert(b != NULL, fname, line);
    circa_assert(stop >= start, fname, line);
    circa_assert(stop < str(a)->len, fname, line);
    circa_assert(stop < str(b)->len, fname, line);
  }
  if (a == b) return true;
  for (size_t i = start; i <= stop; i++)
    if (a[i] != b[i]) return false;
  return true;
}

/*
** -- str_eq_lit_ --
** Description
**   Checks if a string is equal to a C string literal.
** Arguments
**   s: String (Str)
**   cs: C String Literal (const char *const restrict)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Equality (Bool)
*/

_circa_
bool str_eq_lit_(Str s, const char *const restrict cs, circa_msg fname,
circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(cs != NULL, fname, line);
    circa_assert(s != cs, fname, line);
  }
  const size_t s_len = str(s)->len;
  const size_t cs_len = strlen(cs);
  if (s_len != cs_len) return false;
  for (size_t i = 0; i < s_len; i++)
    if (s[i] != cs[i]) return false;
  return true;
}

/*
** Stack Operations
*/

/*
** -- str_push_ --
** Description
**   Pushes a new character on top of a string. A null terminator is
**   automatically appended.
** Arguments
**   s: String (Str)
**   ch: Character (char)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_push_(Str s, char ch, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(ch != '\0', fname, line);
  }
  return str_set_(s, str(s)->len, ch, fname, line);
}

/*
** -- str_tos_ --
** Description
**   Returns the top of a string non-destructively.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Character (char)
*/

_circa_
char str_tos_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  return str_get_(s, str(s)->len - 1, fname, line);
}

/*
** -- str_pop_ --
** Description
**   Pops the top character of a string.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Character (char)
*/

_circa_
char str_pop_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  const char c = str_get_(s, --str(s)->len, fname, line);
  s[str(s)->len] = '\0';
  return c;
}

/*
** String Operations
*/

/*
** -- str_cpy_ --
** Description
**   Copies the contents of one string to another.
** Arguments
**   dst: Destination (Str)
**   src: Source (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_cpy_(Str dst, Str src, circa_msg fname, circa_msg line) {
  {
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
  }
  if (src == dst) return dst;
  const size_t src_len = str(src)->len;
  dst = str_rqr_(dst, src_len + 1, fname, line);
  memcpy(dst, src, src_len + 1);
  str(dst)->len = src_len;
  return dst;
}

/*
** -- str_cat_ --
** Description
**   Catenates the contents of one string to another.
** Arguments
**   dst: Destination (Str)
**   src: Source (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_cat_(Str dst, Str src, circa_msg fname, circa_msg line) {
  {
    circa_assert(dst != NULL, fname, line);
    circa_assert(src != NULL, fname, line);
  }
  if (dst == src) {
    dst = str_rqr_(dst, str(dst)->len * 2 + 1, fname, line);
    src = dst;
  } else {
    dst = str_rqr_(dst, str(dst)->len + str(src)->len + 1, fname, line);
  }
  memcpy((dst + str(dst)->len), src, str(src)->len + 1);
  str(dst)->len = str(dst)->len + str(src)->len;
  return dst;
}

/*
** -- str_rvs_ --
** Description
**   Reverses a string.
** Arguments
**   s: String (Str)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   void
*/

_circa_
void str_rvs_(Str s, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
  }
  const size_t len = str(s)->len;
  char tmp;
  for (size_t i = 0, j = len - 1; i < len / 2; i++, j--) {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
  }
}

/*
** IO Operations
*/

/*
** -- str_read_ --
** Description
**   Read a file pointer into a string.
** Arguments
**   s: String (Str)
**   fp: File Pointer (FILE*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback String (Str)
*/

_circa_ _circa_rets_
Str str_read_(Str s, FILE *fp, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(fp != NULL, fname, line);
  }
  fseek(fp, 0, SEEK_END); // TODO: Consider error handling this.
  size_t cap = (size_t) ftell(fp);
  rewind(fp); // TODO: Consider using fseek to error handle.
  s = str_rqr_(s, cap + 1, fname, line);
  str(s)->len = cap;
  fread(s, cap, 1, fp);
  rewind(fp); // TODO: Consider using fseek to error handle.
  s[cap] = '\0';
  return s;
}

/*
** -- str_write_ --
** Description
**   Write a string into a file pointer.
** Arguments
**   s: String (Str)
**   fp: File Pointer (FILE*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   void
*/

_circa_
void str_write_(Str s, FILE *fp, circa_msg fname, circa_msg line) {
  {
    circa_assert(s != NULL, fname, line);
    circa_assert(fp != NULL, fname, line);
  }
  fputs(s, fp); // TODO: See if there's a more optimal length based variation.
  rewind(fp);   // TODO: Consider using fseek to error handle.
}

/*
** Diagnostics Clean-Up
*/

#if defined(__GNUC__)
  #pragma GCC diagnostic pop
#endif

#endif /* CIRCA_STR_H */
