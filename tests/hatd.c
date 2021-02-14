#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"

#include "../circa_imath.h"

#define T int
  #include "../circa_hatd.h"
#undef T

int main() {
  hatd_int xs;
  hatd_int_alloc(&xs);

  for (int i = 0; i < 10000; i++)
    hatd_int_snoc_v(&xs, i);

  for (int i = 0; i < 10000; i++)
    if (hatd_int_get_v(&xs, i) != i)
      puts("uh oh");

  for (int i = 0; i < 10000; i++)
    if (hatd_int_pop_v(&xs) != 10000 - 1 - i)
      puts("uh oh");

  hatd_int_free(&xs);

  return 0;
}
