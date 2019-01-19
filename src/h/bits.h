/*
** bits.h | The Circa Library Set | Numerical functions.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_BITS_H
#define CIRCA_BITS_H

/*
** Because we need a prime number as our capacity for `Dict` in order to have
** the best hash distribution, the solution used is to align an input number to
** the next highest power of two, get the 2's logarithm of that, and then get
** the corresponding prime number higher than that from the array of primes
** below.
*/

static const uint32_t circa_primes[] = {
  2,       2,       5,       11,     // 1,       2,       4,       8
  17,      37,      67,      131,    // 16,      32,      64,      128,
  257,     521,     1031,    2053,   // 256,     512,     1024,    2048
  4099,    8209,    16411,   32771,  // 4096,    8192,    16384,   32768,
  65537,   131101,  262147,  524309, // 65536,   131072,  262144,  524288
  1048583, 2097169, 4194319, 8388617 // 1048576, 2097152, 4194304, 8388608
};

/*
** Then we have a portable version of binary popcount, which tells you how many
** bits are set to 1 in an unsigned integer.
*/

static inline uint8_t  u8_pop(uint8_t  n);
static inline uint8_t u16_pop(uint16_t n);
static inline uint8_t u32_pop(uint32_t n);
static inline uint8_t u64_pop(uint64_t n);
static inline  size_t usz_pop(size_t   n);

/*
** There's also a portable implementation of CLZ, standing for "Count Leading 
** Zeroes." As the name would imply, it counts the number of leading zeroes in
** an unsigned integer.
*/

static inline uint8_t  u8_clz(uint8_t  n);
static inline uint8_t u16_clz(uint16_t n);
static inline uint8_t u32_clz(uint32_t n);
static inline uint8_t u64_clz(uint64_t n);
static inline  size_t usz_ctz(size_t   n);

/*
** Similarly there is a portable version of CTZ, similar to CLZ but counting
** trailing zeroes instead.
*/

static inline uint8_t  u8_ctz(uint8_t  n);
static inline uint8_t u16_ctz(uint16_t n);
static inline uint8_t u32_ctz(uint32_t n);
static inline uint8_t u64_ctz(uint64_t n);
static inline  size_t usz_ctz(size_t   n);

/*
** Next we have an implementation of the binary logarithm, which finds its
** output by doing `(sizeof(T) * 8) - clz(n) - 1`, essentially subtracting
** the number of leading zeroes from the number of total bits the number has.
**
** Through a simple demonstration this can verify that this works:
**
** |         8 -> Binary 0000 1000 |
** | 0000 1000 -> CLZ of 4         |
** | 8 - 4 - 1 -> log2 of 8 is 3   |
** |    1 << 3 -> 8                |
**
*/

static inline uint8_t  u8_log2(uint8_t  n);
static inline uint8_t u16_log2(uint16_t n);
static inline uint8_t u32_log2(uint32_t n);
static inline uint8_t u64_log2(uint64_t n);
static inline  size_t usz_log2(size_t   n);

/*
** Then we get a "next power of two" function, named `np2`. This uses the `log2`
** function above on the input, adds one to it, then shifts over one. To put it
** more precisely: `1 << (log2(n ? n - 1 : n) + 1)`.
**
** We can demonstrate this below:
** 
** Example 1) Keeping at current power of two.
** |        8 - 1 -> 7                |
** |            7 -> Binary 0000 0111 |
** |    0000 0111 -> CLZ of 7 is 5    |
** |    8 - 5 - 1 -> log2 of 7 is 3   |
** | 1 << (2 + 1) -> np2 of 8 is 8    |
** 
** Example 2) Upping to next power otherwise.
** |        9 - 1 -> 8                |
** |            8 -> Binary 0000 1000 |
** |    0000 1000 -> CLZ of 8 is 4    |
** |    8 - 4 - 1 -> log2 of 8 is 3   |
** | 1 << (3 + 1) -> np2 of 9 is 16   |
*/

static inline uint16_t  u8_np2(uint8_t  n);
static inline uint32_t u16_np2(uint16_t n);
static inline uint64_t u32_np2(uint32_t n);
static inline uint64_t u64_np2(uint64_t n);
static inline size_t   usz_np2(size_t   n);

/*
** Next there's a trick for fast division by 10, taken from MSVC.
**
** Essentially what it does is multiply the divisor by a very close
** approximation of 1/10 * 2^32, then removing 2^32 via a bit shift.
** This approximation is `0x1999999A`.
**
** The 64-bit version works the same way, but 1/10 * 2^64 instead. This means
** that it relies upon 128 bit integers to work, so it first has to check for
** GNU C support and that the host system is 64 bit.
** The approximation is `0x19999997E241C000`. This notably fits in 64 bits, but
** the result of the multiplication will not.
*/

static inline uint8_t   u8_div10(uint8_t  n);
static inline uint16_t u16_div10(uint16_t n);
static inline uint32_t u32_div10(uint32_t n);
static inline uint64_t u64_div10(uint64_t n);
static inline size_t   usz_div10(size_t   n);

/*
** Then we have ceil10, which aligns a number to the nearest higher multiple
** of 10. This works by doing `10 * div10(n + 9)`.
*/

static inline uint16_t  u8_ceil10(uint8_t  n);
static inline uint32_t u16_ceil10(uint16_t n);
static inline uint64_t u32_ceil10(uint32_t n);
static inline uint64_t u64_ceil10(uint64_t n);
static inline size_t   usz_ceil10(size_t   n);

