/*
** sum.c | Circa Examples | Summing numbers.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int add(int a, int b) {
  return a + b;
}

int main() {
  Seq(int) xs = seq_alloc(int, 1);
  
  for (int i = 0; i < 10; i++)
    seq_push(xs, i);
  
  seq_collapse(xs, add);

  seq_foreach(xs, x)
    printf("%i\n", x);

  return 0;
}
