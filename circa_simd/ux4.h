#include "cx4.h"

#define GEN_UNARY(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v) { return circa_gnu_or_std(O v, (C2(T, x4)) {O v.V0, O V.V1, O v.V2, O v.V3}); }
  GEN_UNARY(not, ~);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) a, C2(T, x4) b) { return circa_gnu_or_std(a O b, (C2(T, x4)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3 }); }
  GEN_OP(and, &)
  GEN_OP(or,  |)
  GEN_OP(xor, ^)
  GEN_OP(mod, %)
  GEN_OP(lsh, <<)
  GEN_OP(rsh, >>)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v, T f) { return C3(T, x4, B)(v, C2(T, x4_set1)(f)); }
  GEN_OP1(and1, and)
  GEN_OP1(or1,   or)
  GEN_OP1(xor1, xor)
  GEN_OP1(mod1, mod)
  GEN_OP1(lsh1, lsh)
  GEN_OP1(rsh1, rsh)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x4) C3(T, x4, F)(T f, C2(T, x4) v) { return C3(T, x4, B)(C2(T, x4_set1)(f), v); }
  GEN_1OP(1mod, mod)
  GEN_1OP(1lsh, lsh)
  GEN_1OP(1rsh, rsh)
#undef GEN_1OP

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
