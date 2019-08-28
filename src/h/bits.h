/*
** bits.h | The Circa Library Set | Integer operations.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_BITS_H
#define CIRCA_BITS_H

/*
** Dependencies
*/

/* Standard */

#include <stdint.h>
#include <stdlib.h>

/* Circa */

#include "config.h"
#include "debug.h"

/*
** Macros
*/

// This macro helps us to automatically select builtins for GNU C
// code; given by @craigbarnes.

#ifdef CIRCA_GNU
  #define USE_BUILTIN(FN, ARG)                                                 \
    if (__builtin_types_compatible_p(__typeof__(ARG), unsigned long long)) {   \
      return (uint8_t) __builtin_ ## FN ## ll(ARG);                            \
    } else if (__builtin_types_compatible_p(__typeof__(ARG), unsigned long)) { \
      return (uint8_t) __builtin_ ## FN ## l(ARG);                             \
    } else if (__builtin_types_compatible_p(__typeof__(ARG), unsigned int)) {  \
      return (uint8_t) __builtin_ ## FN(ARG);                                  \
    }
#endif

/*
** Global Declarations
*/

// To put this as simply as possible: Generating prime numbers every time we want
// a new map capacity seems annoying, so instead there's a table.

CIRCA_EXTERN uint32_t circa_primes[24];

/*
** Function Declarations
*/

/* Minimum */

static inline uint8_t   u8_min(uint8_t a,  uint8_t  b);
static inline uint16_t u16_min(uint16_t a, uint16_t b);
static inline uint32_t u32_min(uint32_t a, uint32_t b);
static inline uint64_t u64_min(uint64_t a, uint64_t b);
static inline   size_t usz_min(size_t   a, size_t   b);

/* Maximum */

static inline uint8_t   u8_max(uint8_t a,  uint8_t  b);
static inline uint16_t u16_max(uint16_t a, uint16_t b);
static inline uint32_t u32_max(uint32_t a, uint32_t b);
static inline uint64_t u64_max(uint64_t a, uint64_t b);
static inline   size_t usz_max(size_t   a, size_t   b);

/* Popcount */

static inline uint8_t  u8_pop(uint8_t  n);
static inline uint8_t u16_pop(uint16_t n);
static inline uint8_t u32_pop(uint32_t n);
static inline uint8_t u64_pop(uint64_t n);
static inline  size_t usz_pop(size_t   n);

/* Count Leading Zeroes */

static inline uint8_t  u8_clz(uint8_t  n);
static inline uint8_t u16_clz(uint16_t n);
static inline uint8_t u32_clz(uint32_t n);
static inline uint8_t u64_clz(uint64_t n);
static inline  size_t usz_clz(size_t   n);

/* Count Trailing Zeroes */

static inline uint8_t  u8_ctz(uint8_t n);
static inline uint8_t u16_ctz(uint16_t n);
static inline uint8_t u32_ctz(uint32_t n);
static inline uint8_t u64_ctz(uint64_t n);
static inline size_t  usz_clz(size_t   n);

/* Twos Logarithm */

static inline uint8_t  u8_log2(uint8_t  n);
static inline uint8_t u16_log2(uint16_t n);
static inline uint8_t u32_log2(uint32_t n);
static inline uint8_t u64_log2(uint64_t n);
static inline  size_t usz_log2(size_t   n);

/* Next Power of Two */

static inline uint16_t  u8_np2(uint8_t  n);
static inline uint32_t u16_np2(uint16_t n);
static inline uint64_t u32_np2(uint32_t n);
static inline uint64_t u64_np2(uint64_t n);
static inline  size_t  usz_np2(size_t   n);

/* Prime Greater Than */

static inline uint32_t  u8_primegt(uint8_t  n);
static inline uint32_t u16_primegt(uint16_t n);
static inline uint32_t u32_primegt(uint32_t n);
static inline uint32_t u64_primegt(uint64_t n);
static inline  size_t  usz_primegt(size_t   n);

/*
** Function Definitions
*/

/* Minimum */

