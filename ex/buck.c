/*
** buck.c | The Circa Library Set | Buckets example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Buck b = buck_alloc(int, 10, 100);

  for (int i = 0; i < 2000; i++)
    buck_push(int, &b, &i);

  for (int i = 0; i < 2000; i++)
    printf("%i\n", buck_get(int, &b, i));

  buck_free(int, &b);

  return 0;
}
