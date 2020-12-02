#include "cx3.h"

#define GEN_FUSED(F, O1, O2) static inline C2(T, x3) C3(T, x3, F)(C2(T, x3) a, C2(T, x3) b, C2(T, x3) c) { return circa_gnu_or_std(a O1 b O2 c, (C2(T, x3)) { a.x O1 b.x O2 c.x, a.y O1 b.y O2 c.y, a.z O1 b.z O2 c.z }); }
  GEN_FUSED(fma, *, +)
  GEN_FUSED(fms, *, -)
#undef GEN_FUSED

static inline
C2(T, x3) C2(T, x3_abs)(C2(T, x3) v) {
  union { C2(T, x3) f; C2(U, x3) u; } conv = {.f = v};
  conv.u = C2(U, x3_and1)(conv.u, 0x7FFFFFFF);
  return conv.f;
}

static inline
C2(T, x3) C3(T, x3, yzx)(C2(T, x3) v) {
#if __has_builtin(__builtin_shufflevector)
  return __builtin_shufflevector(v, v, 1, 2, 0, 3);
#elif __has_builtin(__builtin_shuffle)
  return __builtin_shuffle(v, (C2(I, x4)) {1, 2, 0, 3});
#else
  return (C2(T, x3)) {y(v), z(v), x(v), w(v)};
#endif
}

static inline
C2(T, x3) C2(T, x3_cross)(C2(T, x3) a, C2(T, x3) b) {
  return C2(T, x3_yzx)(
    C3(T, x3, sub)(
      C3(T, x3, mul)(a, C3(T, x3, yzx)(b)),
      C3(T, x3, mul)(b, C3(T, x3, yzx)(a))
    )
  );
}

static inline
C2(T, x3) C2(T, x3_rot)(C2(T, x3) v, C2(T, x4) q) {
  const C2(T, x3) qv = C2(T, x3_set)(x(q), y(q), z(q));
  const C2(T, x3) t = C2(T, x3_mul1)(C2(T, x3_cross)(qv, v), 2);
  return C2(T, x3_add)(v, C2(T, x3_add)(C2(T, x3_mul1)(t, w(q)), C2(T, x3_cross(qv, t))));
}