static inline
uint8_t u8_min(uint8_t a, uint8_t b) {
  return a < b ? a : b;
}

static inline
uint16_t u16_min(uint16_t a, uint16_t b) {
  return a < b ? a : b;
}

static inline
uint32_t u32_min(uint32_t a, uint32_t b) {
  return a < b ? a : b;
}

static inline
uint64_t u64_min(uint64_t a, uint64_t b) {
  return a < b ? a : b;
}

static inline
size_t usz_min(size_t a, size_t b) {
  return a < b ? a : b;
}

/* Maximum */

static inline
uint8_t u8_max(uint8_t a, uint8_t b) {
  return a > b ? a : b;
}

static inline
uint16_t u16_max(uint16_t a, uint16_t b) {
  return a > b ? a : b;
}

static inline
uint32_t u32_max(uint32_t a, uint32_t b) {
  return a > b ? a : b;
}

static inline
uint64_t u64_max(uint64_t a, uint64_t b) {
  return a > b ? a : b;
}

static inline
size_t usz_max(size_t a, size_t b) {
  return a > b ? a : b;
}

/* Popcount */

static inline
uint8_t u8_pop(uint8_t n) {
  return u16_pop(n);
}

static inline
uint8_t u16_pop(uint16_t n) {
  #ifdef CIRCA_GNU
    return u32_pop(n);
  #else
    n -= ((n >> 1) & 0x5555);
    n = (n & 0x3333) + ((n >> 2) & 0x3333);
    return (((n + (n >> 4)) & 0x0F0F) * 0x0101) >> 8;
  #endif
}

static inline
uint8_t u32_pop(uint32_t n) {
  #ifdef __GNUC__
    USE_BUILTIN(popcount, n)
  #else
    n -= ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
  #endif
}

static inline
uint8_t u64_pop(uint64_t n) {
  #ifdef __GNUC__
    USE_BUILTIN(popcount, n)
  #else
    n -= ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (n * 0x0101010101010101ULL) >> 56;
  #endif
}

static inline
size_t usz_pop(size_t n) {
  return (sizeof(size_t) == 8) ? u64_pop(n) : u32_pop(n);
}

/* Count Leading Zeroes */

static inline
uint8_t u8_clz(uint8_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 8);
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n) - (64 - 8);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    return u8_pop(~n);
  #endif
}

static inline
uint8_t u16_clz(uint16_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 16);
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n) - (64 - 16);
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    return u16_pop(~n);
  #endif
}

static inline
uint8_t u32_clz(uint32_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 32);
  #ifdef __GNUC__
    USE_BUILTIN(clz, n)
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return u32_pop(~n);
  #endif
}

static inline
uint8_t u64_clz(uint64_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 64);
  #ifdef __GNUC__
    USE_BUILTIN(clz, n)
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n |= (n >> 32);
    return u64_pop(~n);
  #endif
}

static inline
size_t usz_clz(size_t n) {
  return (sizeof(size_t) == 8) ? u64_clz(n) : u32_clz(n);
}

/* Count Trailing Zeroes */

static inline
uint8_t u8_ctz(uint8_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 8);
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    uint8_t c = 8;
    n &= -((int8_t) n);
    c -= (n) ? 1 : 0;
    c -= (n & 0x0F) ? 4 : 0;
    c -= (n & 0x33) ? 2 : 0;
    c -= (n & 0x55) ? 1 : 0;
    return c;
  #endif
}

static inline
uint8_t u16_ctz(uint16_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 16);
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    uint8_t c = 16;
    n &= -((int16_t) n);
    c -= (n) ? 1 : 0;
    c -= (n & 0x00FF) ? 8 : 0;
    c -= (n & 0x0F0F) ? 4 : 0;
    c -= (n & 0x3333) ? 2 : 0;
    c -= (n & 0x5555) ? 1 : 0;
    return c;
  #endif
}

