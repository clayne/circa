#include "../circa.h"

int main() {
  Str this = str_readfile("ex/read.c");
  puts(this);
  str_del(this);
  return 0;
}
