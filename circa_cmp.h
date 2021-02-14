#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef enum cmp {
  LT,
  GT,
  EQ
} cmp;

#define CIRCA_CMP_DECL(T) circa_static cmp C2(T, cmp)(T *, T *);
  CIRCA_CMP_DECL(char)
  CIRCA_CMP_DECL(short)
  CIRCA_CMP_DECL(int)
  CIRCA_CMP_DECL(unsigned)
  CIRCA_CMP_DECL(long)
  CIRCA_CMP_DECL(uint8_t)
  CIRCA_CMP_DECL(uint16_t)
  CIRCA_CMP_DECL(uint32_t)
  CIRCA_CMP_DECL(uint64_t)
  CIRCA_CMP_DECL(int8_t)
  CIRCA_CMP_DECL(int16_t)
  CIRCA_CMP_DECL(int32_t)
  CIRCA_CMP_DECL(int64_t)
  CIRCA_CMP_DECL(size_t)
#undef CIRCA_CMP_DECL

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_cmp.c"
#endif
