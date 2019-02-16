/*
** seqkey.c | Circa Examples | Sequence-Keyed Maps Example
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Seq(int) xs;
  
  ce_critical
    xs = seq_alloc(int, 1);
  
  for (int i = 0; i < 3; i++)
    seq_set_lit(xs, i, i + 1);

  SeqMap(int, int) xss;
  
  ce_critical
    xss = seqmap_alloc(int, int, 1);
 
  seqmap_set_lit(xss, xs, 1);
  
  for (int i = 0; i < 3; i++)
    seq_set(xs, i, i);

  seqmap_set_lit(xss, xs, 2);

  printf("%i\n", seqmap_get(xss, xs));

  seqmap_free(xss);
  seq_free(xs);

  return 0;
}
