/*
** slice.h | The Circa Library Set | Simple slices.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SLICE_H
#define CIRCA_SLICE_H

/*
** Dependencies
*/

/* Standard */

#include <stdlib.h>

/*
** Type Definitions
*/

typedef struct {
  size_t le;
  size_t ri;
} Slice;

/*
** Function Declarations
*/

static inline Slice slice_lit(size_t le, size_t ri);

#endif // CIRCA_SLICE_H
