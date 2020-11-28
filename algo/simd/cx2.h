typedef circa_std_or_gnu(struct { T x; T y; }, T) C2(T, x2) circa_if_gnu(__attribute__((vector_size(2 * sizeof(T)))));

static inline C2(T, x2) C2(T, x2_set)(T x, T y) { C2(T, x2) v; x(v) = x; y(v) = y; return v; }
static inline C2(T, x2) C2(T, x2_set1)(T f) { C2(T, x2) v; x(v) = f; y(v) = f; return v; }

#define GEN_OP(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b) { return circa_gnu_or_std(a O b, (C2(T, x2)) { a.x O b.x, a.y O b.y }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v, T f) { return circa_gnu_or_std(v O f, (C2(T, x2)) {v.x O f, v.y O f}); }
  GEN_OP1(add1, +)
  GEN_OP1(sub1, -)
  GEN_OP1(mul1, *)
  GEN_OP1(div1, /)
#undef GEN_OP1

#define GEN_1OP(F, O) static inline C2(T, x2) C3(T, x2, F)(T f, C2(T, x2) v) { return circa_gnu_or_std(f O v, (C2(T, x2)) {f O v.x, f O v.y}); }
  GEN_1OP(1sub, -)
  GEN_1OP(1div, /)
#undef GEN_1OP
