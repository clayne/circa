   //                                                  /////////////////////////
  // seq.h | Circa's dynamically allocated sequences. /////////////////////////
 // https://github.com/davidgarland/circa            /////////////////////////
//                                                  /////////////////////////

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

  //              //////////////////////////////////////////////////////////////
 // Dependencies //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

#include "../circa.h"

  //           /////////////////////////////////////////////////////////////////
 // Accessors /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// seq ///
// Description
//   Returns a pointer to the structure attached to a sequence.
// Arguments
//   s: Sequence (Seq)
// Returns
//   Sequence Structure (struct SeqData*)

_circa_
struct SeqData *seq(Seq s) {
  {
    circa_assert(s != NULL);
  }
  return ((struct SeqData*) s) - 1;
}

  //            ////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//            ////////////////////////////////////////////////////////////////

/// seq_new ///
// Description
//   Returns a new sequence of a given capacity.
// Arguments
//   T: Type (type)
//   cap: Capacity (size_t)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_new_(size_t siz, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(cap > 0);
  }
  struct SeqData *s = NULL;
  while (s == NULL) s = malloc(sizeof(*s) + (cap * siz));
  s->cap = cap;
  s->len = 0;
  return s->data;
}

/// seq_lit ///
// Description
//   Creates a new sequence from a literal.
// Arguments
//   T: Type (type)
//   len: Length (size_t)
//   lits: Literals (T)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_lit_(size_t siz, size_t len, void *lits) {
  {
    circa_assert(siz > 0);
    circa_assert(len > 0);
    circa_assert(lits != NULL);
  }
  Seq s = seq_new_(siz, len);
  memcpy(s, lits, (siz * len));
  seq(s)->len = len;
  return s;
}

/// seq_from ///
// Description
//   Creates a new sequence from another.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_from_(size_t siz, Seq s){
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
  }
  const size_t s_len = seq(s)->len;
  Seq s2 = seq_new_(siz, s_len);
  seq(s2)->len = s_len;
  memcpy(s2, s, (siz * s_len));
  return s2;
}

/// seq_wrap ///
// Description
//   Creates a new sequence from a C array.
// Arguments
//   T: Type (type)
//   ptr: Pointer (void*)
//   len: Length (size_t)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_wrap_(size_t siz, void *ptr, size_t len) {
  {
    circa_assert(siz > 0);
    circa_assert(ptr != NULL);
    circa_assert(len > 0);
  }
  Seq s = seq_new_(siz, len);
  memcpy(s, ptr, (siz * len));
  seq(s)->len = len;
  return s;
}

/// seq_rsz ///
// Description
//   Resizes a sequence to a given capacity.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   cap: Capacity (size_t)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_rsz_(size_t siz, Seq s, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  #ifdef CIRCA_SECURE
    // Zero out freed memory, if any.
    if (cap < seq(s)->cap) {
      memset(((char*) s) + cap, 0, ((seq(s)->cap - cap) * siz));
    }
  #endif
  struct SeqData *s2 = NULL;
  while (s2 == NULL) s2 = realloc(seq(s), sizeof(*s2) + (cap * siz));
  s2->cap = cap;
  return s2->data;
}

/// seq_rqr ///
// Description
//   Requires that a sequence be able to contain at least a given capacity.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   cap: Capacity (size_t)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_rqr_(size_t siz, Seq s, size_t cap) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(cap > 0);
  }
  return (cap > seq(s)->cap) ? seq_rsz_(siz, s, cap + CIRCA_ALLOC) : s;
}

/// seq_shr ///
// Description
//   Shrinks a sequence's capacity to its length.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_shr_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
  }
  return (seq(s)->len != seq(s)->cap) ? seq_rsz_(siz, s, seq(s)->len) : s;
}

/// seq_del ///
// Description
//   Deletes a sequence.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
// Returns
//   void

_circa_
Seq seq_del_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
  }
  if (s != NULL) {
    #ifdef CIRCA_SECURE
      // Zero out the memory to be freed.
      memset(seq(s), 0, siz * seq(s)->cap);
    #endif
    free(seq(s));
  }
  return NULL;
}

  //              //////////////////////////////////////////////////////////////
 // Sequence Ops //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

