#include "cx2.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b, C2(T, x2) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x2)) { a.x O1 b.x O2 c.x, a.y O1 b.y O2 c.y }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED
