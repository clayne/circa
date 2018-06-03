// odd.c

#include "../circa.h"

void puti(int n) { printf("%i\n", n); }

bool odd(int n) { return n & 1; }

int main() {
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(int, a, odd, b);
  seq_do(int, b, puti);
  seq_del(int, a);
  seq_del(int, b);
  return 0;
}
