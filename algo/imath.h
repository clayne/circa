/*
** Description
**   Implementations of several integer math functions. GNU builtins are used
**   whenever possible, but otherwise there are reasonably efficient fallbacks
**   written in Standard C.
** Dependencies
**   <stddef.h> or <stdlib.h>
**   <stdint.h>
**   "circa/macro/cat.h"
**   "circa/macro/gnu.h"
** Source
**   https://github.com/davidgarland/circa
*/

/*
** Function Prototypes
*/

// Find the minimum of two numbers.
static inline circa_gnu_attr(const)  uint8_t  uint8_t_min( uint8_t,  uint8_t);
static inline circa_gnu_attr(const) uint16_t uint16_t_min(uint16_t, uint16_t);
static inline circa_gnu_attr(const) uint32_t uint32_t_min(uint32_t, uint32_t);
static inline circa_gnu_attr(const) uint64_t uint64_t_min(uint64_t, uint64_t);
static inline circa_gnu_attr(const)   size_t   size_t_min(  size_t,   size_t);

// Find the maximum of two numbers.
static inline circa_gnu_attr(const)  uint8_t  uint8_t_max( uint8_t,  uint8_t);
static inline circa_gnu_attr(const) uint16_t uint16_t_max(uint16_t, uint16_t);
static inline circa_gnu_attr(const) uint32_t uint32_t_max(uint32_t, uint32_t);
static inline circa_gnu_attr(const) uint64_t uint64_t_max(uint64_t, uint64_t);
static inline circa_gnu_attr(const)   size_t   size_t_max(  size_t,   size_t);

// Find the population count of a number; in other words, the number of nonzero
// bits.
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t  uint8_t_pop( uint8_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint16_t_pop(uint16_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint32_t_pop(uint32_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint64_t_pop(uint64_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t   size_t_pop(  size_t);

// Find the number of leading binary zeros in a number. The function is only
// defined for nonzero inputs.
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t  uint8_t_clz( uint8_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint16_t_clz(uint16_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint32_t_clz(uint32_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint64_t_clz(uint64_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t   size_t_clz(  size_t);

// Bit scan reverse. Finds the index of the leftmost nonzero bit from the
// right. This may also be viewed as integer log2. The function is only defined
// for nonzero inputs.
static inline circa_gnu_attr(const) uint8_t  uint8_t_bsr( uint8_t);
static inline circa_gnu_attr(const) uint8_t uint16_t_bsr(uint16_t);
static inline circa_gnu_attr(const) uint8_t uint32_t_bsr(uint32_t);
static inline circa_gnu_attr(const) uint8_t uint64_t_bsr(uint64_t);
static inline circa_gnu_attr(const) uint8_t   size_t_bsr(  size_t);

// Find the number of trailing binary zeros in a number. This function is only
// defined for nonzero inputs.
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t  uint8_t_ctz( uint8_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint16_t_ctz(uint16_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint32_t_ctz(uint32_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t uint64_t_ctz(uint64_t);
static circa_if_gnu(inline) circa_gnu_attr(const) uint8_t   size_t_ctz(  size_t);

/*
** Function Implementations
*/

#define def_min_max(T) \
  static inline circa_gnu_attr(const) T C2(T, min)(T a, T b) { return a < b ? a : b; } \
  static inline circa_gnu_attr(const) T C2(T, max)(T a, T b) { return a > b ? a : b; }
def_min_max(uint8_t)
def_min_max(uint16_t)
def_min_max(uint32_t)
def_min_max(uint64_t)
def_min_max(size_t)
#undef def_min_max

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint8_t_pop(uint8_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x55);
  n = (n & 0x33) + ((n >> 2) & 0x33);
  return (((n + (n >> 4)) & 0x0F) * 0x01);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint16_t_pop(uint16_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x5555);
  n = (n & 0x3333) + ((n >> 2) & 0x3333);
  return (((n + (n >> 4)) & 0x0F0F) * 0x0101) >> 8;
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint32_t_pop(uint32_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x55555555);
  n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
  return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint64_t_pop(uint64_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x5555555555555555ULL);
  n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
  n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
  return (n * 0x0101010101010101ULL) >> 56;
}

static inline circa_gnu_attr(const)
uint8_t size_t_pop(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_pop(n) : uint32_t_pop(n);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint8_t_clz(uint8_t n) {
  assert(n);
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  return uint8_t_pop(~n);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint16_t_clz(uint16_t n) {
  assert(n);
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  return uint16_t_pop(~n);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint32_t_clz(uint32_t n) {
  assert(n);
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  return uint32_t_pop(~n);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint64_t_clz(uint64_t n) {
  assert(n);
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  n |= (n >> 32);
  return uint64_t_pop(~n);
}

static inline circa_gnu_attr(const)
uint8_t size_t_clz(size_t n) {
  assert(n);
  return sizeof(size_t) == 8 ? uint64_t_clz(n) : uint32_t_clz(n);
}

static inline circa_gnu_attr(const)
uint8_t uint8_t_bsr(uint8_t n) {
  return 7 - uint8_t_clz(n);
}

static inline circa_gnu_attr(const)
uint8_t uint16_t_bsr(uint16_t n) {
  return 15 - uint16_t_clz(n);
}

static inline circa_gnu_attr(const)
uint8_t uint32_t_bsr(uint32_t n) {
  return 31 - uint32_t_clz(n);
}

static inline circa_gnu_attr(const)
uint8_t uint64_t_bsr(uint64_t n) {
  return 63 - uint64_t_clz(n);
}

static inline circa_gnu_attr(const)
uint8_t size_t_bsr(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_bsr(n) : uint32_t_bsr(n);
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint8_t_ctz(uint8_t n) {
  assert(n);
  circa_gnu_builtin(ctz, n);
  return uint8_t_bsr(n & (~n + 1));
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint16_t_ctz(uint16_t n) {
  assert(n);
  circa_gnu_builtin(ctz, n);
  return uint16_t_bsr(n & (~n + 1));
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint32_t_ctz(uint32_t n) {
  assert(n);
  circa_gnu_builtin(ctz, n);
  return uint32_t_bsr(n & (~n + 1));
}

static circa_if_gnu(inline) circa_gnu_attr(const)
uint8_t uint64_t_ctz(uint64_t n) {
  assert(n);
  circa_gnu_builtin(ctz, n);
  return uint64_t_bsr(n & (~n + 1));
}

static inline circa_gnu_attr(const)
uint8_t size_t_ctz(size_t n) {
  assert(n);
  return sizeof(size_t) == 8 ? uint64_t_ctz(n) : uint32_t_ctz(n);
}
