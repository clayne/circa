#ifndef CIRCA_MAC_H
#define CIRCA_MAC_H

#define C2(A, B) C2_(A, B)
#define C2_(A, B) A##_##B
#define C3(A, B, C) C3_(A, B, C)
#define C3_(A, B, C) A##_##B##_##C
#define C4(A, B, C, D) C4_(A, B, C, D)
#define C4_(A, B, C, D) A##_##B##_##C##_##D

#define STR_(S) #S
#define STR(S) STR_(S)

#ifdef __GNUC__
  #define circa_gnu_or_std(X, ...) X
  #define circa_std_or_gnu(X, ...) __VA_ARGS__
  #define circa_gnu_only(...) __VA_ARGS__
#else
  #define circa_gnu_or_std(X, ...) __VA_ARGS__
  #define circa_std_or_gnu(X, ...) X
  #define circa_gnu_only(...)
#endif

#define circa_attr(A) circa_gnu_only(__attribute__((A)))
#define circa_use circa_attr(warn_unused_result)

#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

#ifndef likely
  #define likely(X) circa_gnu_or_std(__builtin_expect(!!(X), 1), (X))
#endif

#ifndef unlikely
  #define unlikely(X) circa_gnu_or_std(__builtin_expect(!!(X), 0), (X))
#endif

#ifndef if_likely
  #define if_likely(C) if (likely(C))
#endif

#ifndef if_unlikely
  #define if_unlikely(C) if (unlikely(C))
#endif

#endif /* CIRCA_MAC_H */
