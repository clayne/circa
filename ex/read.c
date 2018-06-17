#include "../circa.h"

int main() {
  Str this = str_new(1);
  str_readfile(this, "ex/read.c");
  puts(this);
  str_del(this);
  return 0;
}