/// seq_cpy ///
// Description
//   Copy the contents of one sequence to another.
// Arguments
//   T: Type (type)
//   dst: Destination Sequence (Seq)
//   src: Source Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_cpy_(size_t siz, Seq dst, Seq src) {
  {
    circa_assert(siz > 0);
    circa_assert(dst != NULL);
    circa_assert(src != NULL);
  }
  const size_t src_len = seq(src)->len;
  dst = seq_rqr_(siz, dst, src_len);
  memcpy(dst, src, src_len);
  seq(dst)->len = src_len;
  return dst;
}

/// seq_cat ///
// Description
//   Catenate the contents of one sequence to another.
// Arguments
//   T: Type (type)
//   dst: Destination Sequence (Seq)
//   src: Source Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_cat_(size_t siz, Seq dst, Seq src) {
  {
    circa_assert(siz > 0);
    circa_assert(dst != NULL);
    circa_assert(src != NULL);
  }
  dst = seq_rqr_(siz, dst, seq(dst)->len + seq(src)->len);
  memcpy((((char*) dst) + (siz * seq(dst)->len)), src, siz * seq(src)->len);
  seq(dst)->len = seq(dst)->len + seq(src)->len;
  return dst;
}

/// seq_rvs ///
// Description
//   Reverses the contents of a sequence.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_rvs_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
  }
  const size_t len = seq(s)->len;
  s = seq_rqr_(siz, s, len + 1);
  void *const restrict tmp = (((char*) s) + (siz * len));
  for (size_t i = 0, j = len - 1; i < len / 2; i++, j--) {
    memmove(tmp, seq_get_(siz, s, i), siz);
    memmove(seq_get_(siz, s, i), seq_get_(siz, s, j), siz);
    memmove(seq_get_(siz, s, j), tmp, siz);
  }
  return s;
}

  //           /////////////////////////////////////////////////////////////////
 // Array Ops /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// seq_set ///
// Description
//   Sets a sequence's value at a given address.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   addr: Address (size_t)
//   val: Value (T)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_set_(size_t siz, Seq s, size_t addr, void *val) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(val != NULL);
  }
  s = seq_rqr_(siz, s, addr + 1);
  if (addr >= str(s)->len) str(s)->len = addr + 1;
  memcpy(((char*) s) + (siz * addr), val, siz);
  return s;
}

/// seq_get ///
// Description
//   Gets a sequence's value at a given address.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   addr: Address (size_t)
// Returns
//   Value (T)

_circa_
void *seq_get_(size_t siz, Seq s, size_t addr) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(seq(s)->len > 0);
    circa_assert(addr < seq(s)->len);
  }
  return (void*) ((char*) s) + (siz * addr);
}

  //           /////////////////////////////////////////////////////////////////
 // Stack Ops /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// seq_push ///
// Description
//   Pushes a new value on top of a sequence.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   val: Value (T)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_push_(size_t siz, Seq s, void *val) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(val != NULL);
  }
  return seq_set_(siz, s, seq(s)->len, val);
}

/// seq_push_ext ///
// Description
//   Pushes a new value on top of a sequence
//   and pre-allocates some extra memory.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
//   val: Value (T)
//   pre: Prealloc Size (size_t)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_push_ext_(size_t siz, Seq s, void *val, size_t pre) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(val != NULL);
  }
  // TODO: Optimize to avoid an extra call to seq_rqr.
  s = seq_rqr_(siz, s, seq(s)->len + pre);
  s = seq_push_(siz, s, val);
  return s;
}

/// seq_tos ///
// Description
//   Returns the top of a sequence non-destructively.
// Arguments
//   T: Type (type)
//   s: Sequence (Seq)
// Returns
//   Value (T)

_circa_
void *seq_tos_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(seq(s)->len > 0);
  }
  return seq_get_(siz, s, seq(s)->len - 1);
}

/// seq_pop ///
// Description
//   Pops the top value of a sequence.
// Arguments
//   T: Type (T)
//   s: Sequence (Seq)
// Returns
//   Value (T)

_circa_
void *seq_pop_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
    circa_assert(seq(s)->len > 0);
  }
  void *result = seq_get_(siz, s, seq(s)->len - 1);
  seq(s)->len -= 1;
  return result;
}

/// seq_dup ///
// Description
//   Duplicates the top few segments of a sequence.
// Arguments
//   T: Type (T)
//   s: Sequence (Seq)
// Returns
//   Sequence (Seq)

_circa_
Seq seq_dup_(size_t siz, Seq s) {
  {
    circa_assert(siz > 0);
    circa_assert(s != NULL);
  }
  return seq_push_(siz, s, seq_tos_(siz, s));
}

#endif // CIRCA_SEQ_H
