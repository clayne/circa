/*
** slice.c | The Circa Library Set | Slices
** https://github.com/davidgarland/circa
*/

#include "../h/slice.h"

Slice slice_lit(size_t le, size_t ri) {
  Slice s = {le, ri};
  return s;
}

bool slice_in(Slice s, size_t n) {
  return (n >= s.le) && (n <= s.ri);
}

bool slice_in_len(Slice s, size_t n, size_t l) {
  return (n >= s.le) && (n + (l - 1) <= s.ri);
}

bool slice_overlap(Slice a, Slice b) {
  return ((b.le >= a.le) && (b.le <= a.ri))
      || ((b.ri >= a.le) && (b.le <= a.ri));
}

bool slice_contains(Slice a, Slice b) {
  return (a.le <= b.le) && (a.ri >= b.ri);
}
