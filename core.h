/*
** core.h | Circa | Core configuration and debugging macros.
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
** Diagnostics
*/

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wlanguage-extension-token"
#endif

/*
** Dependencies
*/

/* Standard */

#include <stdlib.h>
#include <stdio.h>

/*
** Boolean Definition
*/

#if defined(__STDC_VERSION__)
  #if __STDC_VERSION__ >= 199901L
    #include <stdbool.h>
  #else
    #define CIRCA_BOOL
  #endif
#else
  #define CIRCA_BOOL
#endif

#ifdef CIRCA_BOOL
  typedef char bool;
  const bool true = 1;
  const bool false = 0;
#endif

/*
** Config Defaults
*/

/* Environment */

#define CIRCA_D_STATIC true /* Enable static linkage. Desirable in most cases. */
#define CIRCA_D_INLINE true /* Enable inlining. Desireable in most cases. */

/* Debugging */

#define CIRCA_D_DBG true /* Enable assertions. */

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

/* Debugging */

#if defined(CIRCA_DBG)
  #define _circa_dbg_(...) __VA_ARGS__
#else
  #define _circa_dbg_(...)
#endif

/* Automatic */

#ifdef __GNUC__
  #define _circa_gnu_(...) __VA_ARGS__
#else
  #define _circa_gnu_(...)
#endif

/*
** Config Application
*/

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

typedef const char *const restrict circa_msg;

#endif /* CIRCA_CORE_H */
