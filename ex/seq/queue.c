/*
** queue.c | Circa Examples | Queueing Integers.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Seq(int) xs = seq_alloc(int, 3);
  for (int i = 0; i < 3; i++)
    seq_push(xs, &i);
  for (int i = 0; i < 3; i++)
    printf("%i\n", seq_pull(xs));
  seq_free(xs);
  return 0;
}