/*
** And there's the most important function for `Dict`'s operation in specific,
** the `primegt` functions. These return a prime number greater than the input;
** it's not guaranteed to be the next highest, just higher than in general.
**
** How this function works is documented back up where `circa_primes` is defined
** at the top of the file, if you want to read about it.
*/

static inline uint32_t  u8_primegt(uint8_t  n);
static inline uint32_t u16_primegt(uint16_t n);
static inline uint32_t u32_primegt(uint32_t n);
static inline uint32_t u64_primegt(uint64_t n);
static inline size_t   usz_primegt(size_t   n);

/*
** Finally we have a generic "round" function to round a number up to some
** multiple of another number, because that just seems genereally useful to
** have. The first argument is the input, the second argument is the number the
** input should be made a multiple of.
*/

static inline uint16_t  u8_round(uint8_t  n, uint8_t  m);
static inline uint32_t u16_round(uint16_t n, uint16_t m);
static inline uint64_t u32_round(uint32_t n, uint32_t m);
static inline size_t   usz_round(size_t   n, size_t   m);

/*
** Because all of these functions are static, we have to implement them here.
** So you can see the exact implementations below.
*/

/*
** Binary Popcount
*/

static inline
uint8_t u8_pop(uint8_t n) {
  return u32_pop(n);
}

static inline
uint8_t u16_pop(uint16_t n) {
  return u32_pop(n);
}

static inline
uint8_t u32_pop(uint32_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_popcount(n);
  #else
    n -= ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
  #endif
}

static inline
uint8_t u64_pop(uint64_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_popcountll(n);
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

/*
** Binary CLZ
*/

static inline
uint8_t u8_clz(uint8_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n) - (64 - 8);
  #else
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    return u8_pop(n);
  #endif
}

static inline
uint8_t u16_clz(uint16_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n) - (64 - 16);
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    return u16_pop(n);
  #endif
}

static inline
uint8_t u32_clz(uint32_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n) - (64 - 32);
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return u32_pop(n);
  #endif
}

static inline
uint8_t u64_clz(uint64_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_clzll(n);
  #else
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n |= (n >> 32);
    return u64_pop(n);
  #endif
}

static inline
uint8_t usz_clz(size_t n) {
    return (sizeof(size_t) == 8) ? u64_clz(n) : u32_clz(n);
}

/*
** Binary CTZ
*/

static inline
uint8_t u8_ctz(uint8_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n++;
    return n;
  #endif
}

static inline
uint8_t u16_ctz(uint16_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n++;
    return (uint8_t n);
  #endif
}

static inline
uint8_t u32_ctz(uint32_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n++;
    return (uint8_t) n;  
  #endif
}

static inline
uint8_t u64_ctz(uint64_t n) {
  #ifdef __GNUC__
    return (uint8_t) __builtin_ctzll(n);
  #else
    n--;
    n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    n |= (n >> 32);
    n++;
    return (uint8_t) n;
  #endif
}

static inline
size_t usz_ctz(size_t n) {
  return (sizeof(size_t) == 8) ? u64_ctz(n) : u32_ctz(n);
}

/*
** Binary Logarithm
*/

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

/*
** Next Binary Power
*/

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

/*
** Divide By Ten
*/

static inline
uint8_t u8_div10(uint8_t n) {
  return (uint8_t) u32_div10(n);
}

static inline
uint16_t u16_div10(uint16_t n) {
  return (uint16_t) u32_div10(n);
}

static inline
uint32_t u32_div10(uint32_t n) {
  register const uint64_t inv = 0x199999A;
  return (uint32_t) ((inv * n) >> 32); 
}

static inline
uint64_t u64_div10(uint64_t n) {
  #ifdef __GNUC__
    if (sizeof(size_t) == 8) {
      __uint128_t x = 0x19999997E241C000;
      return (uint64_t) ((x * (n + 1)) >> 64);
    } else {
      return n / 10;
    }
  #else
    return n / 10;
  #endif
}

static inline
size_t usz_div10(size_t n) {
  return (sizeof(size_t) == 8) ? u64_div10(n) : u32_div10(n);
}

/*
** Round Up To Nearest Ten
*/

static inline
uint16_t u8_ceil10(uint8_t n) {
  return 10 * u8_div10(n + 9);
}

static inline
uint32_t u16_ceil10(uint16_t n) {
  return 10 * u16_div10(n + 9);
}

static inline
uint64_t u32_ceil10(uint32_t n) {
  return 10 * u32_div10(n + 9);
}

static inline
uint64_t u64_ceil10(uint64_t n) {
  return 10 * u64_div10(n + 9);
}

static inline
size_t usz_ceil10(size_t n) {
  return (sizeof(size_t) == 8) ? u64_ceil10(n) : u32_ceil10(n);
}

/*
** Prime Greater Than N
*/

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

/*
** Generic Round
*/

static inline
uint16_t u8_round(uint8_t n, uint8_t m) {
  return ((n + m - 1) / m) * m;
}

static inline
uint32_t u16_round(uint16_t n, uint16_t m) {
  return ((n + m - 1) / m) * m;
}

static inline
uint64_t u32_round(uint32_t n, uint32_t m) {
  return ((n + m - 1) / m) * m;
}

static inline
uint64_t u64_round(uint64_t n, uint64_t m) {
  return ((n + m - 1) / m) * m;
}

#endif // CIRCA_BITS_H
