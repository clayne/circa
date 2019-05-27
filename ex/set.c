/*
** set.c | The Circa Library Set | Sets example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Set(int) e = set_alloc(int, 1);

  for (int i = 0; i < 10; i++)
    set_add(e, &i);

  set_foreach(e, n)
    printf("%i\n", n);

  set_free(e);
  set_free(e);
  
  return 0;
}
