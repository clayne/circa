/*
** seqmap.c | The Circa Library Set | SeqMap example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  SeqMap(int, int) sm = seqmap_alloc(int, int, 1);

  Seq(int) xs = seq_lit(1, 2, 3); {
    seqmap_set(sm, xs, &(int){5});

    seq_foreach(xs, x) {
      printf("%i\n", x);
    }
  } seq_free(xs);

  seqmap_foreach(sm, ks, v) {
    seq_foreach(ks, k) {
      printf("%i ", k);
    }
    printf("-> %i\n", v);
  }

  seqmap_free(sm);
  return 0;
}
