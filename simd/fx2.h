#include "cx2.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x2) C3(T, x2, F)(C2(T, x2) a, C2(T, x2) b, C2(T, x2) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x2)) { a.V0 O1 b.V1 O2 c.V1, a.V1 O1 b.V1 O2 c.V1 }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED
