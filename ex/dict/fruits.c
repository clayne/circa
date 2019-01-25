/*
** fruits.c | Circa Examples | Store/print various fruits. 
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Dict(int) fruits;

  circa_critical
    fruits = dict_alloc(int, 1);
  
  dict_set_lit(fruits, "orange", 5);
  dict_set_lit(fruits, "apple", 10);
  dict_set_lit(fruits, "banana", 13);

  dict_foreach(fruits, k, v) {
    printf("%s(s): %i\n", k, v);
  }
  
  dict_free(fruits);
  
  return 0;
}
