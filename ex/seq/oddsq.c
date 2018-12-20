/*
** oddsq.c | Circa Examples | Print all odd squares of 0-9.
** https://github.com/davidgarland/circa
*/

#include <stdbool.h>
#include <circa.h>

void puti(int i);
int sq(int n);
bool odd(int n);

void puti(int i) {
  printf("%i\n", i);
}

int sq(int n) {
  return n * n;
}

bool odd(int n) {
  return n & 1;
}

int main() {
  Seq(int) xs = NULL;
  circa_retry
    xs = seq_alloc(int, 10);
  for (size_t i = 0; i < seq(xs)->cap; i++)
    seq_push(xs, (int) i);
  seq_apply(xs, sq);
  seq_keep(xs, odd);
  seq_do(xs, puti);
  seq_free(xs);
  return 0;
}


