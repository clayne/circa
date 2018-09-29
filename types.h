/*
** types.h | Circa | Rustic scalar typedefs.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_TYPES_H
#define CIRCA_TYPES_H

/*
** Compatibility Checks
*/

/* TODO: Use C feature checking macros to implement types.h for C89. */

#ifndef __STDC_VERSION__
  #error "[circa/types.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/types.h]: C99 is required."
#endif

/*
** Dependencies
*/

/* Standard */

#include <stddef.h>
#include <stdint.h>

/* Internal */

#include "core.h"

/*
** Type Definitions
*/

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   usize;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef ptrdiff_t isize;

typedef float       f32;
typedef double      f64;
typedef long double f80;

#endif // CIRCA_TYPES_H
