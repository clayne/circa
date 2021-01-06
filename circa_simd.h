#ifndef CIRCA_SIMD_H
#define CIRCA_SIMD_H

#include "circa_mac.h"

#define  V0(V) circa_std_or_gnu((V).V0,  (V)[ 0])
#define  V1(V) circa_std_or_gnu((V).V1,  (V)[ 1])
#define  V2(V) circa_std_or_gnu((V).V2,  (V)[ 2])
#define  V3(V) circa_std_or_gnu((V).V3,  (V)[ 3])
#define  V4(V) circa_std_or_gnu((V).V4,  (V)[ 4])
#define  V5(V) circa_std_or_gnu((V).V5,  (V)[ 5])
#define  V6(V) circa_std_or_gnu((V).V6,  (V)[ 6])
#define  V7(V) circa_std_or_gnu((V).V7,  (V)[ 7])
#define  V8(V) circa_std_or_gnu((V).V8,  (V)[ 8])
#define  V9(V) circa_std_or_gnu((V).V9,  (V)[ 9])
#define V10(V) circa_std_or_gnu((V).V10, (V)[10])
#define V11(V) circa_std_or_gnu((V).V11, (V)[11])
#define V12(V) circa_std_or_gnu((V).V12, (V)[12])
#define V13(V) circa_std_or_gnu((V).V13, (V)[13])
#define V14(V) circa_std_or_gnu((V).V14, (V)[14])
#define V15(V) circa_std_or_gnu((V).V15, (V)[15])
#define V16(V) circa_std_or_gnu((V).V16, (V)[16])
#define V17(V) circa_std_or_gnu((V).V17, (V)[17])
#define V18(V) circa_std_or_gnu((V).V18, (V)[18])
#define V19(V) circa_std_or_gnu((V).V19, (V)[19])
#define V20(V) circa_std_or_gnu((V).V20, (V)[20])
#define V21(V) circa_std_or_gnu((V).V21, (V)[21])
#define V22(V) circa_std_or_gnu((V).V22, (V)[22])
#define V23(V) circa_std_or_gnu((V).V23, (V)[23])
#define V24(V) circa_std_or_gnu((V).V24, (V)[24])
#define V25(V) circa_std_or_gnu((V).V25, (V)[25])
#define V26(V) circa_std_or_gnu((V).V26, (V)[26])
#define V27(V) circa_std_or_gnu((V).V27, (V)[27])
#define V28(V) circa_std_or_gnu((V).V28, (V)[28])
#define V29(V) circa_std_or_gnu((V).V29, (V)[29])
#define V30(V) circa_std_or_gnu((V).V30, (V)[30])
#define V31(V) circa_std_or_gnu((V).V31, (V)[31])
#define V32(V) circa_std_or_gnu((V).V32, (V)[32])
#define V33(V) circa_std_or_gnu((V).V33, (V)[33])
#define V34(V) circa_std_or_gnu((V).V34, (V)[34])
#define V35(V) circa_std_or_gnu((V).V35, (V)[35])
#define V36(V) circa_std_or_gnu((V).V36, (V)[36])
#define V37(V) circa_std_or_gnu((V).V37, (V)[37])
#define V38(V) circa_std_or_gnu((V).V38, (V)[38])
#define V39(V) circa_std_or_gnu((V).V39, (V)[39])
#define V40(V) circa_std_or_gnu((V).V40, (V)[40])
#define V41(V) circa_std_or_gnu((V).V41, (V)[41])
#define V42(V) circa_std_or_gnu((V).V42, (V)[42])
#define V43(V) circa_std_or_gnu((V).V43, (V)[43])
#define V44(V) circa_std_or_gnu((V).V44, (V)[44])
#define V45(V) circa_std_or_gnu((V).V45, (V)[45])
#define V46(V) circa_std_or_gnu((V).V46, (V)[46])
#define V47(V) circa_std_or_gnu((V).V47, (V)[47])
#define V48(V) circa_std_or_gnu((V).V48, (V)[48])
#define V49(V) circa_std_or_gnu((V).V49, (V)[49])
#define V50(V) circa_std_or_gnu((V).V50, (V)[50])
#define V51(V) circa_std_or_gnu((V).V51, (V)[51])
#define V52(V) circa_std_or_gnu((V).V52, (V)[52])
#define V53(V) circa_std_or_gnu((V).V53, (V)[53])
#define V54(V) circa_std_or_gnu((V).V54, (V)[54])
#define V55(V) circa_std_or_gnu((V).V55, (V)[55])
#define V56(V) circa_std_or_gnu((V).V56, (V)[56])
#define V57(V) circa_std_or_gnu((V).V57, (V)[57])
#define V58(V) circa_std_or_gnu((V).V58, (V)[58])
#define V59(V) circa_std_or_gnu((V).V59, (V)[59])
#define V60(V) circa_std_or_gnu((V).V60, (V)[60])
#define V61(V) circa_std_or_gnu((V).V61, (V)[61])
#define V62(V) circa_std_or_gnu((V).V62, (V)[62])
#define V63(V) circa_std_or_gnu((V).V63, (V)[63])
#define VN(V, N) circa_std_or_gnu((*((float*) V) + (N)), (V)[N])

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpsabi"
#endif

