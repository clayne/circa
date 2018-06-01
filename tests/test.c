#include "../circa.h"

void puti(int n) {
  printf("%i\n", n);
}

int main() {
  Arena a = arena_new(int, 5);
  int *n = arena_take(int, a);
  arena_give(int, a, n);
  arena_del(int, a);
  return 0;
}
