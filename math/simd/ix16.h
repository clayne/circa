#include "cx16.h"

#define GEN_UNARY(F, O) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) v) { return circa_gnu_or_std(O v, (C2(T, x16)) {O v.V0, O V.V1, O v.V2, O v.V3, O v.V4, O v.V5, O v.V6, O v.V7, O v.V8, O v.V9, O v.V10, O v.V11, O v.V12, O v.V13, O v.V14, O v.V15}); }
  GEN_UNARY(not, ~);
#undef GEN_UNARY

#define GEN_OP(F, O) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) a, C2(T, x16) b) { return circa_gnu_or_std(a O b, (C2(T, x16)) { a.V0 O b.V0, a.V1 O b.V1, a.V2 O b.V2, a.V3 O b.V3, a.V4 O b.V4, a.V5 O b.V5, a.V6 O b.V6, a.V7 O b.V7, a.V8 O b.V8, a.V9 O b.V9, a.V10 O b.V10, a.V11 O b.V11, a.V12 O b.V12, a.V13 O b.V13, a.V14 O b.V14, a.V15 O b.V15 }); }
  GEN_OP(and, &)
  GEN_OP( or, |)
  GEN_OP(xor, ^)
  GEN_OP(mod, %)
  GEN_OP(lsh, <<)
  GEN_OP(rsh, >>)
#undef GEN_OP

#define GEN_OP1(F, B) static inline C2(T, x16) C3(T, x16, F)(C2(T, x16) v, T f) { return C3(T, x16, B)(v, C2(T, x16_set1)(f)); } 
  GEN_OP1(and1, and)
  GEN_OP1( or1,  or)
  GEN_OP1(xor1, xor)
  GEN_OP1(mod1, mod)
  GEN_OP1(lsh1, lsh)
  GEN_OP1(rsh1, rsh)
#undef GEN_OP1

#define GEN_1OP(F, B) static inline C2(T, x16) C3(T, x16, F)(T f, C2(T, x16) v) { return C3(T, x16, B)(C2(T, x16_set1)(f), v); }
  GEN_1OP(1mod, mod)
  GEN_1OP(1lsh, lsh)
  GEN_1OP(1rsh, rsh)
#undef GEN_1OP
