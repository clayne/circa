/*
** bits.h | Circa | Bitwise operations for the rustic types.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_BITS_H
#define CIRCA_BITS_H

/*
** Compatibility Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/bits.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/bits.h]: C99 is required."
#endif

/*
** Dependencies
*/

/* Internal */

#include "core.h"
#include "types.h"

/*
** Constants
*/

static const u64 circa_primes[] = {
  2,     2,      5,      11,    // 1,     2,      4,      8,
  17,    37,     67,     131,   // 16,    32,     64,     128,
  257,   521,    1031,   2053,  // 256,   512,    1024,   2048,
  4099,  8209,   16411,  32771, // 4096,  8192,   16384,  32768,
  65537, 131101, 262147, 524309 // 65536, 131072, 262144, 524288
};

/*
** Function Declarations
*/

/* Binary Popcount */

_circa_ u8  u8_pop(u8  n);
_circa_ u8 u16_pop(u16 n);
_circa_ u8 u32_pop(u32 n);
_circa_ u8 u64_pop(u64 n);

/* Count Leading Zeros */

_circa_ u8  u8_clz(u8  n);
_circa_ u8 u16_clz(u16 n);
_circa_ u8 u32_clz(u32 n);
_circa_ u8 u64_clz(u64 n);

/* Count Trailing Zeros */

_circa_ u8  u8_ctz(u8  n);
_circa_ u8 u16_ctz(u16 n);
_circa_ u8 u32_ctz(u32 n);
_circa_ u8 u64_ctz(u64 n);

/* Binary Logarithm */

_circa_ u8  u8_log2(u8  n);
_circa_ u8 u16_log2(u16 n);
_circa_ u8 u32_log2(u32 n);
_circa_ u8 u64_log2(u64 n);

/* Next Power Of 2 */

_circa_ u8   u8_np2(u8  n);
_circa_ u16 u16_np2(u16 n);
_circa_ u32 u32_np2(u32 n);
_circa_ u64 u64_np2(u64 n);

/* Divide By 10 */

_circa_ u8   u8_div10(u8  n);
_circa_ u16 u16_div10(u16 n);
_circa_ u32 u32_div10(u32 n);
_circa_ u64 u64_div10(u64 n);

/* Round Up To The Nearest 10 */

_circa_ u8   u8_ceil10(u8  n);
_circa_ u16 u16_ceil10(u16 n);
_circa_ u32 u32_ceil10(u32 n);
_circa_ u64 u64_ceil10(u64 n);

/* Prime Greater Than Input */

_circa_ u8   u8_primegt(u8  n);
_circa_ u16 u16_primegt(u16 n);
_circa_ u32 u32_primegt(u32 n);
_circa_ u64 u64_primegt(u64 n);

/*
** Binary Popcount
*/

/*
** -- u64_pop --
** Description
**   Finds the popcount of an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Popcount (u8)
*/

_circa_
u8 u64_pop(u64 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_popcountll(n);
  #else
    n -= ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (n * 0x0101010101010101ULL) >> 56;
  #endif
}

/*
** -- u32_pop --
** Description
**   Finds the popcount of an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Popcount (u8)
*/

_circa_
u8 u32_pop(u32 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_popcount(n);
  #else
    n -= ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
  #endif
}

/*
** -- u16_pop --
** Description
**   Finds the popcount of an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Popcount (u8)
*/

_circa_
u8 u16_pop(u16 n) {
  return u32_pop(n);
}

/*
** -- u8_pop --
** Description
**   Finds the popcount of an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Popcount (u8)
*/

_circa_
u8 u8_pop(u8 n) {
  return u32_pop(n);
}

/*
** Count Leading Zeros
*/

/*
** -- u64_clz --
** Description
**   Counts the leading binary zeros of an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Leading Zeros (u8)
*/

