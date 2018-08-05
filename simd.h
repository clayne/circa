/*
** simd.h | SIMD data types and functions.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SIMD_H
#define CIRCA_SIMD_H

/*
** Compatibility Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/simd.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/simd.h]: C99 is required."
#endif

#ifndef __GNUC__
  #error "[circa/simd.h]: GNU C is required."
#endif

/*
** Dependencies
*/

#include "core.h"
#include "types.h"

/*
** Declarator Macros
*/

/* General */

#define Vec(T, S) T##x##S
#define VecFnName(T, S, N) T##x##S##_##N
#define VecFn(T, S, N) Vec(T, S) VecFnName(T, S, N)

#define DeclVec(T, S) \
\
typedef T Vec(T, S) __attribute__((vector_size(S * sizeof(T)))); \
\
_circa_ VecFn(T, S,  add)(Vec(T, S), Vec(T, S)); \
_circa_ VecFn(T, S, addf)(Vec(T, S),         T); \
_circa_ VecFn(T, S,  sub)(Vec(T, S), Vec(T, S)); \
_circa_ VecFn(T, S, subf)(Vec(T, S),         T); \
_circa_ VecFn(T, S, fsub)(        T, Vec(T, S)); \
_circa_ VecFn(T, S,  mul)(Vec(T, S), Vec(T, S)); \
_circa_ VecFn(T, S, mulf)(Vec(T, S),         T); \
_circa_ VecFn(T, S,  div)(Vec(T, S), Vec(T, S)); \
_circa_ VecFn(T, S, divf)(Vec(T, S),         T); \
_circa_ VecFn(T, S, fdiv)(        T, Vec(T, S)); \
\
_circa_ VecFn(T, S, fma)(Vec(T, S), Vec(T, S), Vec(T, S)); \
_circa_ VecFn(T, S, fms)(Vec(T, S), Vec(T, S), Vec(T, S));

/* Specialized */

#define DeclVec2(T) \
\
DeclVec(T, 2) \
\
_circa_ VecFn(T, 2,  set)(T, T); \
_circa_ VecFn(T, 2, setf)(T); \
_circa_ VecFn(T, 2, zero)(void);

#define DeclVec4(T) \
\
DeclVec(T, 4) \
\
_circa_ VecFn(T, 4,  set)(T, T, T, T); \
_circa_ VecFn(T, 4, setf)(T); \
_circa_ VecFn(T, 4, zero)(void);

#define DeclVec8(T) \
\
DeclVec(T, 8) \
\
_circa_ VecFn(T, 8,  set)(T, T, T, T, T, T, T, T); \
_circa_ VecFn(T, 8, setf)(T); \
_circa_ VecFn(T, 8, zero)(void);

#define DeclVec16(T) \
\
DeclVec(T, 16) \
\
_circa_ VecFn(T, 16,  set)(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T); \
_circa_ VecFn(T, 16, setf)(T); \
_circa_ VecFn(T, 16, zero)(void);

#define DeclVec32(T) \
\
DeclVec(T, 32) \
\
_circa_ VecFn(T, 32,  set)(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
                           T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T); \
_circa_ VecFn(T, 32, setf)(T); \
_circa_ VecFn(T, 32, zero)(void);

#define DeclVec64(T) \
\
DeclVec(T, 64) \
\
_circa_ VecFn(T, 64,  set)(T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
                           T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
                           T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, \
                           T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T); \
_circa_ VecFn(T, 64, setf)(T); \
_circa_ VecFn(T, 64, zero)(void);

/*
** Definer Macros
*/

