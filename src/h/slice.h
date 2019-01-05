/*
** slice.h | The Circa Library Set | Slices Header
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SLICE_H
#define CIRCA_SLICE_H

#include "core.h"

typedef struct {
  size_t le, ri;
} Slice;

bool slice_in(Slice s, size_t n);
bool slice_in_len(Slice s, size_t n, size_t l);
bool slice_overlap(Slice a, Slice b);
bool slice_contains(Slice a, Slice b);

#endif // CIRCA_SLICE_H
