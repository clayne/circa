typedef circa_std_or_gnu(struct { T V0; T V1; T V2; T V3; T V4; T V5; T V6; T V7; T V8; T V9; T V10; T V11; T V12; T V13; T V14; T V15; }, T) C2(T, x16) circa_gnu_only(__attribute__((vector_size(16 * sizeof(T)))));

static inline C2(T, x16) C2(T, x16_set)(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15) { return (C2(T, x16)) {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15}; }
static inline C2(T, x16) C2(T, x16_set1)(T f) { return (C2(T, x16)) {f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) v) { return circa_gnu_or_std(O v, (C2(T, x16)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7, O v.V8, O v.V9, O v.V10, O v.V11, O v.V12, O v.V13, O v.V14, O v.V15}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) a, C2(T, x16) b) { return circa_gnu_or_std(a O b, (C2(T, x16)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7, a.V8 O b.V8, a.V9 O b.V9, a.V10 O b.V10, a.V11 O b.V11, a.V12 O b.V12, a.V13 O b.V13, a.V14 O b.V14, a.V15 O b.V15 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) v, T f) { return C3(T, x16, B)(v, C2(T, x16_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x16) C3(T, x16, F)(T f, C2(T, x16) v) { return C3(T, x16, B)(C2(T, x16_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x16) C2(T, x16_sq)(C2(T, x16) v) { return C2(T, x16_mul)(v, v); }

static inline T C2(T, x16_sum)(C2(T, x16) v) { return V0(v) + V1(v) + V2(v) + V3(v) + V4(v) + V5(v) + V6(v) + V7(v) + V8(v) + V9(v) + V10(v) + V11(v) + V12(v) + V13(v) + V14(v) + V15(v); }

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
