#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_imath.h"
  #define circa_static
#endif

#define def_min_max(T) \
  circa_static T C2(T, min)(T a, T b) { return a < b ? a : b; } \
  circa_static T C2(T, max)(T a, T b) { return a > b ? a : b; }
def_min_max(uint8_t)
def_min_max(uint16_t)
def_min_max(uint32_t)
def_min_max(uint64_t)
def_min_max(size_t)
#undef def_min_max

#ifdef __GNUC__
  #define circa_gnu_builtin(F, A) \
    if (__builtin_types_compatible_p(__typeof__(A), unsigned long long)) { \
      return (uint8_t) __builtin_##F##ll(A); \
    } else if (__builtin_types_compatible_p(__typeof__(A), unsigned long)) { \
      return (uint8_t) __builtin_##F##l(A); \
    } else { \
      return (uint8_t) __builtin_##F(A); \
    }
#else
  #define circa_gnu_builtin(F, A)
#endif

circa_static 
uint8_t uint8_t_bswap(uint8_t n) {
  return n;
}

circa_static 
uint16_t uint16_t_bswap(uint16_t n) {
  circa_gnu_only(return __builtin_bswap16(n));
  return (n << 8) | (n >> 8);
}

circa_static 
uint32_t uint32_t_bswap(uint32_t n) {
  circa_gnu_only(return __builtin_bswap32(n));
  n = (n << 16) | (n >> 16);
  n = ((n & 0x00FF00FF) << 8) | ((n & 0xFF00FF00) >> 8);
  return n;
}

circa_static 
uint64_t uint64_t_bswap(uint64_t n) {
  circa_gnu_only(return __builtin_bswap64(n));
  n = (n << 32) | (n >> 32);
  n = ((n & 0x0000FFFF0000FFFF) << 16) | ((n & 0xFFFF0000FFFF0000) >> 16);
  n = ((n & 0x00FF00FF00FF00FF) << 8) | ((n & 0xFF00FF00FF00FF00) >> 8);
  return n;
}

circa_static 
size_t size_t_bswap(size_t n) {
  return (sizeof(size_t) == 8) ? uint64_t_bswap(n) : uint32_t_bswap(n);
}

circa_static  
uint8_t uint8_t_pop(uint8_t n) {
  circa_gnu_only(return __builtin_popcount(n));
  n -= ((n >> 1) & 0x55);
  n = (n & 0x33) + ((n >> 2) & 0x33);
  return (((n + (n >> 4)) & 0x0F) * 0x01);
}

circa_static  
uint8_t uint16_t_pop(uint16_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x5555);
  n = (n & 0x3333) + ((n >> 2) & 0x3333);
  return (((n + (n >> 4)) & 0x0F0F) * 0x0101) >> 8;
}

circa_static  
uint8_t uint32_t_pop(uint32_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x55555555);
  n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
  return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

circa_static  
uint8_t uint64_t_pop(uint64_t n) {
  circa_gnu_builtin(popcount, n);
  n -= ((n >> 1) & 0x5555555555555555ULL);
  n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
  n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
  return (n * 0x0101010101010101ULL) >> 56;
}

circa_static 
uint8_t size_t_pop(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_pop(n) : uint32_t_pop(n);
}

circa_static  
uint8_t uint8_t_clz(uint8_t n) {
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  return uint8_t_pop(~n);
}

circa_static  
uint8_t uint16_t_clz(uint16_t n) {
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  return uint16_t_pop(~n);
}

circa_static  
uint8_t uint32_t_clz(uint32_t n) {
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  return uint32_t_pop(~n);
}

circa_static  
uint8_t uint64_t_clz(uint64_t n) {
  circa_gnu_builtin(clz, n);
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  n |= (n >> 32);
  return uint64_t_pop(~n);
}

circa_static 
uint8_t size_t_clz(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_clz(n) : uint32_t_clz(n);
}

circa_static 
uint8_t uint8_t_bsr(uint8_t n) {
  return 7 - uint8_t_clz(n);
}

circa_static 
uint8_t uint16_t_bsr(uint16_t n) {
  return 15 - uint16_t_clz(n);
}

circa_static 
uint8_t uint32_t_bsr(uint32_t n) {
  return 31 - uint32_t_clz(n);
}

circa_static 
uint8_t uint64_t_bsr(uint64_t n) {
  return 63 - uint64_t_clz(n);
}

circa_static 
uint8_t size_t_bsr(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_bsr(n) : uint32_t_bsr(n);
}

circa_static  
uint8_t uint8_t_ctz(uint8_t n) {
  circa_gnu_builtin(ctz, n);
  return uint8_t_bsr(n & (~n + 1));
}

circa_static  
uint8_t uint16_t_ctz(uint16_t n) {
  circa_gnu_builtin(ctz, n);
  return uint16_t_bsr(n & (~n + 1));
}

circa_static  
uint8_t uint32_t_ctz(uint32_t n) {
  circa_gnu_builtin(ctz, n);
  return uint32_t_bsr(n & (~n + 1));
}

circa_static
uint8_t uint64_t_ctz(uint64_t n) {
  circa_gnu_builtin(ctz, n);
  return uint64_t_bsr(n & (~n + 1));
}

circa_static
uint8_t size_t_ctz(size_t n) {
  return sizeof(size_t) == 8 ? uint64_t_ctz(n) : uint32_t_ctz(n);
}

#undef circa_gnu_builtin

#undef circa_static

/*
** Copyright 2020 David Garland
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/