#define DefVec(T, S) \
\
_circa_ VecFn(T, S, fma)(Vec(T, S) a, Vec(T, S) b, Vec(T, S) c) { \
  return a * b + c; \
} \
\
_circa_ VecFn(T, S, fms)(Vec(T, S) a, Vec (T, S) b, Vec(T, S) c) { \
  return a * b - c; \
} \
\
_circa_ VecFn(T, S, add)(Vec(T, S) a, Vec(T, S) b) { \
  return a + b; \
} \
\
_circa_ VecFn(T, S, addf)(Vec(T, S) v, T f) { \
  return v + f; \
} \
\
_circa_ VecFn(T, S, sub)(Vec(T, S) a, Vec(T, S) b) { \
  return a - b; \
} \
\
_circa_ VecFn(T, S, subf)(Vec(T, S) v, T f) { \
  return v - f; \
} \
\
_circa_ VecFn(T, S, fsub)(T f, Vec(T, S) v) { \
  return f - v; \
} \
\
_circa_ VecFn(T, S, mul)(Vec(T, S) a, Vec(T, S) b) { \
  return a * b; \
} \
\
_circa_ VecFn(T, S, mulf)(Vec(T, S) v, T f) { \
  return v * f; \
} \
\
_circa_ VecFn(T, S, div)(Vec(T, S) a, Vec(T, S) b) { \
  return a / b; \
} \
\
_circa_ VecFn(T, S, divf)(Vec(T, S) v, T f) { \
  return v / f; \
} \
\
_circa_ VecFn(T, S, fdiv)(T f, Vec(T, S) v) { \
  return f / v; \
}

#define DefVec2(T) \
\
DefVec(T, 2) \
\
_circa_ VecFn(T, 2, set)(T a, T b) { \
  Vec(T, 2) v = {a, b}; \
  return v; \
} \
\
_circa_ VecFn(T, 2, setf)(T f) { \
  return VecFnName(T, 2, set)(f, f); \
} \
\
_circa_ VecFn(T, 2, zero)(void) { \
  return VecFnName(T, 2, setf)(0); \
}

#define DefVec4(T) \
\
DefVec(T, 4) \
\
_circa_ VecFn(T, 4, set)(T a, T b, T c, T d) { \
  Vec(T, 4) V = {a, b, c, d}; \
  return V; \
} \
\
_circa_ VecFn(T, 4, setf)(T f) { \
  return VecFnName(T, 4, set)(f, f, f, f); \
} \
\
_circa_ VecFn(T, 4, zero)(void) { \
  return VecFnName(T, 4, setf)(0); \
}

#define DefVec8(T) \
\
DefVec(T, 8) \
\
_circa_ VecFn(T, 8, set)(T a, T b, T c, T d, T e, T f, T g, T h) { \
  Vec(T, 8) V = {a, b, c, d, e, f, g, h}; \
  return V; \
} \
\
_circa_ VecFn(T, 8, setf)(T f) { \
  return VecFnName(T, 8, set)(f, f, f, f, f, f, f, f); \
} \
\
_circa_ VecFn(T, 8, zero)(void) { \
  return VecFnName(T, 8, setf)(0); \
}

#define DefVec16(T) \
\
DefVec(T, 16) \
\
_circa_ VecFn(T, 16, set)(T a, T b, T c, T d, T e, T f, T g, T h, \
                          T i, T j, T k, T l, T m, T n, T o, T p) { \
  Vec(T, 16) V = {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p}; \
  return V; \
} \
\
_circa_ VecFn(T, 16, setf)(T f) { \
  return VecFnName(T, 16, set)(f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f); \
} \
\
_circa_ VecFn(T, 16, zero)(void) { \
  return VecFnName(T, 16, setf)(0); \
}

#define DefVec32(T) \
\
DefVec(T, 32) \
\
_circa_ VecFn(T, 32, set)(T  a, T  b, T  c, T  d, T  e, T  f, T  g, T  h, \
                          T  i, T  j, T  k, T  l, T  m, T  n, T  o, T  p, \
                          T  q, T  r, T  s, T  t, T  u, T  v, T  w, T  x, \
                          T  y, T  z, T a2, T b2, T c2, T d2, T e2, T f2) { \
  Vec(T, 32) V = {a,  b,  c,  d,  e,  f,  g,  h, \
                  i,  j,  k,  l,  m,  n,  o,  p, \
                  q,  r,  s,  t,  u,  v,  w,  x, \
                  y,  z,  a2, b2, c2, d2, e2, f2}; \
  return V; \
} \
\
_circa_ VecFn(T, 32, setf)(T f) { \
  return VecFnName(T, 32, set)(f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, \
                               f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f); \
} \
\
_circa_ VecFn(T, 32, zero)(void) { \
  return VecFnName(T, 32, setf)(0); \
}

