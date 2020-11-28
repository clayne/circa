#ifdef __GNUC__
  #define circa_if_gnu(...) __VA_ARGS__
  #define circa_std_or_gnu(X, ...) __VA_ARGS__
  #define circa_gnu_or_std(X, ...) X
#else
  #define circa_if_gnu(...)
  #define circa_std_or_gnu(X, ...) X
  #define circa_gnu_or_std(X, ...) __VA_ARGS__
#endif

#ifndef likely
  #define likely(X) circa_std_or_gnu(X, __builtin_expect(!!(X), 1))
#endif

#ifndef unlikely
  #define unlikely(X) circa_std_or_gnu(X, __builtin_expect(!!(X), 0))
#endif

#define circa_gnu_attr(...) circa_if_gnu(__attribute__((__VA_ARGS__)))

#ifdef __GNUC__
  #define circa_gnu_builtin(F, A) \
    if (__builtin_types_compatible_p(__typeof__(A), unsigned long long)) { \
      return (uint8_t) __builtin_##F##ll(A); \
    } else if (__builtin_types_compatible_p(__typeof__(A), unsigned long)) { \
      return (uint8_t) __builtin_##F##l(A); \
    } else { \
      return (uint8_t) __builtin_##F(A); \
    }
#else
  #define circa_gnu_builtin(F, A)
#endif
