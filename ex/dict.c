/*
** dict.c | The Circa Library Set | Dict example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Dict(int) fruits = dict_alloc(int, 1);

  dict_set(fruits, "oranges", &(int){7});
  dict_set(fruits, "peaches", &(int){5});
  dict_set(fruits, "apples",  &(int){10});

  dict_free(fruits);

  return 0;
}