#define DefVec64(T) \
\
DefVec(T, 64) \
\
_circa_ VecFn(T, 64, set)(T  a, T  b, T  c, T  d, T  e, T  f, T  g, T  h, \
                          T  i, T  j, T  k, T  l, T  m, T  n, T  o, T  p, \
                          T  q, T  r, T  s, T  t, T  u, T  v, T  w, T  x, \
                          T  y, T  z, T a2, T b2, T c2, T d2, T e2, T f2, \
                          T g2, T h2, T i2, T j2, T k2, T l2, T m2, T n2, \
                          T o2, T p2, T q2, T r2, T s2, T t2, T u2, T v2, \
                          T w2, T x2, T y2, T z2, T a3, T b3, T c3, T d3, \
                          T e3, T f3, T g3, T h3, T i3, T j3, T k3, T l3) { \
  Vec(T, 64) V = {a,  b,  c,  d,  e,  f,  g,  h, \
                  i,  j,  k,  l,  m,  n,  o,  p, \
                  q,  r,  s,  t,  u,  v,  w,  x, \
                  y,  z,  a2, b2, c2, d2, e2, f2, \
                  g2, h2, i2, j2, k2, l2, m2, n2, \
                  o2, p2, q2, r2, s2, t2, u2, v2, \
                  w2, x2, y2, z2, a3, b3, c3, d3, \
                  e3, f3, g3, h3, i3, j3, k3, l3}; \
  return V; \
} \
\
_circa_ VecFn(T, 64, setf)(T f) { \
  return VecFnName(T, 64, set)(f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, \
                               f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, \
                               f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, \
                               f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f); \
} \
\
_circa_ VecFn(T, 64, zero)(void) { \
  return VecFnName(T, 64, setf)(0); \
}


/*
** Implementer Macros
*/

#define ImplVec2(T)  DeclVec2(T)  DefVec2(T)
#define ImplVec4(T)  DeclVec4(T)  DefVec4(T)
#define ImplVec8(T)  DeclVec8(T)  DefVec8(T)
#define ImplVec16(T) DeclVec16(T) DefVec16(T)
#define ImplVec32(T) DeclVec32(T) DefVec32(T)
#define ImplVec64(T) DeclVec64(T) DefVec64(T)

/*
** Implementation
*/

#if defined(__GNUC__) && !defined(__clang__) // clang doesn't use "-Wpsabi".
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpsabi"
#endif

ImplVec2(u8)
ImplVec4(u8)
ImplVec8(u8)
ImplVec16(u8)
ImplVec32(u8)
ImplVec64(u8)

ImplVec2(u16)
ImplVec4(u16)
ImplVec8(u16)
ImplVec16(u16)
ImplVec32(u16)

ImplVec2(u32)
ImplVec4(u32)
ImplVec8(u32)
ImplVec16(u32)

ImplVec2(u64)
ImplVec4(u64)
ImplVec8(u64)

ImplVec2(i8)
ImplVec4(i8)
ImplVec8(i8)
ImplVec16(i8)
ImplVec32(i8)
ImplVec64(i8)

ImplVec2(i16)
ImplVec4(i16)
ImplVec8(i16)
ImplVec16(i16)
ImplVec32(i16)

ImplVec2(i32)
ImplVec4(i32)
ImplVec8(i32)
ImplVec16(i32)

ImplVec2(i64)
ImplVec4(i64)
ImplVec8(i64)

ImplVec2(f32)
ImplVec4(f32)
ImplVec8(f32)
ImplVec16(f32)

ImplVec2(f64)
ImplVec4(f64)
ImplVec8(f64)

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif

/*
** Undefinition
*/

#undef Vec
#undef VecFnName
#undef VecFn

#undef DeclVec
#undef DeclVec2
#undef DeclVec4
#undef DeclVec8
#undef DeclVec16
#undef DeclVec32
#undef DeclVec64

#undef DefVec
#undef DefVec2
#undef DefVec4
#undef DefVec8
#undef DefVec16
#undef DefVec32
#undef DefVec64

#undef ImplVec2
#undef ImplVec4
#undef ImplVec8
#undef ImplVec16
#undef ImplVec32
#undef ImplVec64

#endif /* CIRCA_SIMD_H */
