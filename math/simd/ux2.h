#include "cx2.h"

#define GEN_UNARY(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v) { return circa_gnu_or_std(O v, (C2(T, x2)) {O v.V0, O v.V1}); }
  GEN_UNARY(not, ~)
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b) { return circa_gnu_or_std(a O b, (C2(T, x2)) { a.V0 O b.V0, a.V1 O b.V1 }); }
  GEN_OP(and,  &)
  GEN_OP(or,   |)
  GEN_OP(xor,  ^)
  GEN_OP(mod,  %)
  GEN_OP(lsh, <<)
  GEN_OP(rsh, >>)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) v, T f) { return C3(T, x2, B)(v, C2(T, x2_set1)(f)); }
  GEN_OP1(and1, and)
  GEN_OP1(or1,   or)
  GEN_OP1(xor1, xor)
  GEN_OP1(mod1, mod)
  GEN_OP1(lsh1, lsh)
  GEN_OP1(rsh1, rsh)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x2) C3(T, x2, F)(T f, C2(T, x2) v) { return C3(T, x2, B)(C2(T, x2_set1)(f), v); }
  GEN_1OP(1mod, mod)
  GEN_1OP(1lsh, lsh)
  GEN_1OP(1rsh, rsh)
#undef GEN_1OP
