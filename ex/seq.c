/*
** seq.c | The Circa Library Set | Sequences example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int sq(int x) {
  return x * x;
}

bool odd(int x) {
  return x & 1;
}

int main() {
  Seq(int) xs = seq_alloc(int, 10);
  
  for (int i = 0; i < 10; i++)
    seq_push(xs, &i);

  seq_keep(xs, odd); // Keep only the odd numbers.

  seq_apply(xs, sq); // Square every number.

  seq_foreach(xs, x)
    printf("%i\n", x);
  
  seq_free(xs);
}
