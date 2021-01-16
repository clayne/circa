typedef struct { S xs; S ys; } C2(S, v2);

#define GEN_OP(F) static inline C2(S, v2) C3(S, v2, F)(C2(S, v2) a, C2(S, v2) b) { return (C2(S, v2)) { C2(S, add)(a.xs, b.xs), C2(S, add)(a.ys, b.ys) }; }
  GEN_OP(add)
  GEN_OP(sub)
  GEN_OP(mul)
  GEN_OP(div)
#undef GEN_OP
