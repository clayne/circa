#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_cmp.h"
  #define circa_static
#endif

#define CIRCA_CMP_DEF(T) circa_static cmp C2(T, cmp)(T *a, T *b) { return (*a < *b) ? LT : (*a > *b) ? GT : EQ; }
  CIRCA_CMP_DEF(char)
  CIRCA_CMP_DEF(short)
  CIRCA_CMP_DEF(int)
  CIRCA_CMP_DEF(unsigned)
  CIRCA_CMP_DEF(long)
  CIRCA_CMP_DEF(uint8_t)
  CIRCA_CMP_DEF(uint16_t)
  CIRCA_CMP_DEF(uint32_t)
  CIRCA_CMP_DEF(uint64_t)
  CIRCA_CMP_DEF(int8_t)
  CIRCA_CMP_DEF(int16_t)
  CIRCA_CMP_DEF(int32_t)
  CIRCA_CMP_DEF(int64_t)
  CIRCA_CMP_DEF(size_t)
#undef CIRCA_CMP_DEF

#undef circa_static
