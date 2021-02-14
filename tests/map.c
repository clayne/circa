#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#define CIRCA_N_LOG
#define CIRCA_N_DEBUG

#include "../circa_mac.h"
#include "../circa_err.h"
#include "../circa_cmp.h"
#include "../circa_hash.h"

#define K int
#define V int
  #include "../circa_map.h"
#undef V
#undef K

int main() {
  map_int_int m;
  map_int_int_alloc(&m);
  
  for (int i = 0; i < 200000000; i++) {
    int j = i + 1;
    map_int_int_set(&m, &i, &j);
  }

  for (int i = 0; i < 200000000; i++) {
    int j = i + 1;
    if (*map_int_int_lookup(&m, &i) != j) {
      puts("uh oh!");
    }
  }
  
  printf("%zu\n", m.cap);

  map_int_int_free(&m);
  return 0;
}
