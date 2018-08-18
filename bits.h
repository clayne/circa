/*
** bits.h | Bitwise operations for the rustic types.
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

/* Binary Popcount */

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
u8 u64_pop(u64 n)
{
  #if defined(__GNUC__)
    return __builtin_popcountll(n);
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
u8 u32_pop(u32 n)
{
  #if defined(__GNUC__)
    return __builtin_popcount(n);
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
u8 u16_pop(u16 n)
{
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
u8 u8_pop(u8 n)
{
  return u32_pop(n);
}

/* Count Leading Zeros */

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
u8 u64_clz(u64 n)
{
  #if defined(__GNUC__)
    return __builtin_clzll(n);
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
u8 u32_clz(u32 n)
{
  #if defined(__GNUC__)
    return __builtin_clz(n);
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
u8 u16_clz(u16 n)
{
  #if defined(__GNUC__)
    return __builtin_clz(n);
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
u8 u8_clz(u8 n)
{
  #if defined(__GNUC__)
    return __builtin_clz(n);
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
u8 u64_ctz(u64 n)
{
  #if defined(__GNUC__)
    return __builtin_ctzll(n);
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
u8 u32_ctz(u32 n)
{
  #if defined(__GNUC__)
    return __builtin_ctz(n);
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
u8 u16_ctz(u16 n)
{
  #if defined(__GNUC__)
    return __builtin_ctz(n);
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
u8 u8_ctz(u8 n)
{
  #if defined(__GNUC__)
    return __builtin_ctz(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n++;
    return n;
  #endif
}

#endif /* CIRCA_BITS_H */
