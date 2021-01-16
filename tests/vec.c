#include <stdint.h>
#include <stdio.h>
#include "../circa_vec.h"

int main() {
  uint8_t_x2_v2 v = { uint8_t_x2_set1(5), uint8_t_x2_set1(7) };

  uint8_t_x2_v2 v2 = uint8_t_x2_v2_add(v, v);

  printf("%hhu, %hhu; %hhu, %hhu\n", V0(v2.xs), V1(v2.xs), V0(v2.ys), V1(v2.ys));

  return 0;
}
