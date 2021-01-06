#include <stdlib.h>
#include <stdio.h>

#include "../circa_mac.h"
#include "../circa_err.h"

#define T int
  #include "../circa_arr.c"
#undef T

int main() {
  arr_int xs;
  arr_int_alloc(&xs);
  arr_int_free(&xs);
  return 0;
}
