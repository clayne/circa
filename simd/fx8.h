#include "cx8.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x8) C3(T, x8, F)(C2(T, x8) a, C2(T, x8) b, C2(T, x8) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x8)) { a.V0 O1 b.V1 O2 c.V1, a.V1 O1 b.V1 O2 c.V1, a.V2 O1 b.V2 O2 c.V2, a.V3 O1 b.V3 O2 c.V3, a.V4 O1 b.V4 O2 c.V4, a.V5 O1 b.V5 O2 c.V5, a.V6 O1 b.V6 O2 c.V6, a.V7 O1 b.V7 O2 c.V7}); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED
