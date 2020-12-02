#include "cx2.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b, C2(T, x2) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x2)) { a.x O1 b.x O2 c.x, a.y O1 b.y O2 c.y }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED

static inline
C2(T, x2) C2(T, x2_mod)(C2(T, x2) a, C2(T, x2) b) {
  a = C2(T, x2_div)(a, b);
  a = C2(T, x2_sub)(a, C2(T, x2_trunc)(a));
  return C2(T, x2_mul)(a, b);
}

static inline
C2(T, x2) C2(T, x2_mod1)(C2(T, x2) v, T f) {
  return C2(T, x2_mod)(v, C2(T, x2_set1)(f));
}

static inline
C2(T, x2) C2(T, x2_abs)(C2(T, x2) v) {
  union { C2(T, x2) f; C2(U, x2) u; } conv = {.f = v};
  conv.u = C2(U, x2_and1)(conv.u, 0x7FFFFFFF);
  return conv.f;
}
