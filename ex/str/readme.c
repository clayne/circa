/*
** readme.c | Circa | Prints out the README.md in the current directory.
** https://github.com/davidgarland/circa
*/

#include "../../str.h"

int main() {
  FILE *fp = fopen("README.md", "r");
  if (fp != NULL) {
    Str readme = str_new(1);
    str_read(readme, fp);
    printf("%s", readme);
    str_del(readme);
    fclose(fp);
  }
  return 0;
}
