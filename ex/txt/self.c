/*
** self.c | Circa Example Programs | A program that reads and prints itself.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main() {
  Txt buffer = txt_alloc(1);
  FILE *self = fopen("ex/txt/self.c", "r");
  txt_read(buffer, self);
  fclose(self);
  printf("%s", buffer);
  txt_free(buffer);
  return 0;
}
