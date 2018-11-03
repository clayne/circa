/*
** bits.h | The Circa Library Set | Bit-Twiddling
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_BITS_H
#define CIRCA_BITS_H

/*
** Dependencies
*/

/* Internal */

#include "core.h"

/*
** Constants
*/

/* Primes */

// These are a set of primes closest to 2^N where N is the index of the array.
// The corresponding non-prime numbers are given to the right for comparison.
// This is used for the "primegt" function, which in turn is for Dict(T).

static const uint64_t circa_primes[] = {
  2,       2,       5,       11,     // 1,       2,       4,       8
  17,      37,      67,      131,    // 16,      32,      64,      128,
  257,     521,     1031,    2053,   // 256,     512,     1024,    2048
  4099,    8209,    16411,   32771,  // 4096,    8192,    16384,   32768,
  65537,   131101,  262147,  524309, // 65536,   131072,  262144,  524288
  1048583, 2097169, 4194319, 8388617 // 1048576, 2097152, 4194304, 8388608
};

/*
** Prototypes
*/

/* Binary Popcount */

_circa_static_ _circa_const_ uint8_t  u8_pop(uint8_t  n);
_circa_static_ _circa_const_ uint8_t u16_pop(uint16_t n);
_circa_static_ _circa_const_ uint8_t u32_pop(uint32_t n);
_circa_static_ _circa_const_ uint8_t u64_pop(uint64_t n);
_circa_static_ _circa_const_ uint8_t usz_pop(size_t   n);

/* Binary Count Leading Zeros */

_circa_static_ _circa_const_ uint8_t  u8_clz(uint8_t  n);
_circa_static_ _circa_const_ uint8_t u16_clz(uint16_t n);
_circa_static_ _circa_const_ uint8_t u32_clz(uint32_t n);
_circa_static_ _circa_const_ uint8_t u64_clz(uint64_t n);
_circa_static_ _circa_const_ uint8_t usz_clz(size_t   n);

/* Binary Count Trailing Zeros */

_circa_static_ _circa_const_ uint8_t  u8_ctz(uint8_t  n);
_circa_static_ _circa_const_ uint8_t u16_ctz(uint16_t n);
_circa_static_ _circa_const_ uint8_t u32_ctz(uint32_t n);
_circa_static_ _circa_const_ uint8_t u64_ctz(uint64_t n);
_circa_static_ _circa_const_ uint8_t usz_ctz(size_t   n);

/* Binary Logarithm */

_circa_static_ _circa_const_ uint8_t  u8_log2(uint8_t  n);
_circa_static_ _circa_const_ uint8_t u16_log2(uint16_t n);
_circa_static_ _circa_const_ uint8_t u32_log2(uint32_t n);
_circa_static_ _circa_const_ uint8_t u64_log2(uint64_t n);
_circa_static_ _circa_const_ uint8_t usz_log2(size_t   n);

/* Next Power of Two */

_circa_static_ _circa_const_ uint16_t  u8_np2(uint8_t  n);
_circa_static_ _circa_const_ uint32_t u16_np2(uint16_t n);
_circa_static_ _circa_const_ uint64_t u32_np2(uint32_t n);
_circa_static_ _circa_const_ uint64_t u64_np2(uint64_t n);
_circa_static_ _circa_const_ size_t   usz_np2(size_t   n);

/* Divide By Ten */

_circa_static_ _circa_const_ uint8_t   u8_div10(uint8_t  n);
_circa_static_ _circa_const_ uint16_t u16_div10(uint16_t n);
_circa_static_ _circa_const_ uint32_t u32_div10(uint32_t n);
_circa_static_ _circa_const_ uint64_t u64_div10(uint64_t n);
_circa_static_ _circa_const_ size_t   usz_div10(size_t   n);

/* Round Up To The Nearest Ten */

_circa_static_ _circa_const_ uint16_t  u8_ceil10(uint8_t  n);
_circa_static_ _circa_const_ uint32_t u16_ceil10(uint16_t n);
_circa_static_ _circa_const_ uint64_t u32_ceil10(uint32_t n);
_circa_static_ _circa_const_ uint64_t u64_ceil10(uint64_t n);
_circa_static_ _circa_const_ size_t   usz_ceil10(size_t   n);

