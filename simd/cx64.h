typedef circa_std_or_gnu(struct { T V0; T V1; T V2; T V3; T V4; T V5; T V6; T V7; T V8; T V9; T V10; T V11; T V12; T V13; T V14; T V15; T V16; T V17; T V18; T V19; T V20; T V21; T V22; T V23; T V24; T V25; T V26; T V27; T V28; T V29; T V30; T V31; T V32; T V33; T V34; T V35; T V36; T V37; T V38; T V39; T V40; T V41; T V42; T V43; T V44; T V45; T V46; T V47; T V48; T V49; T V50; T V51; T V52; T V53; T V54; T V55; T V56; T V57; T V58; T V59; T V60; T V61; T V62; T V63; }, T) C2(T, x64) circa_gnu_only(__attribute__((vector_size(64 * sizeof(T)))));

static inline C2(T, x64) C2(T, x64_set)(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15, T v16, T v17, T v18, T v19, T v20, T v21, T v22, T v23, T v24, T v25, T v26, T v27, T v28, T v29, T v30, T v31, T v32, T v33, T v34, T v35, T v36, T v37, T v38, T v39, T v40, T v41, T v42, T v43, T v44, T v45, T v46, T v47, T v48, T v49, T v50, T v51, T v52, T v53, T v54, T v55, T v56, T v57, T v58, T v59, T v60, T v61, T v62, T v63) { return (C2(T, x64)) {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63}; }
static inline C2(T, x64) C2(T, x64_set1)(T f) { return (C2(T, x64)) {f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x64) C3(T, x64, F)(C2(T, x64) v) { return circa_gnu_or_std(O v, (C2(T, x64)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7, O v.V8, O v.V9, O v.V10, O v.V11, O v.V12, O v.V13, O v.V14, O v.V15, O v.V16, O v.V17, O v.V18, O v.V19, O v.V20, O v.V21, O v.V22, O v.V23, O v.V24, O v.V25, O v.V26, O v.V27, O v.V28, O v.V29, O v.V30, O v.V31, O v.V32, O v.V33, O v.V34, O v.V35, O v.V36, O v.V37, O v.V38, O v.v39, O v.V40, O v.V41, O v.V42, O v.V43, O v.V44, O v.V45, O v.V46, O v.V47, O v.V48, O v.V49, O v.V50, O v.V51, O v.V52, O v.V53, O v.V54, O v.V55, O v.V56, O v.V57, O v.V58, O v.V59, O v.V60, O v.V61, O v.V62, O v.V63}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x64) C3(T, x64, F)(C2(T, x64) a, C2(T, x64) b) { return circa_gnu_or_std(a O b, (C2(T, x64)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7, a.V8 O b.V8, a.V9 O b.V9, a.V10 O b.V10, a.V11 O b.V11, a.V12 O b.V12, a.V13 O b.V13, a.V14 O b.V14, a.V15 O b.V15, a.V16 O b.V16, a.V17 O b.V17, a.V18 O b.V18, a.V19 O b.V19, a.V20 O b.V20, a.V21 O b.V21, a.V22 O b.V22, a.V23 O b.V23, a.V24 O b.V24, a.V25 O b.V25, a.V26 O b.V26, a.V27 O b.V27, a.V28 O b.V28, a.V29 O b.V29, a.V30 O b.V30, a.V31 O b.V31 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x64) C3(T, x64, F)(C2(T, x64) v, T f) { return C3(T, x64, B)(v, C2(T, x64_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x64) C3(T, x64, F)(T f, C2(T, x64) v) { return C3(T, x64, B)(C2(T, x64_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x64) C2(T, x64_sq)(C2(T, x64) v) { return C2(T, x64_mul)(v, v); }

static inline T C2(T, x64_sum)(C2(T, x64) v) { return V0(v) + V1(v) + V2(v) + V3(v) + V4(v) + V5(v) + V6(v) + V7(v) + V8(v) + V9(v) + V10(v) + V11(v) + V12(v) + V13(v) + V14(v) + V15(v) + V16(v) + V17(v) + V18(v) + V19(v) + V20(v) + V21(v) + V22(v) + V23(v) + V24(v) + V25(v) + V26(v) + V27(v) + V28(v) + V29(v) + V30(v) + V31(v) + V32(v) + V33(v) + V34(v) + V35(v) + V36(v) + V37(v) + V38(v) + V39(v) + V40(v) + V41(v) + V42(v) + V43(v) + V44(v) + V45(v) + V46(v) + V47(v) + V48(v) + V49(v) + V50(v) + V51(v) + V52(v) + V53(v) + V54(v) + V55(v) + V56(v) + V57(v) + V58(v) + V59(v) + V60(v) + V61(v) + V62(v) + V63(v); }