#define T uint8_t
  #include "simd/ux64.h"
  #include "simd/ux32.h"
  #include "simd/ux16.h"
  #include "simd/ux8.h"
  #include "simd/ux4.h"
  #include "simd/ux2.h"
#undef T

#define T uint16_t
  #include "simd/ux32.h"
  #include "simd/ux16.h"
  #include "simd/ux8.h"
  #include "simd/ux4.h"
  #include "simd/ux2.h"
#undef T

#define T uint32_t
  #include "simd/ux16.h"
  #include "simd/ux8.h"
  #include "simd/ux4.h"
  #include "simd/ux2.h"
#undef T

#define T uint64_t
  #include "simd/ux8.h"
  #include "simd/ux4.h"
  #include "simd/ux2.h"
#undef T

#define T int8_t
  #include "simd/ix64.h"
  #include "simd/ix32.h"
  #include "simd/ix16.h"
  #include "simd/ix8.h"
  #include "simd/ix4.h"
  #include "simd/ix2.h"
#undef T

#define T int16_t
  #include "simd/ix32.h"
  #include "simd/ix16.h"
  #include "simd/ix8.h"
  #include "simd/ix4.h"
  #include "simd/ix2.h"
#undef T

#define T int32_t
  #include "simd/ix16.h"
  #include "simd/ix8.h"
  #include "simd/ix4.h"
  #include "simd/ix2.h"
#undef T

#define T int64_t
  #include "simd/ix8.h"
  #include "simd/ix4.h"
  #include "simd/ix2.h"
#undef T

#define T float
#define I int32_t
#define U uint32_t
  #include "simd/fx16.h"
  #include "simd/fx8.h"
  #include "simd/fx4.h"
  #include "simd/fx2.h"
#undef U
#undef I
#undef T

#define T double
#define I int64_t
#define U uint64_t
  #include "simd/fx8.h"
  #include "simd/fx4.h"
  #include "simd/fx2.h"
#undef U
#undef I
#undef T

#define GEN_FLOAT_ABS(W) static inline C2(float, W) C3(float, W, abs)(C2(float, W) v) { union { C2(float, W) f; C2(uint32_t, W) u; } conv = {.f = v}; conv.u = C3(uint32_t, W, and1)(conv.u, 0x7FFFFFFFU); return conv.f; }
  GEN_FLOAT_ABS(x16)
  GEN_FLOAT_ABS(x8)
  GEN_FLOAT_ABS(x4)
  GEN_FLOAT_ABS(x2)
#undef GEN_FLOAT_ABS

#define GEN_DOUBLE_ABS(W) static inline C2(double, W) C3(double, W, abs)(C2(double, W) v) { union { C2(double, W) f; C2(uint64_t, W) u; } conv = {.f = v}; conv.u = C3(uint64_t, W, and1)(conv.u, 0x7FFFFFFFFFFFFFFFULL); return conv.f; }
  GEN_DOUBLE_ABS(x8)
  GEN_DOUBLE_ABS(x4)
  GEN_DOUBLE_ABS(x2)
#undef GEN_FLOAT_ABS

#endif // CIRCA_SIMD_H

/*
** Copyright 2020 David Garland
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/
