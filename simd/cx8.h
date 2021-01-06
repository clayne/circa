typedef circa_std_or_gnu(struct { T V0; T V1; T V2; T V3; T V4; T V5; T V6; T V7; }, T) C2(T, x8) circa_gnu_only(__attribute__((vector_size(8 * sizeof(T)))));

static inline C2(T, x8) C2(T, x8_set)(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7) { return (C2(T, x8)) {v0, v1, v2, v3, v4, v5, v6, v7}; }
static inline C2(T, x8) C2(T, x8_set1)(T f) { return (C2(T, x8)) {f, f, f, f, f, f, f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) v) { return circa_gnu_or_std(O v, (C2(T, x8)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) a, C2(T, x8) b) { return circa_gnu_or_std(a O b, (C2(T, x8)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) v, T f) { return C3(T, x8, B)(v, C2(T, x8_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x8) C3(T, x8, F)(T f, C2(T, x8) v) { return C3(T, x8, B)(C2(T, x8_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x8) C2(T, x8_sq)(C2(T, x8) v) { return C2(T, x8_mul)(v, v); }

static inline T C2(T, x8_sum)(C2(T, x8) v) { return V0(v) + V1(v) + V2(v) + V3(v) + V4(v) + V5(v) + V6(v) + V7(v); }
