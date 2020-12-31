#include <stdlib.h>
#include <stdio.h>

#include "../err.h"
#include "../mac.h"

#define T int
  #include "../arr.h"
#undef T

int main() {
  arr_int xs;
  arr_int_alloc(&xs);
  arr_int_free(&xs);
  return 0;
}
