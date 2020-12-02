static inline C2(T, x4) C2(T, x4_set)(T x, T y, T z, T w) { C2(T, x4) v; x(v) = x; y(v) = y; z(v) = z; w(v) = w; return v; }
static inline C2(T, x4) C2(T, x4_set1)(T f) { C2(T, x4) v; x(v) = f; y(v) = f; z(v) = f; w(v) = f; return v; }

#define GEN_UNARY(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v) { return circa_gnu_or_std(O v, (C2(T, x4)) {O x(v), O y(v), O z(v), O w(v)}); }
  GEN_UNARY(neg, -);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) a, C2(T, x4) b) { return circa_gnu_or_std(a O b, (C2(T, x4)) { a.x O b.x, a.y O b.y, a.z O b.z, a.w O b.w }); }
  GEN_OP(add, +)
  GEN_OP(sub, -)
  GEN_OP(mul, *)
  GEN_OP(div, /)
#undef GEN_OP

#define GEN_OP1(F, O) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) v, T f) { return circa_gnu_or_std(v O f, (C2(T, x4)) {v.x O f, v.y O f, v.z O f, v.w O f}); }
  GEN_OP1(add1, +)
  GEN_OP1(sub1, -)
  GEN_OP1(mul1, *)
  GEN_OP1(div1, /)
#undef GEN_OP1

#define GEN_1OP(F, O) static inline C2(T, x4) C3(T, x4, F)(T f, C2(T, x4) v) { return circa_gnu_or_std(f O v, (C2(T, x4)) {f O v.x, f O v.y, f O v.z, f O v.w}); }
  GEN_1OP(1sub, -)
  GEN_1OP(1div, /)
#undef GEN_1OP

static inline C2(T, x4) C2(T, x4_sq)(C2(T, x4) v) { return C2(T, x4_mul)(v, v); }

static inline T C2(T, x4_sum)(C2(T, x4) v) { return x(v) + y(v) + z(v) + w(v); }
