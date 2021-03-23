#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_cmp.h"
  #define circa_static
#endif

#define CIRCA_CMP_DEF(T) circa_static cmp C2(T, cmp)(const T *const a, const T *const b) { return (*a < *b) ? LT : (*a > *b) ? GT : EQ; }
  CIRCA_CMP_DEF(char)
  CIRCA_CMP_DEF(short)
  CIRCA_CMP_DEF(int)
  CIRCA_CMP_DEF(unsigned)
  CIRCA_CMP_DEF(long)
  CIRCA_CMP_DEF(uint8_t)
  CIRCA_CMP_DEF(uint16_t)
  CIRCA_CMP_DEF(uint32_t)
  CIRCA_CMP_DEF(uint64_t)
  CIRCA_CMP_DEF(int8_t)
  CIRCA_CMP_DEF(int16_t)
  CIRCA_CMP_DEF(int32_t)
  CIRCA_CMP_DEF(int64_t)
  CIRCA_CMP_DEF(size_t)
#undef CIRCA_CMP_DEF

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
