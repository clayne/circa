/*
** insdel.c | Circa Examples | Insertion/deletion example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main () {
  Seq(int) xs = seq_alloc(int, 1);

  for (int i = 0; i < 10; i++)
    seq_ins(xs, 0, &(int){i});

  seq_del(xs, 5);

  seq_foreach(xs, x) {
    printf("%i\n", x);
  }

  seq_free(xs);
}
