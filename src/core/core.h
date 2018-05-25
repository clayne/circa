   /////////////////////////////////////////////////////////////////////////////
  // core.h | The Circa Core Library for C | Core Header //////////////////////
 // Author: David Garland (https://github.com/davidgarland/circa) ////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef CIRCA_CORE_H
#define CIRCA_CORE_H

  //////////////////////////////////////////////////////////////////////////////
 // Dependencies //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// C99 Standard Library

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Circa Corelibs

#include "cfg.h"
#include "dbg.h"

  //////////////////////////////////////////////////////////////////////////////
 // Core Macros ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define _circa_ static inline

  //////////////////////////////////////////////////////////////////////////////
 // Type Definitions //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Dynamic Sequences

#define Seq(...) __VA_ARGS__ *
typedef Seq(void) Seq;
typedef struct Type_SeqFields {
  size_t cap;
  size_t len;
  char data[];
} *SeqFields;

static const size_t SeqSize = sizeof(struct Type_SeqFields);

// Dynamic Strings

typedef char *Str;
typedef struct Type_StrFields {
  size_t cap;
  size_t len;
  char data[];
} *StrFields;

static const size_t StrSize = sizeof(struct Type_StrFields);

  //////////////////////////////////////////////////////////////////////////////
 // Seq Function Prototypes ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Accessors

_circa_ SeqFields seq(Seq s);

// Allocators

#define  seq_new(T, C) (Seq(T)) seq_new_(sizeof(T), C)
_circa_  Seq  seq_new_(size_t siz, size_t cap);

#define  seq_rsz(T, S, C) (S) = (Seq(T)) seq_rsz_(sizeof(T), S, C)
_circa_  Seq  seq_rsz_(size_t siz, Seq s, size_t cap);

#define  seq_rqr(T, S, C) (S) = (Seq(T)) seq_rqr_(sizeof(T), S, C)
_circa_  Seq seq_rqr_(size_t siz, Seq s, size_t cap);

#define  seq_shr(T, S) (S) = (Seq(T)) seq_shr_(sizeof(T), S)
_circa_  Seq seq_shr_(size_t siz, Seq s);

#define  seq_del(T, S) seq_del_(sizeof(T), S)
_circa_  void seq_del_(size_t siz, Seq s);

// Array Ops

#define  seq_set(T, S, A, V) (S) = (Seq(T)) seq_set_(sizeof(T), S, A, &(T){V})
_circa_  Seq  seq_set_(size_t siz, Seq s, size_t addr, void *val);

#define  seq_get(T, S, A) *((Seq(T)) seq_get_(sizeof(T), S, A))
_circa_  void *seq_get_(size_t siz, Seq s, size_t addr);

// Stack Ops

#define  seq_push(T, S, V) (S) = (Seq(T)) seq_push_(sizeof(T), S, &(T){V})
_circa_  Seq  seq_push_(size_t siz, Seq s, void *val);

#define  seq_tos(T, S) *((Seq(T)) seq_tos_(sizeof(T), S))
_circa_  void *seq_tos_(size_t siz, Seq s);

#define  seq_pop(T, S) *((Seq(T)) seq_pop_(sizeof(T), S)) 
_circa_  void *seq_pop_(size_t siz, Seq s);

// Processing

#define seq_map(T, A, F, B) \
{ \
  const size_t LEN = seq(A)->len; \
  seq_rqr(T, B, LEN); \
  seq(B)->len = LEN; \
  for (int I = 0; I < LEN; I++) { \
    seq_set(T, B, I, F(seq_get(T, A, I))); \
  } \
}

#define seq_filter(T, A, F, B) \
{ \
  const size_t LEN = seq(A)->len; \
  seq_rqr(T, B, LEN); \
  seq(B)->len = 0; \
  for (int I = 0; I < LEN; I++) { \
    if (F(seq_get(T, A, I))) { \
      seq_push(T, B, seq_get(T, A, I)); \
    } \
  } \
}

#define seq_reduce(T, A, F, B) \
{ \
  const size_t LEN = seq(A)->len; \
  for (int I = 0; I < LEN; I++) { \
    B = F(B, seq_get(T, A, I)); \
  } \
}

  //////////////////////////////////////////////////////////////////////////////
 // Str Function Prototypes ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Accessors

_circa_ StrFields str(Str s);

// Allocators

#define str_new(C) str_new_(C)
_circa_ Str str_new_(size_t cap);

#define str_rsz(S, C) (S) = str_rsz_(S, C)
_circa_ Str str_rsz_(Str s, size_t cap);

#define str_rqr(S, C) (S) = str_rqr_(S, C)
_circa_ Str str_rqr_(Str s, size_t cap);

#define str_shr(S) (S) = str_shr_(S)
_circa_ Str str_shr_(Str s);

#define str_del(S) str_del_(S)
_circa_ void str_del_(Str s);

// Array Ops

#define str_set(S, A, V) (S) = str_set_(S, A, V)
_circa_ Str str_set_(Str s, size_t addr, char val);

#define str_get(S, A) str_get_(S, A)
_circa_ char str_get_(Str s, size_t addr);

// Processing

#define str_map(A, F, B) \
{ \
  const size_t LEN = str(A)->len; \
  str_rqr(B, LEN); \
  str(B)->len = LEN; \
  for (int I = 0; I < LEN; I++) { \
    str_set(B, I, F(str_get(A, I))); \
  } \
}

#define str_filter(A, F, B) \
{ \
  const size_t LEN = str(A)->len; \
  seq_rqr(T, B, LEN); \
  seq(B)->len = 0; \
  for (int I = 0; I < LEN; I++) { \
    if (F(str_get(A, I))) { \
      str_push(B, seq_get(A, I)); \
    } \
  } \
}

#define str_reduce

#endif // CIRCA_CORE_H
