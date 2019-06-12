/*
** txt.c | The Circa Library Set | Text example.
** https://github.com/davidgarland/circa
*/

#include <circa.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    puts("Pass files in and this program will act like cat.");
    exit(1);
  }
  
  Txt t = txt_alloc(1);

  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (!fp) {
      txt_fmt(t, "txt.o: %s: No such file\n", argv[i]);
      txt_write(t, stdout);
      exit(1);
    }
    txt_read(t, fp);
    txt_write(t, stdout);
    fclose(fp);
  }

  txt_free(t);

  return 0;
}
