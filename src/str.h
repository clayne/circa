   //                                                  /////////////////////////
  // str.h | Circa's dynamically allocated strings.   /////////////////////////
 // https://github.com/davidgarland/circa            /////////////////////////
//                                                  /////////////////////////

#ifndef CIRCA_STR_H
#define CIRCA_STR_H

  //              //////////////////////////////////////////////////////////////
 // Dependencies //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

#include "../circa.h"

  //           /////////////////////////////////////////////////////////////////
 // Accessors /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// str ///
// Description
//   Returns a pointer to the structure attached to a string.
// Arguments
//   s: String (Str)
// Returns
//   Sequence Structure (struct StrData*)

_circa_
struct StrData *str(Str s) {
  {
    circa_assert(s != NULL);
  }
  return ((struct StrData*) s) - 1;
}

  //            ////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//            ////////////////////////////////////////////////////////////////

/// str_new ///
// Description
//   Returns a new string of a given capacity.
// Arguments
//   cap: Capacity (size_t)
// Returns
//   String (Str)

_circa_
Str str_new_(size_t cap) {
  {
    circa_assert(cap > 0);
  }
  struct StrData *s = NULL;
  while (s == NULL) s = malloc(sizeof(*s) + cap + 1);
  s->cap = cap;
  s->len = 0;
  return s->data;
}

/// str_lit ///
// Description
//   Creates a new string from a literal.
// Arguments
//   cs: C String (const char *restrict)
// Returns
//   String (Str)

_circa_
Str str_lit_(const char *restrict cs) {
  {
    circa_assert(cs != NULL);
  }
  const size_t cs_len = strlen(cs);
  Str s = str_new_(cs_len + 1);
  str(s)->len = cs_len;
  memcpy(s, cs, cs_len + 1);
  return s;
}


/// str_from ///
// Description
//   Creates a new string from another.
// Arguments
//   s: String (Str)
// Returns
//   String (Str)

_circa_
Str str_from_(Str s) {
  {
    circa_assert(s != NULL);
  }
  const size_t s_len = str(s)->len;
  Str s2 = str_new_(s_len + 1);
  str(s2)->len = s_len;
  memcpy(s2, s, s_len + 1);
  return s2;
}

/// str_rsz ///
// Description
//   Resizes a string to a given capacity.
// Arguments
//   s: String (Str)
//   cap: Capacity (size_t)
// Returns
//   String (Str)

