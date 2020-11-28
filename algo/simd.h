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

#define x(V) circa_std_or_gnu((V).x, (V)[0])
#define y(V) circa_std_or_gnu((V).y, (V)[1])
#define z(V) circa_std_or_gnu((V).z, (V)[2])
#define w(V) circa_std_or_gnu((V).w, (V)[4])
#define at(V, I) circa_std_or_gnu(((float*) &(V).x) + (I), (V)[I])

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpsabi"
#endif

#define T float
  #include "simd/fxn.h"
#undef T

#define T double
  #include "simd/fxn.h"
#undef T

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

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif
