/*
** buck.c | The Circa Library Set | Buckets example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Buck b = buck_alloc(int, 10, 100);

  for (int i = 0; i < 5; i++)
    buck_push(int, &b, &i);

  printf("len: %zu\n", b.len);

  buck_foreach(int, &b, n, {
    printf("%i\n", n);
  });

  buck_free(int, &b);

  return 0;
}
