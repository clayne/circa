#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"

#include "../circa_imath.h"

#define T int
#define C 5
  #include "../circa_hatc.h"
#undef C
#undef T

int main() {
  hatc_int xs;
  hatc_int_alloc(&xs);

  for (int i = 0; i < 10000; i++)
    hatc_int_snoc_v(&xs, i);

  hatc_int_free(&xs);
  return 0;
}
