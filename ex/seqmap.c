/*
** seqmap.c | The Circa Library Set | SeqMap example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  SeqMap(int, int) sm = seqmap_alloc(int, int, 1);

  for (intmax_t i = 0; i < seqmap(sm)->cap; i++)
    printf("%i\n", seqmap(sm)->probe[i]);

  Seq(int) s = seq_lit(1, 2, 3);
  seqmap_set(sm, s, &(int){5});
  seq_free(s);
  
  seqmap_free(sm);
  return 0;
}
