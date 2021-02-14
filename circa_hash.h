#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

#define CIRCA_HASH_DECL(T) static size_t C2(T, hash)(T *x);
  CIRCA_HASH_DECL(char)
  CIRCA_HASH_DECL(short)
  CIRCA_HASH_DECL(int)
  CIRCA_HASH_DECL(unsigned)
  CIRCA_HASH_DECL(long)
  CIRCA_HASH_DECL(uint8_t)
  CIRCA_HASH_DECL(uint16_t)
  CIRCA_HASH_DECL(uint32_t)
  CIRCA_HASH_DECL(uint64_t)
  CIRCA_HASH_DECL(int8_t)
  CIRCA_HASH_DECL(int16_t)
  CIRCA_HASH_DECL(int32_t)
  CIRCA_HASH_DECL(int64_t)
  CIRCA_HASH_DECL(size_t)
#undef CIRCA_HASH_DECL

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_hash.c"
#endif

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
