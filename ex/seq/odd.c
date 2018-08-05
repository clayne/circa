/*
** ex/seq/odd.c | Print the odd numbers in 1-10.
** https://github.com/davidgarland/circa
*/

#include "../../seq.h"

static inline
void puti(int n)
{
  printf("%i\n", n);
}

static inline
bool odd(int n)
{
  return n & 1;
}

int main()
{
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(a, odd, b);
  seq_do(b, puti);
  seq_del(a);
  seq_del(b);
  return 0;
}
