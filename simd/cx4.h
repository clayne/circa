typedef circa_std_or_gnu(struct { T V0; T V1; T V2; T V3; }, T) C2(T, x4) circa_gnu_only(__attribute__((vector_size(4 * sizeof(T)))));

static inline C2(T, x4) C2(T, x4_set)(T v0, T v1, T v2, T v3) { return (C2(T, x4)) {v0, v1, v2, v3}; }
static inline C2(T, x4) C2(T, x4_set1)(T f) { return (C2(T, x4)) {f, f, f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v) { return circa_gnu_or_std(O v, (C2(T, x4)) {O v.V0, O V.V1, O v.V2, O v.V3}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) a, C2(T, x4) b) { return circa_gnu_or_std(a O b, (C2(T, x4)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v, T f) { return C3(T, x4, B)(v, C2(T, x4_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x4) C3(T, x4, F)(T f, C2(T, x4) v) { return C3(T, x4, B)(C2(T, x4_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x4) C2(T, x4_sq)(C2(T, x4) v) { return C2(T, x4_mul)(v, v); }

static inline T C2(T, x4_sum)(C2(T, x4) v) { return V0(v) + V1(v) + V2(v) + V3(v); }

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
