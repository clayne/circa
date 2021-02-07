#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

#define CIRCA_IMATH_DECL(T) \
  circa_static T C2(T, min)(T, T); \
  circa_static T C2(T, max)(T, T); \
  circa_static T C2(T, bswap)(T); \
  circa_static uint8_t C2(T, pop)(T); \
  circa_static uint8_t C2(T, clz)(T); \
  circa_static uint8_t C2(T, bsr)(T); \
  circa_static uint8_t C2(T, ctz)(T);
  CIRCA_IMATH_DECL(uint8_t)
  CIRCA_IMATH_DECL(uint16_t)
  CIRCA_IMATH_DECL(uint32_t)
  CIRCA_IMATH_DECL(uint64_t)
  CIRCA_IMATH_DECL(size_t)
#undef CIRCA_IMATH_DECL

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_imath.c"
#endif
