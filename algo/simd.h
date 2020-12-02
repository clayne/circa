/*
** Description
**   This header offers a complete set of portable (but unstable wrt ABI)
**   vectors that will compile down to using SIMD if enabled, or scalar
**   operations otherwise.
** Dependencies
**   <stdint.h>
**   "circa/macro/cat.h"
**   "circa/macro/gnu.h"
** Source
**   https://github.com/davidgarland/circa
*/

#if defined(__GNUC__) && defined(__amd64__)
  #include <x86intrin.h>
#endif

#define x(V) circa_std_or_gnu((V).x, (V)[0])
#define y(V) circa_std_or_gnu((V).y, (V)[1])
#define z(V) circa_std_or_gnu((V).z, (V)[2])
#define w(V) circa_std_or_gnu((V).w, (V)[4])
#define at(V, I) circa_std_or_gnu(((float*) &(V).x) + (I), (V)[I])

#define vectypes(T) \
typedef circa_std_or_gnu(struct { T x; T y; }, T) C2(T, x2) circa_if_gnu(__attribute__((vector_size(2 * sizeof(T))))); \
typedef circa_std_or_gnu(struct { T x; T y; T z; }, T) C2(T, x3) circa_if_gnu(__attribute__((vector_size(4 * sizeof(T))))); \
typedef circa_std_or_gnu(struct { T x; T y; T z; T w; }, T) C2(T, x4) circa_if_gnu(__attribute__((vector_size(4 * sizeof(T)))));
  vectypes(uint8_t)
  vectypes(uint16_t)
  vectypes(uint32_t)
  vectypes(uint64_t)
  vectypes(int8_t)
  vectypes(int16_t)
  vectypes(int32_t)
  vectypes(int64_t)
  vectypes(float)
  vectypes(double)
#undef vectypes

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpsabi"
#endif

static inline float_x2 float_x2_trunc(float_x2);
static inline double_x2 double_x2_trunc(double_x2);

#define T uint8_t
  #include "simd/ixn.h"
#undef T

#define T uint16_t
  #include "simd/ixn.h"
#undef T

#define T uint32_t
  #include "simd/ixn.h"
#undef T

#define T uint64_t
  #include "simd/ixn.h"
#undef T

#define T int8_t
  #include "simd/ixn.h"
#undef T

#define T int16_t
  #include "simd/ixn.h"
#undef T

#define T int32_t
  #include "simd/ixn.h"
#undef T

#define T int64_t
  #include "simd/ixn.h"
#undef T

#define T float
#define I int32_t
#define U uint32_t
  #include "simd/fxn.h"
#undef U
#undef I
#undef T

#define T double
#define I int64_t
#define U uint64_t
  #include "simd/fxn.h"
#undef U
#undef I
#undef T

static inline
float_x2 float_x2_trunc(float_x2 v) {
#if defined(__GNUC__) && defined(__SSE4_1__)
   float_x4 r = _mm_round_ps(float_x4_set(x(v), y(v), 0, 0), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
   return float_x2_set(x(r), y(r));
#else
   return float_x2_set(truncf(x(v)), truncf(y(v)));
#endif
}

static inline
double_x2 double_x2_trunc(double_x2 v) {
#if defined(__GNUC__) && defined(__SSE4_1__)
  return _mm_round_pd(v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
#else
  return double_x2_set(trunc(x(v)), trunc(y(v)));
#endif
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif
