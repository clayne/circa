/*
** debug.h | The Circa Library Set | Debugging utilities.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DEBUG_H
#define CIRCA_DEBUG_H

/*
** Dependencies
*/

/* Circa */

#include <core.h>

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
  #define circa_log(MSG) circa_log_(__FILE__, __func__, __LINE__, MSG)
#else
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

CIRCA_EXTERN const char *const restrict CE_MSG[CE_LENGTH];

#endif // CIRCA_DEBUG_H
