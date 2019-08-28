/*
** mapseq.c | The Circa Library Set | MapSeq example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  MapSeq(int, int) ms = mapseq_alloc(int, int, 1);

  Seq(int) xs = seq_alloc(int, 3);
    seq_cpy_lit(xs, 1, 2, 3);
    mapseq_set(ms, &(int){1}, xs);
    seq_cpy_lit(xs, 2, 3, 4);
    mapseq_set(ms, &(int){2}, xs);
    seq_cpy_lit(xs, 3, 4, 5);
    mapseq_set(ms, &(int){3}, xs);
  seq_free(xs);

  mapseq_foreach(ms, k, vs) {
    printf("%i -> ", k);
    seq_foreach(vs, v) {
      printf("%i ", v);
    }
    puts("");
  }

  mapseq_free(ms);

  return 0;
}
