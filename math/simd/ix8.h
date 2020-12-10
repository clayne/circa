#include "cx8.h"

#define GEN_UNARY(F, O) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) v) { return circa_gnu_or_std(O v, (C2(T, x8)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7}); }
  GEN_UNARY(not, ~)
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) a, C2(T, x8) b) { return circa_gnu_or_std(a O b, (C2(T, x8)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7 }); }
  GEN_OP(and,  &)
  GEN_OP( or,  |)
  GEN_OP(xor,  ^)
  GEN_OP(mod,  %)
  GEN_OP(lsh, <<)
  GEN_OP(rsh, >>)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) v, T f) { return C3(T, x8, B)(v, C2(T, x8_set1)(f)); }
  GEN_OP1(and1, and)
  GEN_OP1( or1,  or)
  GEN_OP1(xor1, xor)
  GEN_OP1(mod1, mod) 
  GEN_OP1(lsh1, lsh)
  GEN_OP1(rsh1, rsh)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x8) C3(T, x8, F)(T f, C2(T, x8) v) { return C3(T, x8, B)(C2(T, x8_set1)(f), v); }
  GEN_1OP(1mod, mod)
  GEN_1OP(1lsh, lsh)
  GEN_1OP(1rsh, rsh)
#undef GEN_1OP
