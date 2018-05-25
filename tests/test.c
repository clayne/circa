#include "../circa.h"

int sum(int a, int b) {
  return a + b;
}

int main() {
  Seq(int) a = seq_new(int, 5);
  int b = 0;

  for (int i = 0; i < 5; i++) seq_set(int, a, i, i);

  seq_reduce(int, a, sum, b);

  printf("%i\n", b);

  return 0;
}
