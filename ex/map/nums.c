/*
** nums.c | Circa Examples | Mapping numbers.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

typedef struct {
  int x, y, z;
} Nums;

void nums_print(Nums a, Nums b) {
  printf("%i, %i, %i -> %i, %i, %i\n", a.x, a.y, a.z, b.x, b.y, b.z);
}

int main() {
  Nums a = {0, 1, 2};
  Nums b = {2, 1, 0};
  Nums c = {0, 1, 0};
  
  Map(Nums, Nums) m = NULL;
  
  ce_critical
    m = map_alloc(Nums, Nums, 1);
  
  map_set(m, &a, &b);
  map_set(m, &b, &c);
  map_set(m, &c, &a);
 
  map_foreach(m, x, y)
    nums_print(x, y);

  map_free(m);
  return 0;
}
