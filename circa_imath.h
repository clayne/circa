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
  CIRCA_DECL_MIN_MAX(uint8_t)
  CIRCA_DECL_MIN_MAX(uint16_t)
  CIRCA_DECL_MIN_MAX(uint32_t)
  CIRCA_DECL_MIN_MAX(uint64_t)
  CIRCA_DECL_MIN_MAX(size_t)
#undef def_min_max

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_imath.c"
#endif
