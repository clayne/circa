/*
** slice.c | Circa Examples | Text slicing.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Txt t = txt_lit("Hello, World!");
  Txt s = txt_alloc(1);
  txt_cpy_slice(s, t, slice_lit(0, 4));
  puts(s);
  txt_free(t);
  txt_free(s);
  return 0;
}
