/*
** debug.h | The Circa Library Set | Debugging utilities.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DEBUG_H
#define CIRCA_DEBUG_H

/*
** Dependencies
*/

/* Standard */

#include <stdlib.h>
#include <stdio.h>

/* Circa */

#include "core.h"

/*
** Macros
*/

#ifdef CIRCA_GUARD
  #define circa_guard(...) if (__VA_ARGS__)
#else
  #define circa_guard(...) if (0)
#endif

#ifdef CIRCA_LOG
  #define circa_log_(FILE, FUNC, LINE, MSG) \
  do { \
    printf("[circa] %s: In %s on line %zu: %s", FILE, FUNC, LINE, MSG); \
  } while(0)
  #define circa_log(MSG) circa_log_(__FILE__, __func__, (size_t) __LINE__, MSG)
#else
  #define circa_log_(FILE, FUNC, LINE, MSG)
  #define circa_log(MSG)
#endif

/*
** Type Definitions
*/

typedef enum {
  CE_OK = 0,
  CE_NOK = 1,
  CE_ARG,
  CE_OOM,
  CE_OOB,
  CE_FMT,
  CE_FILE_READ,
  CE_FILE_WRITE,
  CE_LENGTH
} CircaError;

/*
** Variable Declarations
*/

CIRCA_EXTERN _Thread_local CircaError CE;

CIRCA_EXTERN char *const restrict CE_MSG[CE_LENGTH];

/*
** Function Declarations
*/

#define circa_throw(E) circa_throw_(__FILE__, __func__, __LINE__, (E))
static inline void circa_throw_(const char *file, const char *func, size_t line, CircaError ce);

/*
** Function Definitions
*/

static inline
void circa_throw_(const char *file, const char *func, size_t line, CircaError ce) {
  CE = ce;
  circa_log_(file, func, line, CE_MSG[ce]);
}

/*
** Header Only Mode
*/

#ifdef CIRCA_HEADER_ONLY
  #include "../c/debug.c"
#endif

#endif // CIRCA_DEBUG_H
