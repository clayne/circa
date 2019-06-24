/*
** map.c | The Circa Library Set | Maps example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Map(int, int) m = map_alloc(int, int, 1);

  map_set(m, &(int){0}, &(int){1});
  map_set(m, &(int){1}, &(int){2});
  map_set(m, &(int){2}, &(int){0});

  for (int i = 0; i < 10; i++)
    map_set(m, &i, &(int){5});

  map_foreach(m, k, v)
    printf("%i -> %i\n", k, v);

  map_free(m);

  return 0;
}
