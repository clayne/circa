#include "cx4.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) a, C2(T, x4) b, C2(T, x4) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x4)) { a.x O1 b.x O2 c.x, a.y O1 b.y O2 c.y, a.z O1 b.z O2 c.z, a.w O1 b.w O2 c.w }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED

static inline
C2(T, x4) C2(T, x4_abs)(C2(T, x4) v) {
  union { C2(T, x4) f; C2(U, x4) u; } conv = {.f = v};
  conv.u = C2(U, x4_and1)(conv.u, 0x7FFFFFFF);
  return conv.f;
}