_circa_
u8 u64_clz(u64 n) {
  {
    circa_assert(n != 0, "?", "?");
  }
  #if defined(__GNUC__)
    return (u8) __builtin_clzll(n);
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

/*
** -- u32_clz --
** Description
**   Counts the leading binary zeros of an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Leading Zeros (u8)
*/

_circa_
u8 u32_clz(u32 n) {
  {
    circa_assert(n != 0, "?", "?");
  }
  #if defined(__GNUC__)
    return (u8) __builtin_clzll((u64) n) - (64 - 32);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return u32_pop(n);
  #endif
}

/*
** -- u16_clz --
** Description
**   Counts the leading binary zeros of an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Leading Zeros (u8)
*/

_circa_
u8 u16_clz(u16 n) {
  {
    circa_assert(n != 0, "?", "?");
  }
  #if defined(__GNUC__)
    return (u8) __builtin_clzll((u64) n) - (64 - 16);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    return u16_pop(n);
  #endif
}

/*
** -- u8_clz --
** Description
**   Counts the leading binary zeros of an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Leading Zeros (u8)
*/

_circa_
u8 u8_clz(u8 n) {
  {
    circa_assert(n != 0, "?", "?");
  }
  #if defined(__GNUC__)
    return (u8) __builtin_clzll((u64) n) - (64 - 8);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    return u8_pop(n);
  #endif
}

/*
** Count Trailing Zeros
*/

/*
** -- u64_ctz --
** Description
**   Counts the trailing binary zeros of an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Trailing Zeros (u8)
*/

_circa_
u8 u64_ctz(u64 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n |= (n >> 32);
    n++;
    return n;
  #endif
}

/*
** -- u32_ctz --
** Description
**   Counts the trailing binary zeros of an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Trailing Zeros (u8)
*/

_circa_
u8 u32_ctz(u32 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_ctz(n);
  #else
    n--;
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n++;
    return n;
  #endif
}

/*
** -- u16_ctz --
** Description
**   Counts the trailing binary zeros of an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Trailing Zeros (u8)
*/

_circa_
u8 u16_ctz(u16 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_ctz(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n++;
    return n;
  #endif
}

/*
** -- u8_ctz --
** Description
**   Counts the trailing binary zeros of an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Trailing Zeros (u8)
*/

_circa_
u8 u8_ctz(u8 n) {
  #if defined(__GNUC__)
    return (u8) __builtin_ctz(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n++;
    return n;
  #endif
}

/*
** Binary Logarithm
*/

/*
** -- u64_log2 --
** Description
**   Finds the binary logarithm of an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Binary Logarithm (u8)
*/

_circa_
u8 u64_log2(u64 n) {
  return (n > 0) ? ((8 * sizeof(u64)) - u64_clz(n) - 1) : 0;
}

/*
** -- u32_log2 --
** Description
**   Finds the binary logarithm of an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Binary Logarithm (u8)
*/

_circa_
u8 u32_log2(u32 n) {
  return (n > 0) ? ((8 * sizeof(u32)) - u32_clz(n) - 1) : 0;
}

/*
** -- u16_log2 --
** Description
**   Finds the binary logarithm of an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Binary Logarithm (u16)
*/

_circa_
u8 u16_log2(u16 n) {
  return (n > 0) ? ((8 * sizeof(u16)) - u16_clz(n) - 1) : 0;
}

/*
** -- u8_log2 --
** Description
**   Finds the binary logarithm of an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Binary Logarithm (u8)
*/

_circa_
u8 u8_log2(u8 n) {
  return (n > 0) ? ((8 * sizeof(u8)) - u8_clz(n) - 1) : 0;
}

/*
** Next Power of Two
*/

/*
** -- u64_np2 --
** Description
**   Finds the next power of two greater than an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Next Power of Two (u64)
*/

_circa_
u64 u64_np2(u64 n) {
  return (u64) (1 << (u64_log2(n - 1) + 1));
}

/*
** -- u32_np2 --
** Description
**   Finds the next power of two greater than an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Next Power of Two (u32)
*/

_circa_
u32 u32_np2(u32 n) {
  return (u32) (1 << (u32_log2(n - 1) + 1));
}

/*
** -- u16_np2 --
** Description
**   Finds the next power of two greater than an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Next Power of Two (u16)
*/

_circa_
u16 u16_np2(u16 n) {
  return (u16) (1 << (u16_log2(n - 1) + 1));
}

/*
** -- u8_np2 --
** Description
**   Finds the next power of two greater than an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Next Power of Two (u8)
*/

_circa_
u8 u8_np2(u8 n) {
  return (u8) (1 << (u8_log2(n - 1) + 1));
}

/*
** Divide by Ten
*/

/*
** -- u64_div10 --
** Description
**   Divides an unsigned 64-bit integer by 10.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Unsigned 64-Bit Integer (u64)
*/

_circa_
u64 u64_div10(u64 n) {
  return n / 10; /* TODO: Find optimization for 64 bit div10. */
}

/*
** -- u32_div10 --
** Description
**   Divides an unsigned 32-bit integer by 10.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Unsigned 32-Bit Integer (u32)
*/

_circa_
u32 u32_div10(u32 n) {
  const u64 inv = 0x199999A;
  return (u32) ((inv * n) >> 32);
}

/*
** -- u16_div10 --
** Description
**   Divides an unsigned 16-bit integer by 10.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Unsigned 16-Bit Integer (u16)
*/

_circa_
u16 u16_div10(u16 n) {
  return (u16) u32_div10(n);
}

/*
** -- u8_div10 --
** Description
**   Divides an unsigned 8-bit integer by 10.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Unsigned 8-Bit Integer (u8)
*/

_circa_
u8 u8_div10(u8 n) {
  return (u8) u32_div10(n);
}

/*
** Round Up to the Nearest Ten
*/

/*
** -- u64_ceil10 --
** Description
**   Rounds an unsigned 64-bit integer up to the nearest 10.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Ceiling (u64)
*/

_circa_
u64 u64_ceil10(u64 n) {
  return 10 * u64_div10(n + 9);
}

/*
** -- u32_ceil10 --
** Description
**   Rounds an unsigned 32-bit integer up to the nearest 10.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Ceiling (u32)
*/

_circa_
u32 u32_ceil10(u32 n) {
  return 10 * u32_div10(n + 9);
}

/*
** -- u16_ceil10 --
** Description
**   Rounds an unsigned 16-bit integer up to the nearest 10.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Ceiling (u16)
*/

_circa_
u16 u16_ceil10(u16 n) {
  return 10 * u16_div10(n + 9);
}

/*
** -- u8_ceil10 --
** Description
**   Rounds an unsigned 8-bit integer up to the nearest 10.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Ceiling (u8)
*/

_circa_
u8 u8_ceil10(u8 n) {
  return 10 * u8_div10(n + 9);
}

/*
** Prime Greater Than Input
*/

/*
** -- u64_primegt --
** Description
**   Returns a prime greater than an unsigned 64-bit integer.
** Arguments
**   n: Unsigned 64-Bit Integer (u64)
** Returns
**   Prime (u64)
*/

_circa_
u64 u64_primegt(u64 n) {
  return (u64) circa_primes[u64_ctz(u64_np2(n))];
}

/*
** -- u32_primegt --
** Description
**   Returns a prime number greater than an unsigned 32-bit integer.
** Arguments
**   n: Unsigned 32-Bit Integer (u32)
** Returns
**   Prime (u32)
*/

_circa_
u32 u32_primegt(u32 n) {
  return (u32) circa_primes[u32_ctz(u32_np2(n))];
}

/*
** -- u16_primegt --
** Description
**   Returns a prime number greater than an unsigned 16-bit integer.
** Arguments
**   n: Unsigned 16-Bit Integer (u16)
** Returns
**   Prime (u16)
*/

_circa_
u16 u16_primegt(u16 n) {
  return (u16) circa_primes[u16_ctz(u16_np2(n))];
}

/*
** -- u8_primegt --
** Description
**   Returns a prime number greater than an unsigned 8-bit integer.
** Arguments
**   n: Unsigned 8-Bit Integer (u8)
** Returns
**   Prime (u8)
*/

_circa_
u8 u8_primegt(u8 n) {
  return (u8) circa_primes[u8_ctz(u8_np2(n))];
}

#endif /* CIRCA_BITS_H */
