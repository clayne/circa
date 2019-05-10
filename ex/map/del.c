/*
** del.c | Circa Examples | Map deletion.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Map(int, int) m = map_alloc(int, int, 5);

  map_set(m, &(int){5}, &(int){7});
  printf("len before deletion = %zu\n", map(m)->len);
  map_foreach(m, k, v)
    printf("[%i] = %i\n", k, v);
  
  map_del(m, &(int){5});
  printf("len after deletion = %zu\n", map(m)->len);
  map_foreach(m, k, v)
    printf("[%i] = %i\n", k, v);

  map_free(m);

  return 0;
}
