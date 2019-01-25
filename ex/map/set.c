/*
** set.c | Circa Examples | Mapping number sets.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

typedef struct {
  int x, y, z;
} Set;

void sets_print(Set a, Set b) {
  printf("%i, %i, %i -> %i, %i, %i\n", a.x, a.y, a.z, b.x, b.y, b.z);
}

int main() {
  Set a = {0, 1, 2};
  Set b = {2, 1, 0};
  Set c = {0, 1, 0};
  Map(Set, Set) m = map_alloc(Set, Set, 1);
  map_set(m, a, b);
  map_set(m, b, c);
  map_set(m, c, a);
  map_foreach(m, x, y)
    sets_print(x, y);
  map_free(m);
  return 0;
}
