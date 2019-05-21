/*
** config.h | The Circa Library Set | Configuration file.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_CONFIG_H
#define CIRCA_CONFIG_H

/*
** Config Defaults
*/

/* Debugging */

#define CIRCA_GUARD_D 1 // Whether or not input guard checks are done.
#define CIRCA_LOG_D 0   // Whether or not to log actions to stdout.

/* General */

#define CIRCA_GNU_D 1 // Whether or not GNU C extensions are allowed, if supported.
#define CIRCA_VLA_D 1 // Whether or not to allow VLAs, if supposrted.

/* Linkage */

#define CIRCA_HEADER_ONLY_D 0 // Whether Circa should operate as header-only or not.

/* Allocators */

#define CIRCA_MALLOC_D  malloc  // The default memory allocator.
#define CIRCA_CALLOC_D  calloc  // The default zeroing memory allocator.
#define CIRCA_REALLOC_D realloc // The default reallocator.
#define CIRCA_FREE_D    free    // The default deallocator.

/* Sequences */

#define CIRCA_SEQ_PREALLOC_D 5 // How much Circa should preallocate on require for sequences.

/* Text */

#define CIRCA_TXT_PREALLOC_D 5 // How much Circa should preallocate on require for text.

/*
** Config Handling
*/

/* General */

// GUARD

#if !defined(CIRCA_GUARD) && CIRCA_GUARD_D
  #define CIRCA_GUARD
#endif

#if defined(CIRCA_GUARD) && defined(CIRCA_GUARD_N)
  #undef CIRCA_GUARD
#endif

// LOG

#if !defined(CIRCA_LOG) && CIRCA_LOG_D
  #define CIRCA_LOG
#endif

#if defined(CIRCA_LOG) && defined(CIRCA_LOG_N)
  #undef CIRCA_LOG
#endif

// GNU

#if !defined(CIRCA_GNU) && CIRCA_GNU_D
  #define CIRCA_GNU
#endif

#if defined(CIRCA_GNU) && defined(CIRCA_GNU_N)
  #undef CIRCA_GNU
#endif

// VLA

#if !defined(CIRCA_VLA) && CIRCA_VLA_D
  #define CIRCA_VLA
#endif

#if defined(CIRCA_VLA) && defined(CIRCA_VLA_N)
  #undef CIRCA_VLA
#endif

/* Linkage */

// HEADER_ONLY

#if !defined(CIRCA_HEADER_ONLY) && CIRCA_HEADER_ONLY_D
  #define CIRCA_HEADER_ONLY
#endif

#if defined(CIRCA_HEADER_ONLY) && defined(CIRCA_HEADER_ONLY_N)
  #undef CIRCA_HEADER_ONLY
#endif

/* Allocators */

// MALLOC

#if !defined(CIRCA_MALLOC)
  #define CIRCA_MALLOC CIRCA_MALLOC_D
#endif

// CALLOC

#if !defined(CIRCA_CALLOC)
  #define CIRCA_CALLOC CIRCA_CALLOC_D
#endif

// REALLOC

#if !defined(CIRCA_REALLOC)
  #define CIRCA_REALLOC CIRCA_REALLOC_D
#endif

// FREE

#if !defined(CIRCA_FREE)
  #define CIRCA_FREE CIRCA_FREE_D
#endif

/* Sequences */

// SEQ_PREALLOC

#if !defined(CIRCA_SEQ_PREALLOC)
  #define CIRCA_SEQ_PREALLOC CIRCA_SEQ_PREALLOC_D
#endif

/* Text */

// TXT_PREALLOC

#if !defined(CIRCA_TXT_PREALLOC)
  #define CIRCA_TXT_PREALLOC CIRCA_TXT_PREALLOC_D
#endif

/*
** Config Post-Processing
*/

/* CIRCA_GNU */

#if defined(CIRCA_GNU) && !defined(__GNUC__)
  #undef CIRCA_GNU
#endif

/* CIRCA_VLA */

#if defined(CIRCA_VLA) && defined(__STDC_NO_VLA__)
  #undef CIRCA_VLA
#endif

#endif // CIRCA_CONFIG_H