static inline
uint8_t u32_ctz(uint32_t n) {
  circa_guard (!n)
    return (circa_throw(CE_ARG), 32);
  #ifdef __GNUC__
    USE_BUILTIN(ctz, n)
  #else
    uint8_t c = 32;
    n &= -((int32_t) n);
    c -= (n) ? 1 : 0;
    c -= (n & 0x0000FFFF) ? 16 : 0;
    c -= (n & 0x00FF00FF) ? 8  : 0;
    c -= (n & 0x0F0F0F0F) ? 4  : 0;
    c -= (n & 0x33333333) ? 2  : 0;
    c -= (n & 0x55555555) ? 1  : 0;
    return c;
  #endif
}

static inline
uint8_t u64_ctz(uint64_t n) {
  circa_guard(!n)
    return (circa_throw(CE_ARG), 64);
  #ifdef __GNUC__
    USE_BUILTIN(ctz, n)
  #else
    uint8_t c = 64;
    n &= -((int64_t) n);
    c -= (n) ? 1 : 0;
    c -= (n & 0x00000000FFFFFFFF) ? 32 : 0;
    c -= (n & 0x0000FFFF0000FFFF) ? 16 : 0;
    c -= (n & 0x00FF00FF00FF00FF) ? 8  : 0;
    c -= (n & 0x0F0F0F0F0F0F0F0F) ? 4  : 0;
    c -= (n & 0x3333333333333333) ? 2  : 0;
    c -= (n & 0x5555555555555555) ? 1  : 0;
    return c;
  #endif
}

static inline
size_t usz_ctz(size_t n) {
  return (sizeof(size_t) == 8) ? u64_ctz(n) : u32_ctz(n);
}

/* Twos Logarithm */

static inline
uint8_t u8_log2(uint8_t n) {
  return n ? 8 * sizeof(uint8_t) - u8_clz(n) - 1 : 0;
}

static inline
uint8_t u16_log2(uint16_t n) {
  return n ? 8 * sizeof(uint16_t) - u16_clz(n) - 1 : 0;
}

static inline
uint8_t u32_log2(uint32_t n) {
  return n ? 8 * sizeof(uint32_t) - u32_clz(n) - 1 : 0;
}

static inline
uint8_t u64_log2(uint64_t n) {
  return n ? 8 * sizeof(uint64_t) - u64_clz(n) - 1 : 0;
}

static inline
size_t usz_log2(size_t n) {
  return (sizeof(size_t) == 8) ? u64_log2(n) : u32_log2(n);
}

/* Next Power of Two */

static inline
uint16_t u8_np2(uint8_t n) {
  return (uint16_t) (1 << (u8_log2(n - (n ? 1 : 0)) + 1));
}

static inline
uint32_t u16_np2(uint16_t n) {
  return (uint32_t) (1 << (u16_log2(n - (n ? 1 : 0)) + 1));
}

static inline
uint64_t u32_np2(uint32_t n) {
  return (uint64_t) (1 << (u32_log2(n - (n ? 1 : 0)) + 1));
}

static inline
uint64_t u64_np2(uint64_t n) {
  return (uint64_t) (1 << (u64_log2(n - (n ? 1 : 0)) + 1));
}

static inline
size_t usz_np2(size_t n) {
  return (sizeof(size_t) == 8) ? u64_np2(n) : u32_np2(n);
}

/* Prime Greater Than */

static inline
uint32_t u8_primegt(uint8_t n) {
  return u32_primegt(n);
}

static inline
uint32_t u16_primegt(uint16_t n) {
  return u32_primegt(n);
}

static inline
uint32_t u32_primegt(uint32_t n) {
  return (uint32_t) circa_primes[u64_ctz(u64_np2(n))];
}

static inline
uint32_t u64_primegt(uint64_t n) {
  return (uint32_t) circa_primes[u64_ctz(u64_np2(n))];
}

static inline
size_t usz_primegt(size_t n) {
  return (sizeof(size_t) == 8) ? u64_primegt(n) : u32_primegt(n);
}

#ifdef CIRCA_HEADER_ONLY
  #include "../c/bits.c"
#endif

#endif // CIRCA_BITS_H
