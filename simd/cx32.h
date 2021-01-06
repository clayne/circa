typedef circa_std_or_gnu(struct { T V0; T V1; T V2; T V3; T V4; T V5; T V6; T V7; T V8; T V9; T V10; T V11; T V12; T V13; T V14; T V15; T V16; T V17; T V18; T V19; T V20; T V21; T V22; T V23; T V24; T V25; T V26; T V27; T V28; T V29; T V30; T V31; }, T) C2(T, x32) circa_gnu_only(__attribute__((vector_size(32 * sizeof(T)))));

static inline C2(T, x32) C2(T, x32_set)(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15, T v16, T v17, T v18, T v19, T v20, T v21, T v22, T v23, T v24, T v25, T v26, T v27, T v28, T v29, T v30, T v31) { return (C2(T, x32)) {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31}; }
static inline C2(T, x32) C2(T, x32_set1)(T f) { return (C2(T, x32)) {f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) v) { return circa_gnu_or_std(O v, (C2(T, x32)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7, O v.V8, O v.V9, O v.V10, O v.V11, O v.V12, O v.V13, O v.V14, O v.V15, O v.V16, O v.V17, O v.V18, O v.V19, O v.V20, O v.V21, O v.V22, O v.V23, O v.V24, O v.V25, O v.V26, O v.V27, O v.V28, O v.V29, O v.V30, O v.V31}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) a, C2(T, x32) b) { return circa_gnu_or_std(a O b, (C2(T, x32)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7, a.V8 O b.V8, a.V9 O b.V9, a.V10 O b.V10, a.V11 O b.V11, a.V12 O b.V12, a.V13 O b.V13, a.V14 O b.V14, a.V15 O b.V15, a.V16 O b.V16, a.V17 O b.V17, a.V18 O b.V18, a.V19 O b.V19, a.V20 O b.V20, a.V21 O b.V21, a.V22 O b.V22, a.V23 O b.V23, a.V24 O b.V24, a.V25 O b.V25, a.V26 O b.V26, a.V27 O b.V27, a.V28 O b.V28, a.V29 O b.V29, a.V30 O b.V30, a.V31 O b.V31 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x32) C3(T, x32, F)(C2(T, x32) v, T f) { return C3(T, x32, B)(v, C2(T, x32_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x32) C3(T, x32, F)(T f, C2(T, x32) v) { return C3(T, x32, B)(C2(T, x32_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x32) C2(T, x32_sq)(C2(T, x32) v) { return C2(T, x32_mul)(v, v); }

static inline T C2(T, x32_sum)(C2(T, x32) v) { return V0(v) + V1(v) + V2(v) + V3(v) + V4(v) + V5(v) + V6(v) + V7(v) + V8(v) + V9(v) + V10(v) + V11(v) + V12(v) + V13(v) + V14(v) + V15(v) + V16(v) + V17(v) + V18(v) + V19(v) + V20(v) + V21(v) + V22(v) + V23(v) + V24(v) + V25(v) + V26(v) + V27(v) + V28(v) + V29(v) + V30(v) + V31(v); }
