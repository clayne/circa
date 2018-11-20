/*
** core.h | The Circa Library Set | Main Header
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_CORE_H
#define CIRCA_CORE_H

/*
** Dependencies
*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
** Config
*/

/* Optimization */

#define CIRCA_D_INLINE true

/* Debugging */

#define CIRCA_D_DBG true

/*
** Config Application
*/

/* Optimization */

#if !defined(CIRCA_INLINE) && !defined(CIRCA_N_INLINE) && CIRCA_D_INLINE
  #define CIRCA_INLINE
#elif defined(CIRCA_INLINE) && defined(CIRCA_N_INLINE)
  #undef CIRCA_INLINE
#endif

/* Debugging */

#if !defined(CIRCA_DBG) && !defined(CIRCA_N_DBG) && !defined(NDEBUG) && CIRCA_D_DBG
  #define CIRCA_DBG
#elif defined(CIRCA_DBG) && defined(CIRCA_N_DBG)
  #undef CIRCA_DBG
#endif

/* Automatic */

#ifdef __GNUC__
  #define CIRCA_GNU
#endif

/*
** Config Gate Macros
*/

/* Optimization */

#ifdef CIRCA_INLINE
  #define circa_inline(...) __VA_ARGS__
#else
  #define circa_inline(...)
#endif

/* Debugging */

#ifdef CIRCA_DBG
  #define circa_dbg(...) __VA_ARGS__
#else
  #define circa_dbg(...)
#endif

/* Automatic */

#ifdef CIRCA_GNU
  #define circa_gnu(...) __VA_ARGS__
#else
  #define circa_gnu(...)
#endif

/*
** Optimization Macros
*/

#define _circa_ circa_inline(extern inline)
#define _circa_rets_ circa_gnu(__attribute__((warn_unused_result)))
#define _circa_alcs_ _circa_rets_ circa_gnu(__attribute__((malloc)))
#define _circa_static_ static inline
#define _circa_pure_ circa_gnu(__attribute__((pure)))
#define _circa_const_ circa_gnu(__attribute__((const)))

/*
** Debug Macros
*/

typedef const char *const restrict CircaDbg;
#define CIRCA_ARGS CircaDbg fname, CircaDbg line
#define CIRCA_STR_(X) #X
#define CIRCA_STR(X) CIRCA_STR_(X)
#define CIRCA_DBGI __FILE__, CIRCA_STR(__LINE__)

#define circa_msg_(MSG, FNAME, LINE, FUNC) circa_dbg({ \
  printf("[%s:%s:%s] %s", FNAME, FUNC, LINE, MSG); \
})
#define circa_msg(MSG, FNAME, LINE) circa_msg_(MSG, FNAME, LINE, __func__)

#define circa_error_(MSG, FNAME, LINE, FUNC) circa_dbg({    \
  printf("Error in file '%s', on line %s:\n", FNAME, LINE); \
  printf("By function '%s': '%s'\n", FUNC, MSG);            \
  exit(1);                                                  \
})
#define circa_error(MSG, FNAME, LINE) circa_error_(MSG, FNAME, LINE, __func__)

#define circa_assert_(COND, CONDMSG, FNAME, LINE, FUNC) circa_dbg({     \
  if (!(COND)) {                                                        \
    circa_error_("Assertion (" CONDMSG ") failed.", FNAME, LINE, FUNC); \
  }                                                                     \
})
#define circa_assert(COND, FNAME, LINE) circa_assert_(COND, #COND, FNAME, LINE, __func__)

#define circa_info(M) circa_msg(M, __FILE__, __LINE__)

#endif /* CIRCA_CORE_H */
