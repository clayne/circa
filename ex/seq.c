/*
** seq.c | The Circa Library Set | Sequences example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Seq(int) xs = seq_alloc(int, 10);
  
  for (int i = 0; i < 10; i++)
    seq_push(xs, &i);

  seq_filter(xs, it & 1); // Keep only the odd numbers.

  seq_map(xs, it * it); // Square every number.

  seq_foreach(xs, x)
    printf("%i\n", x);
 
  seq_free(xs);

  return 0;
}
