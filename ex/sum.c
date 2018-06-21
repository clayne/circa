// sum.c

#include "../circa.h"

void puti(int n) { printf("%i\n", n); }

int add(int a, int b) { return a + b; }

int main() {
  Seq(int) ns = seq_lit(int, 1, 2, 3, 4);
  seq_do(ns, puti);
  int n = 0;
  seq_reduce(ns, add, n);
  puts("Sum:");
  puti(n);
  seq_del(ns);
  return 0;
}