/* Prime Greater Than Input */

_circa_static_ _circa_pure_ uint16_t  u8_primegt(uint8_t  n);
_circa_static_ _circa_pure_ uint32_t u16_primegt(uint16_t n);
_circa_static_ _circa_pure_ uint64_t u32_primegt(uint32_t n);
_circa_static_ _circa_pure_ uint64_t u64_primegt(uint64_t n);
_circa_static_ _circa_pure_ size_t   usz_primegt(size_t   n);

/*
** Binary Popcount
*/

_circa_static_ _circa_const_
uint8_t u8_pop(uint8_t n) {
  return u32_pop(n);
}

_circa_static_ _circa_const_
uint8_t u16_pop(uint16_t n) {
  return u32_pop(n);
}

_circa_static_ _circa_const_
uint8_t u32_pop(uint32_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_popcount(n);
  #else
    n -= ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
  #endif
}

_circa_static_ _circa_const_
uint8_t u64_pop(uint64_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_popcountll(n);
  #else
    n -= ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (n * 0x0101010101010101ULL) >> 56;
  #endif
}

_circa_static_ _circa_const_
uint8_t usz_pop(size_t n) {
  return (sizeof(size_t) == 8) ? u64_pop(n) : u32_pop(n);
}

/*
** Binary Count Leading Zeros
*/

_circa_static_ _circa_const_
uint8_t u8_clz(uint8_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_clzll(n) - (64 - 8);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    return u8_pop(n);
  #endif
}

_circa_static_ _circa_const_
uint8_t u16_clz(uint16_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_clzll(n) - (64 - 16);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    return u16_pop(n);
  #endif
}

_circa_static_ _circa_const_
uint8_t u32_clz(uint32_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_clzll(n) - (64 - 32);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return u32_pop(n);
  #endif
}

_circa_static_ _circa_const_
uint8_t u64_clz(uint64_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_clzll(n);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    n |= (n >> 32);
    return u64_pop(n);
  #endif
}

_circa_static_ _circa_const_
uint8_t usz_clz(size_t n) {
  return (sizeof(size_t) == 8) ? u64_clz(n) : u32_clz(n);
}

/*
** Binary Count Trailing Zeros
*/

_circa_static_ _circa_const_
uint8_t u8_ctz(uint8_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n++;
    return (uint8_t) n;
  #endif
}

_circa_static_ _circa_const_
uint8_t u16_ctz(uint16_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n++;
    return (uint8_t) n;
  #endif
}

_circa_static_ _circa_const_
uint8_t u32_ctz(uint32_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    n++;
    return (uint8_t) n;
  #endif
}

_circa_static_ _circa_const_
uint8_t u64_ctz(uint64_t n) {
  #ifdef CIRCA_GNU
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    n |= (n >> 32);
    n++;
    return (uint8_t) n;
  #endif
}

_circa_static_ _circa_const_
uint8_t usz_ctz(size_t n) {
  return (sizeof(size_t) == 8) ? u64_ctz(n) : u32_ctz(n);
}

/*
** Binary Logarithm
*/

_circa_static_ _circa_const_
uint8_t u8_log2(uint8_t n) {
  return (n > 0) ? ((8 * sizeof(uint8_t)) - u8_clz(n) - 1) : 0;
}

_circa_static_ _circa_const_
uint8_t u16_log2(uint16_t n) {
  return (n > 0) ? ((8 * sizeof(uint16_t)) - u16_clz(n) - 1) : 0;
}

_circa_static_ _circa_const_
uint8_t u32_log2(uint32_t n) {
  return (n > 0) ? ((8 * sizeof(uint32_t)) - u32_clz(n) - 1) : 0;
}

_circa_static_ _circa_const_
uint8_t u64_log2(uint64_t n) {
  return (n > 0) ? ((8 * sizeof(uint64_t)) - u64_clz(n) - 1) : 0;
}

