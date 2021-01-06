#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef struct {
  T **l;
  T **m;
  T **h;
  size_t m_cap;
  size_t l_len;
  size_t m_len;
  size_t h_len;
  size_t len;
} C2(hatc, T);

circa_static circa_err C3(hatc, T, alloc)(C2(hatc, T) *);
circa_static circa_err C3(hatc, T, free)(C2(hatc, T) *);

circa_static T *C3(hatc, T, lookup)(C2(hatc, T) *, size_t);
circa_static circa_err C3(hatc, T, set_v)(C2(hatc, T) *, size_t, T);
circa_static circa_err C3(hatc, T, set_r)(C2(hatc, T) *, size_t, T *);
circa_static T C3(hatc, T, get_v)(C2(hatc, T) *, size_t);
circa_static circa_err C3(hatc, T, get_r)(C2(hatc, T) *, size_t, T *);

circa_static circa_err C3(hatc, T, snoc_v)(C2(hatc, T) *, T);
circa_static circa_err C3(hatc, T, snoc_r)(C2(hatc, T) *, T *);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "hatc.c"
#endif
