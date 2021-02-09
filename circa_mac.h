#ifndef CIRCA_MAC_H
#define CIRCA_MAC_H

#define C2(A, B) C2_(A, B)
#define C2_(A, B) A##_##B
#define C3(A, B, C) C3_(A, B, C)
#define C3_(A, B, C) A##_##B##_##C
#define C4(A, B, C, D) C4_(A, B, C, D)
#define C4_(A, B, C, D) A##_##B##_##C##_##D

#define STR_(S) #S
#define STR(S) STR_(S)

#ifdef __GNUC__
  #define circa_gnu_or_std(X, ...) X
  #define circa_std_or_gnu(X, ...) __VA_ARGS__
  #define circa_gnu_only(...) __VA_ARGS__
#else
  #define circa_gnu_or_std(X, ...) __VA_ARGS__
  #define circa_std_or_gnu(X, ...) X
  #define circa_gnu_only(...)
#endif

#define circa_attr(A) circa_gnu_only(__attribute__((A)))
#define circa_use circa_attr(warn_unused_result)

#ifndef likely
  #define likely(X) circa_gnu_or_std(__builtin_expect(!!(X), 1), (X))
#endif

#ifndef unlikely
  #define unlikely(X) circa_gnu_or_std(__builtin_expect(!!(X), 0), (X))
#endif

#ifndef if_likely
  #define if_likely(C) if (likely(C))
#endif

#ifndef if_unlikely
  #define if_unlikely(C) if (unlikely(C))
#endif

#endif /* CIRCA_MAC_H */

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
