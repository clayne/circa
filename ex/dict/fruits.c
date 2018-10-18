/*
** fruits.c | Circa | Insert fruits into a dictionary and print them out.
** https://github.com/davidgarland/circa
*/

#include "../../dict.h"

int main() {
  Dict(int) fruits = dict_new(int, 2);
  //dict_set(fruits, "orange", 5);
  //dict_set(fruits, "apple", 100);
  //dict_set(fruits, "peach", 25);
  dict_set(fruits, "banana", 10);
  dict_set(fruits, "mango", 500);
  dict_set(fruits, "pear", 2);
  dict_foreach(fruits, k, v, {
    printf("%s: %i\n", k, v);
  });
  dict_del(fruits);
  return 0;
}
