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

/* Initializers */

static inline Slice slice_lit(size_t le, size_t ri);

/* Comparison Operators */

static inline bool slice_in(Slice s, size_t n);
static inline bool slice_in_len(Slice s, size_t n, size_t l);
static inline bool slice_overlap(Slice a, Slice b);
static inline bool slice_contains(Slice a, Slice b);

/*
** Function Definitions
*/

/* Initializers */

static inline
Slice slice_lit(size_t le, size_t ri) {
  Slice s = {le, ri};
  return s;
}

/* Comparison Operators */

static inline
bool slice_in(Slice s, size_t n) {
  return (n >= s.le) && (n <= s.ri);
}

static inline
bool slice_in_len(Slice s, size_t n, size_t l) {
  return (n >= s.le) && ((n + (l - 1)) <= s.ri);
}

static inline
bool slice_overlap(Slice a, Slice b) {
  return ((b.le >= a.le) && (b.le <= a.ri))
      && ((b.ri >= a.le) && (b.le <= a.ri));
}

static inline
bool slice_contains(Slice a, Slice b) {
  return (a.le <= b.le) && (a.ri >= b.ri);
}

#endif // CIRCA_SLICE_H
