#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"
#include "../circa_cmp.h"
#include "../circa_imath.h"

#include "../circa_str.h"

int main() {
  str s;
  str_alloc(&s);
  str_free(&s);
  return 0;
}
