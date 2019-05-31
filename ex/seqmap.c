/*
** seqmap.c | The Circa Library Set | SeqMap example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  SeqMap(int, int) sm = seqmap_alloc(int, int, 1);

  Seq(int) xs = seq_alloc(int, 3);
    seq_cpy_lit(xs, 1, 2, 3);
    seqmap_set(sm, xs, &(int){1});
    seq_cpy_lit(xs, 3, 2, 1);
    seqmap_set(sm, xs, &(int){2});
    seq_cpy_lit(xs, 2, 2, 2);
    seqmap_set(sm, xs, &(int){3});
  seq_free(xs);

  seqmap_foreach(sm, ks, v) {
    seq_foreach(ks, k) {
      printf("%i ", k);
    }
    printf("-> %i\n", v);
  }

  seqmap_free(sm);
  return 0;
}
