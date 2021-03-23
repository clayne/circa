#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_hash.h"
  #define circa_static
#endif

circa_static const uint32_t fnv_prime_32 = 16777619U;
circa_static const uint32_t fnv_offset_32 = 2166136261U;

circa_static const uint64_t fnv_prime_64 = 1099511628211ULL;
circa_static const uint64_t fnv_offset_64 = 14695981039346656037ULL;

#if SIZE_MAX == 4294967296
  circa_static const size_t fnv_prime_size = 16777619U;
  circa_static const size_t fnv_offset_size = 2166136261U;
#else
  circa_static const size_t fnv_prime_size = 1099511628211ULL;
  circa_static const size_t fnv_offset_size = 14695981039346656037ULL;
#endif

#define CIRCA_HASH_DEF(T) \
  circa_static size_t C2(T, hash_seeded)(const T *const restrict x, size_t seed) { return (seed ^ (size_t) *x) * fnv_prime_size; } \
  circa_static size_t C2(T, hash)(const T *const restrict x) { return C2(T, hash_seeded)(x, fnv_offset_size); }
  CIRCA_HASH_DEF(char)
  CIRCA_HASH_DEF(short)
  CIRCA_HASH_DEF(int)
  CIRCA_HASH_DEF(unsigned)
  CIRCA_HASH_DEF(long)
  CIRCA_HASH_DEF(uint8_t)
  CIRCA_HASH_DEF(uint16_t)
  CIRCA_HASH_DEF(uint32_t)
  CIRCA_HASH_DEF(uint64_t)
  CIRCA_HASH_DEF(int8_t)
  CIRCA_HASH_DEF(int16_t)
  CIRCA_HASH_DEF(int32_t)
  CIRCA_HASH_DEF(int64_t)
  CIRCA_HASH_DEF(size_t)
#undef CIRCA_HASH_DEF

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
