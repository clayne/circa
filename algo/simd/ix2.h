#include "cx2.h"

#define GEN_UNARY(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v) { return circa_gnu_or_std(O v, C2(T, x2) {O x(v), O y(v)}); }
  GEN_UNARY(not, ~);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b) { return circa_gnu_or_std(a O b, (C2(T, x2)) {x(a) O x(b), y(a) O y(b)}); }
  GEN_OP(and, &);
  GEN_OP(or, |);
  GEN_OP(xor, ^);
  GEN_OP(mod, %);
#undef GEN_OP

#define GEN_OP1(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v, T f) { return circa_gnu_or_std(v O f, (C2(T, x2)) {x(v) O f, y(v) O f}); }
  GEN_OP1(and1, &);
  GEN_OP1(or1, |);
  GEN_OP1(xor1, ^);
  GEN_OP1(mod1, %);
#undef GEN_OP1

#define GEN_1OP(F, O) static inline C2(T, x2) C3(T, x2, F)(T f, C2(T, x2) v) { return circa_gnu_or_std(f O v, (C2(T, x2)) {f O x(v), f O y(v)}); }
  GEN_1OP(1mod, %);
#undef GEN_1OP
