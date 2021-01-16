#include "cx16.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) a, C2(T, x16) b, C2(T, x16) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x16)) { a.V0 O1 b.V1 O2 c.V1, a.V1 O1 b.V1 O2 c.V1, a.V2 O1 b.V2 O2 c.V2, a.V3 O1 b.V3 O2 c.V3, a.V4 O1 b.V4 O2 c.V4, a.V5 O1 b.V5 O2 c.V5, a.V6 O1 b.V6 O2 c.V6, a.V7 O1 b.V7 O2 c.V7, a.V8 O1 b.V8 O2 c.V8, a.V9 O1 b.V9 O2 c.V9, a.V10 O1 b.V10 O2 c.V10, a.V11 O1 b.V11 O2 c.V11, a.V12 O1 b.V11 O2 c.V11, a.V13 O1 b.V13 O2 c.V13, a.V14 O1 b.V14 O2 c.V14, a.V15 O1 b.V15 O2 c.V15 }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED

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
