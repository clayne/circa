#include <stdbool.h>
#include <threads.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"
#include "../circa_cmp.h"
#include "../circa_hash.h"

#define K int
#define V int
  #include "../circa_map.h"
  #include "../circa_smap.h"
#undef V
#undef K

#define SIZ 1000000

int main() {
  smap_int_int m;
  smap_int_int_alloc(&m, 4);

  #pragma omp parallel for num_threads(4)
  for (int i = 0; i < 4; i++) {
    for (int j = i * SIZ; j < (i + 1) * SIZ; j++) {
      smap_int_int_set(&m, &j, &i);
    }
  }

  smap_int_int_free(&m);
  return 0;
}