_circa_
Str str_rsz_(Str s, size_t cap) {
  {
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  #ifdef CIRCA_SECURE
    // Zero out freed memory, if any.
    if (cap < str(s)->cap) {
      memset(s + cap, 0, (str(s)->cap - cap));
    }
  #endif
  struct StrData *s2 = NULL;
  while (s2 == NULL) s2 = realloc(str(s), sizeof(*s2) + cap + 1);
  s2->cap = cap;
  s2->data[cap] = '\0';
  return s2->data;
}

/// str_rqr ///
// Description
//   Requires that a string be able to contain at least a given capacity.
// Arguments
//   s: String (Str)
//   cap: Capacity (size_t)
// Returns
//   String (Str)

_circa_
Str str_rqr_(Str s, size_t cap) {
  {
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  return (cap > str(s)->cap) ? str_rsz(s, cap + CIRCA_ALLOC) : s;
}

/// str_shr ///
// Description
//   Shrinks a string's capacity to its length (plus a null terminator.)
// Arguments
//   s: String (Str)
// Returns
//   String (Str)

_circa_
Str str_shr_(Str s) {
  {
    circa_assert(s != NULL);
  }
  return str_rsz(s, str(s)->len + 1);
}

/// str_del ///
// Description
//   Deletes a string.
// Arguments
//   s: String (Str)
// Returns
//   Null String (Str)

_circa_
Str str_del_(Str s) {
  if (s != NULL) {
    #ifdef CIRCA_SECURE
      // Zero out the memory to be freed.
      memset(str(s), 0, str(s)->cap);
    #endif
    free(seq(s));
  }
  return NULL;
}

  //            ////////////////////////////////////////////////////////////////
 // String Ops ////////////////////////////////////////////////////////////////
//            ////////////////////////////////////////////////////////////////

/// str_cpy ///
// Description
//   Copy the contents of one string to another.
// Arguments
//   dst: String (Str)
//   src: String (Str)
// Returns
//   String (Str)

_circa_
Str str_cpy_(Str dst, Str src) {
  {
    circa_assert(dst != NULL);
    circa_assert(src != NULL);
  }
  const size_t src_len = str(src)->len;
  dst = str_rqr_(dst, src_len + 1);
  memcpy(dst, src, src_len + 1);
  str(dst)->len = src_len;
  return dst;
}

/// str_cat ///
// Description
//   Catenate the characters of one string to another.
// Arguments
//   dst: Destination String (Str)
//   src: Source String (Str)
// Returns
//   String (Str)

_circa_
Str str_cat_(Str dst, Str src) {
  {
    circa_assert(dst != NULL);
    circa_assert(src != NULL);
  }
  dst = str_rqr_(dst, str(dst)->len + str(src)->len);
  memcpy((dst + str(dst)->len), src, str(src)->len);
  str(dst)->len = str(dst)->len + str(src)->len;
  return dst;
}

/// str_rvs ///
// Description
//   Reverses the characters in a string.
// Arguments
//   s: String (Str)
// Returns
//   String (Str)

_circa_
Str str_rvs_(Str s) {
  {
    circa_assert(s != NULL);
  }
  const size_t len = str(s)->len;
  char tmp;
  for (size_t i = 0, j = len - 1; i < len / 2; i++, j--) {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
  }
  return s;
}

  //                ////////////////////////////////////////////////////////////
 // Evaluation Ops ////////////////////////////////////////////////////////////
//                ////////////////////////////////////////////////////////////

/// str_eq ///
// Description
//   Checks two strings for equality.
// Arguments
//   a: String (Str)
//   b: String (Str)
// Returns
//   Equality (bool)

_circa_
bool str_eq_(Str a, Str b) {
  {
    circa_assert(a != NULL);
    circa_assert(b != NULL);
  }
  const size_t a_len = str(a)->len;
  const size_t b_len = str(b)->len;
  if (a_len != b_len) return false;
  for (size_t i = 0; i < a_len; i++) if (a[i] != b[i]) return false;
  return true;
}

/// str_eq_len ///
// Description
//   Checks two strings for equality in a given length.
// Arguments
//   a: String (Str)
//   b: String (Str)
//   len: Length (size_t)
// Returns
//   Equality (bool)

_circa_
bool str_eq_len_(Str a, Str b, size_t len) {
  {
    circa_assert(a != NULL);
    circa_assert(b != NULL);
    circa_assert(len > 0);
    circa_assert(len < str(a)->len);
    circa_assert(len < str(b)->len);
  }
  for (size_t i = 0; i < len; i++) if (a[i] != b[i]) return false;
  return true;
}

  //           /////////////////////////////////////////////////////////////////
 // Array Ops /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// str_set ///
// Description
//   Sets a string's character at a given address.
// Arguments
//   s: String (Str)
//   addr: Address (size_t)
//   ch: Character (char)
// Returns
//   String (Str)

_circa_
Str str_set_(Str s, size_t addr, char ch) {
  {
    circa_assert(s != NULL);
  }
  s = str_rqr_(s, addr + 1);
  if (addr >= seq(s)->len) seq(s)->len = addr + 1;
  s[addr] = ch;
  return s;
}

/// str_get ///
// Description
//   Gets a string's character at a given address.
// Arguments
//   s: String (Str)
//   addr: Address (size_t)
// Returns
//   Character (char)

_circa_
char str_get_(Str s, size_t addr) {
  {
    circa_assert(s != NULL);
    circa_assert(str(s)->len > 0);
    circa_assert(addr < str(s)->len);
  }
  return s[addr];
}

  //           /////////////////////////////////////////////////////////////////
 // Stack Ops /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// str_push ///
// Description
//   Pushes a new character on top of a string.
// Arguments
//   s: String (Str)
//   ch: Character (char)
// Returns
//   String (Str)

_circa_
Str str_push_(Str s, char ch) {
  {
    circa_assert(s != NULL);
  }
  return str_set_(s, str(s)->len, ch);
}

/// str_tos ///
// Description
//   Returns the top of a string non-destructively.
// Arguments
//   s: String (Str)
// Returns
//   Character (char)

_circa_
char str_tos_(Str s) {
  {
    circa_assert(s != NULL);
    circa_assert(str(s)->len > 0);
  }
  return str_get_(s, str(s)->len - 1);
}

/// str_pop ///
// Description
//   Pops the top value of a string.
// Arguments
//   s: String (Str)
// Returns
//   Character (char)

_circa_
char str_pop_(Str s) {
  {
    circa_assert(s != NULL);
    circa_assert(str(s)->len > 0);
  }
  return str_get_(s, --str(s)->len);
}

  //              //////////////////////////////////////////////////////////////
 // IO Functions //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

/// str_readfile ///
// Description
//   Creates a new string from a file.
// Arguments
//   s: String (Str)
//   filename: File Name (char*)
// Returns
//   String (Str)

_circa_
Str str_readfile_(char *filename) {
  {
    circa_assert(filename != NULL);
  }
  FILE *fp = fopen(filename, "r");
  circa_assert(fp != NULL);
  fseek(fp, 0, SEEK_END);
  size_t cap = ftell(fp);
  rewind(fp);
  Str s = str_new(cap + 1);
  str(s)->len = cap;
  fread(s, cap, 1, fp);
  fclose(fp);
  s[cap] = '\0';
  return s;
}

#endif // CIRCA_STR_H
