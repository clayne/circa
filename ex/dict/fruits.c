/*
** fruits.c | Circa Examples | Counting fruits. Hooray.
** https://github.com/davidgarland/circa
*/

#include "../../circa.h"

int main() {
  Dict(int) d = dict_alloc(int, 1);
  
  dict_set(d, "orange", &(int){5});
  dict_set(d, "apple", &(int){2});

  printf("%s: %i\n", "orange", dict_get(d, "orange"));
  printf("%s: %i\n", "apple", dict_get(d, "apple"));

  dict_free(d);

  return 0;
}
