/*
** fruits.c | Intern fruits into a dictionary and print them out.
** https://github.com/davidgarland/circa
*/

#include "../../dict.h"

void dump(Dict(int) fruits) {
  for (size_t i = 0; i < dict(fruits)->cap; i++) {
    char *key = dict(fruits)->buckets[i].key;
    if (dict_has(fruits, key))
      printf("%s: %i\n", key, dict_get(fruits, key));
  }
}

int main() {
  Dict(int) fruits = dict_new(int, 2);
  dict_set(fruits, "orange", 5);
  dict_set(fruits, "apple", 100);
  dict_set(fruits, "peach", 25);
  dict_set(fruits, "banana", 10);
  dict_set(fruits, "mango", 500);
  dict_set(fruits, "pear", 2);
  dump(fruits);
  dict_del(fruits);
  return 0;
}
