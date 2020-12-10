typedef circa_std_or_gnu(struct { T V0; T V1; }, T) C2(T, x2) circa_if_gnu(__attribute__((vector_size(2 * sizeof(T)))));

static inline C2(T, x2) C2(T, x2_set)(T x, T y) { return (C2(T, x2)) {x, y}; }
static inline C2(T, x2) C2(T, x2_set1)(T f) { return (C2(T, x2)) {f, f}; }

#define GEN_UNARY(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v) { return circa_gnu_or_std(O v, (C2(T, x2)) {O v.V0, O v.V1}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b) { return circa_gnu_or_std(a O b, (C2(T, x2)) { a.V0 O b.V0, a.V1 O b.V1 }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v, T f) { return C3(T, x2, B)(v, C2(T, x2_set1)(f)); }
  GEN_OP1(add1, add)
  GEN_OP1(sub1, sub)
  GEN_OP1(mul1, mul)
  GEN_OP1(div1, div)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x2) C3(T, x2, F)(T f, C2(T, x2) v) { return C3(T, x2, B)(C2(T, x2_set1)(f), v); }
  GEN_1OP(1sub, sub)
  GEN_1OP(1div, div)
#undef GEN_1OP

static inline C2(T, x2) C2(T, x2_sq)(C2(T, x2) v) { return C2(T, x2_mul)(v, v); }
static inline T C2(T, x2_sum)(C2(T, x2) v) { return V0(v) + V1(v); }
