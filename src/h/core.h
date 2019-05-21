/*
** core.h | The Circa Library Set | Core utilities.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_CORE_H
#define CIRCA_CORE_H

/*
** Dependencies
*/

/* Circa */

#include <config.h>

/*
** Compiler Checks & Patches
*/

#ifdef __STDC_VERSION__
  #if __STDC_VERSION__ < 201101L
    #error "Circa requires at least C11 to function properly."
  #endif
#else
  #error "Circa requires at least C11 to function properly."
#endif

#ifdef __TINYCC
  #ifndef _Thread_local
    #define _Thread_local
  #endif
#endif

/*
** Portable Attributes
*/

#ifdef CIRCA_GNU
  #define CIRCA_ATTR(...) __attribute__((__VA_ARGS__))
#else
  #define CIRCA_ATTR(...)
#endif

#define CIRCA_PURE    CIRCA_ATTR(const)
#define CIRCA_NONNULL CIRCA_ATTR(nonnull)
#define CIRCA_RETURNS CIRCA_ATTR(warn_unused_result)
#define CIRCA_ALLOCS  CIRCA_ATTR(malloc) CIRCA_RETURNS

#ifdef CIRCA_HEADER_ONLY
  #define CIRCA_EXTERN static
  #define CIRCA_STATIC static
  #define CIRCA static inline
#else
  #define CIRCA_EXTERN extern
  #define CIRCA_STATIC
  #define CIRCA
#endif

#endif // CIRCA_CORE_H