_circa_static_ _circa_const_
uint8_t usz_log2(size_t n) {
  return (sizeof(size_t) == 8) ? u64_log2(n) : u32_log2(n);
}

/*
** Next Power of Two
*/

_circa_static_ _circa_const_
uint16_t u8_np2(uint8_t n) {
  n -= (n ? 1 : 0);
  return (uint16_t) (1 << (u8_log2(n) + 1));
}

_circa_static_ _circa_const_
uint32_t u16_np2(uint16_t n) {
  n -= (n ? 1 : 0);
  return (uint32_t) (1 << (u16_log2(n) + 1));
}

_circa_static_ _circa_const_
uint64_t u32_np2(uint32_t n) {
  n -= (n ? 1 : 0);
  return (uint64_t) (1 << (u32_log2(n) + 1));
}

_circa_static_ _circa_const_
uint64_t u64_np2(uint64_t n) {
  n -= (n ? 1 : 0);
  return (uint64_t) (1 << (u64_log2(n) + 1));
}

_circa_static_ _circa_const_
size_t usz_np2(size_t n) {
  return (sizeof(size_t) == 8) ? u64_np2(n) : u32_np2(n);
}

/*
** Divide By Ten
*/

_circa_static_ _circa_const_
uint8_t u8_div10(uint8_t n) {
  return (uint8_t) u32_div10(n);
}

_circa_static_ _circa_const_
uint16_t u16_div10(uint16_t n) {
  return (uint16_t) u32_div10(n);
}

_circa_static_ _circa_const_
uint32_t u32_div10(uint32_t n) {
  register const uint64_t inv = 0x199999A;
  return (uint32_t) ((inv * n) >> 32); 
}

_circa_static_ _circa_const_
uint64_t u64_div10(uint64_t n) {
  return n / 10; // TODO: Find an optimization for 64 bit div10, if one exists.
}

_circa_static_ _circa_const_
size_t usz_div10(size_t n) {
  return (sizeof(size_t) == 8) ? u64_div10(n) : u32_div10(n);
}

/*
** Round Up To The Nearest Ten
*/

_circa_static_ _circa_const_
uint16_t u8_ceil10(uint8_t n) {
  return 10 * u8_div10(n + 9); // TODO: Handle possible overflow.
}

_circa_static_ _circa_const_
uint32_t u16_ceil10(uint16_t n) {
  return 10 * u16_div10(n + 9); // TODO: Handle possible overflow.
}

_circa_static_ _circa_const_
uint64_t u32_ceil10(uint32_t n) {
  return 10 * u32_div10(n + 9); // TODO: Handle possible overflow.
}

_circa_static_ _circa_const_
uint64_t u64_ceil10(uint64_t n) {
  return 10 * u64_div10(n + 9); // TODO: Handle possible overflow.
}

_circa_static_ _circa_const_
size_t usz_ceil10(size_t n) {
  return (sizeof(size_t) == 8) ? u64_ceil10(n) : u32_ceil10(n);
}

/*
** Prime Greater Than
*/

_circa_static_ _circa_pure_
uint16_t u8_primegt(uint8_t n) {
  return (uint16_t) circa_primes[u16_ctz(u8_np2(n))];
}

_circa_static_ _circa_pure_
uint32_t u16_primegt(uint16_t n) {
  return (uint32_t) circa_primes[u32_ctz(u16_np2(n))];
}

_circa_static_ _circa_pure_
uint64_t u32_primegt(uint32_t n) {
  return (uint64_t) circa_primes[u64_ctz(u32_np2(n))];
}

_circa_static_ _circa_pure_
uint64_t u64_primegt(uint64_t n) {
  return (uint64_t) circa_primes[u64_ctz(u64_np2(n))];
}

_circa_static_ _circa_pure_
size_t usz_primegt(size_t n) {
  return (sizeof(size_t) == 8) ? u64_primegt(n) : u32_primegt(n);
}

#endif /* CIRCA_BITS_H */
