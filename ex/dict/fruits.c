/*
** fruits.c | Circa Examples | Store/print various fruits. 
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Dict(int) fruits;

  circa_critical
    fruits = dict_alloc(int, 1);
  
  circa_debug {
    dict_set_lit(fruits, "orange", 5);
    dict_set_lit(fruits, "apple", 10);
    dict_set_lit(fruits, "banana", 13);
  }

  circa_debug {
    printf("oranges: %i\n", dict_get(fruits, "orange"));
    printf("apples: %i\n", dict_get(fruits, "apple"));
    printf("bananas: %i\n", dict_get(fruits, "banana"));
  }
  
  dict_free(fruits);
  
  return 0;
}
