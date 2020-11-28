#include "cx3.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x3) C3(T, x3, F)(C2(T, x3) a, C2(T, x3) b, C2(T, x3) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x3)) { a.x O1 b.x O2 c.x, a.y O1 b.y O2 c.y, a.z O1 b.z O2 c.z }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED
