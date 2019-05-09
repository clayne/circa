/*
** fmt.c | Circa Examples | String formatting.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Txt t = txt_alloc(1);
  txt_fmt(t, "%i", 5);
  txt_cat_fmt(t, "%i", 5);
  printf("%s\n", t);
  return 0;
}
