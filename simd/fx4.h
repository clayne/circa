#include "cx4.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x4) C3(T, x4, F)(C2(T, x4) a, C2(T, x4) b, C2(T, x4) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x4)) { a.V0 O1 b.V1 O2 c.V1, a.V1 O1 b.V1 O2 c.V1, a.V2 O1 b.V2 O2 c.V2, a.V3 O1 b.V3 O2 c.V3 }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED


