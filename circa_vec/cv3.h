typedef struct { S xs; S ys; S zs; } C2(S, v3);

#define GEN_OP(F) static inline C2(S, v3) C3(S, v3, F)(C2(S, v3) a, C2(S, v3) b) { return (C2(S, v3)) { C2(S, F)(a.xs, b.xs), C2(S, F)(a.ys, b.ys), C2(S, F)(a.zs, b.zs) }; }
  GEN_OP(add)
  GEN_OP(sub)
  GEN_OP(mul)
  GEN_OP(div)
#undef GEN_OP

