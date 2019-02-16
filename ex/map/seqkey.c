/*
** seqkey.c | Circa Examples | Testing Map(K, V) sequence keys.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Map(Seq(int), int) m = map_alloc(Seq(int), int, 3);

  Seq(int) a = seq_lit(int, 1, 2, 3);
  Seq(int) b = seq_lit(int, 2, 3, 4);
  Seq(int) c = seq_lit(int, 3, 4, 5);

  map_free(m);
  
  seq_free(a);
  seq_free(b);
  seq_free(c);
}
