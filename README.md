# The Circa Library Set

Circa is a set of libraries for GNU or ISO C99 and up
that grants the programmer a variety of generic, dynamic
datatypes with a focus on speed and small binaries.

- [Documentation](doc/README.md)

## Some Examples

### Sequences

A program that pushes the numbers `0` to `9` onto a
sequence as though it were a stack, then uses the `foreach`
construct to iterate through them and print them out.

```C
#include <stdio.h>
#include <circa.h>

int main() {
  Seq(int) xs = seq_alloc(int, 1);
  
  for (int i = 0; i < 10; i++)
    seq_push(xs, i);

  seq_foreach(xs, x)
    printf("%i\n", x);
  
  seq_free(xs);
  
  return 0;
}
```

### Text

A file `this.c` reads itself and prints itself out.

```C
#include <stdio.h>
#include <circa.h>

int main() {
  Txt buf = txt_alloc(1);
  
  {
    FILE *self = fopen("this.c", "r");
    txt_read(buf, self);
    fclose(self);
  }
  
  printf("%s", buf);

  txt_free(buf);

  return 0;
}
```
