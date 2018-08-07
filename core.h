/*
** circa/core.h | Circa's core configuration and debugging macros.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_CORE_H
#define CIRCA_CORE_H

/*
** Compatibility Checks
*/

#if defined(__cplusplus)
  #error "[circa/core.h]: C++ is not supported."
#endif

/*
** Dependencies
*/

/* Standard */

#include <stdio.h>

/*
** Boolean Definition
*/

#ifndef __bool_true_false_are_defined
  #ifndef true
    #define true 1
  #endif
  #ifndef false
    #define false 0
  #endif
  #ifndef bool
    #define bool char
  #endif
#endif

/*
** Config Defaults
*/

/* Environment */

#define CIRCA_D_STATIC true // Enable static linkage. Desirable in most cases.
#define CIRCA_D_INLINE true // Enable inlining. Desireable in most cases.

/* Safety & Security */

#define CIRCA_D_SAFE   true // Zero-initialize memory upon allocation.
#define CIRCA_D_SECURE true // Zero memory before freeing to hide data.

/* Debugging */

#define CIRCA_D_DBG true // Enable assertions.

/*
** Config Handling
*/

/* Environment */

#if !defined(CIRCA_STATIC) && !defined(CIRCA_N_STATIC) && CIRCA_D_STATIC
  #define CIRCA_STATIC
#elif defined(CIRCA_STATIC) && defined(CIRCA_N_STATIC)
  #undef CIRCA_STATIC
#endif

#if !defined(CIRCA_INLINE) && !defined(CIRCA_N_INLINE) && CIRCA_D_INLINE
  #define CIRCA_INLINE
#elif defined(CIRCA_INLINE) && defined(CIRCA_N_INLINE)
  #undef CIRCA_INLINE
#endif

/* Safety & Security */

#if !defined(CIRCA_SAFE) && !defined(CIRCA_N_SAFE) && CIRCA_D_SAFE
  #define CIRCA_SAFE
#elif defined(CIRCA_SAFE) && defined(CIRCA_N_SAFE)
  #undef CIRCA_SAFE
#endif

#if !defined(CIRCA_SECURE) && !defined(CIRCA_N_SECURE) && CIRCA_D_SECURE
  #define CIRCA_SECURE
#elif defined(CIRCA_SECURE) && defined(CIRCA_N_SECURE)
  #undef CIRCA_SECURE
#endif

/* Debugging */

#if !defined(CIRCA_DBG) && !defined(CIRCA_N_DBG) && !defined(NDEBUG) && CIRCA_D_DBG
  #define CIRCA_DBG
#elif defined(CIRCA_DBG) && defined(CIRCA_N_DBG)
  #undef CIRCA_DBG
#endif

/* Automatic */

#if defined(__GNUC__)
  #define CIRCA_GNU
#elif defined(__MSC_VER) && !defined(__INTEL_COMPILER)
  #define CIRCA_MSVC
#else
  #pragma message ("Your compiler is not supported. Use a GNU C compiler.")
#endif

/*
** Config "Gate" Macros
*/

/* Environment */

#if defined(CIRCA_STATIC)
  #define _circa_static_(...) __VA_ARGS__
#else
  #define _circa_static_(...)
#endif

#if defined(CIRCA_INLINE)
  #define _circa_inline_(...) __VA_ARGS__
#else
  #define _circa_inline_(...)
#endif

/* Safety & Security */

#if defined(CIRCA_SAFE)
  #define _circa_safe_(...) __VA_ARGS__
#else
  #define _circa_safe_(...)
#endif

#if defined(CIRCA_SECURE)
  #define _circa_secure_(...) __VA_ARGS__
#else
  #define _circa_secure_(...)
#endif

/* Debugging */

#if defined(CIRCA_DBG)
  #define _circa_dbg_(...) __VA_ARGS__
#else
  #define _circa_dbg_(...)
#endif

/* Automatic */

#if defined(CIRCA_GNU)
  #define _circa_gnu_(...) __VA_ARGS__
#else
  #define _circa_gnu_(...)
#endif

#if defined(CIRCA_MSVC)
  #define _circa_msvc_(...) __VA_ARGS__
#else
  #define _circa_msvc_(...)
#endif

/*
** Config Application
*/

/* Macros */

#if defined(CIRCA_GNU)
  #define _circa_typeof_(...) __typeof__(__VA_ARGS__)
#elif defined(CIRCA_MSVC)
  #define _circa_typeof_(...) decltype(__VA_ARGS__)
#elif defined(__cplusplus__)
  #define _circa_typeof_(...) decltype(__VA_ARGS__)
#else
  #pragma message ("Your compiler is not supported. Use GCC, Clang, or MSVC.")
  #pragma message ("Defaulting _circa_typeof_ to __typeof__.")
  #define _circa_typeof_(...) __typeof__(__VA_ARGS__)
#endif

/* Attributes */

#define _circa_rets_ _circa_gnu_(__attribute__((warn_unused_result)))
#define _circa_alcs_ _circa_rets_ _circa_gnu_(__attribute__((malloc)))

#define _circa_ _circa_static_(static) _circa_inline_(inline)

/* Utility Macros */

#define _circa_exp__(M) M
#define _circa_exp_(M) _circa_exp__(M)

#define _circa_str__(S) #S
#define _circa_str_(S) _circa_str__(S)

#define _circa_cat__(A, B) A##B
#define _circa_cat_(A, B) _circa_cat__(A, B)

/* Debugging */

#define circa_error_(MSG, DBG, FNAME, LINE, FUNC) DBG({     \
  printf("Error in file '%s', on line %s:\n", FNAME, LINE); \
  printf("  By function '%s': '%s'\n", FUNC, MSG);          \
  exit(1);                                                  \
})

#define circa_error(E, FNAME, LINE) circa_error_(E, _circa_dbg_, FNAME, LINE, __func__)

#define circa_assert_(COND, CONDSTR, DBG, FNAME, LINE, FUNC) DBG({           \
  if(!(COND)) {                                                              \
    circa_error_("Assertion (" CONDSTR ") failed.", DBG, FNAME, LINE, FUNC); \
  }                                                                          \
})

#define circa_assert(C, FNAME, LINE) circa_assert_(C, #C, _circa_dbg_, FNAME, LINE, __func__)

/*
** Type Definitions
*/

typedef const char *const restrict CircaMsg;

#endif /* CIRCA_CORE_H */
