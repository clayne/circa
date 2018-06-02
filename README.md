# Circa
## A core library for C99 and up.
Circa is a library written in the C99 programming language
taking advantage of macros and fat pointers in order to add
dynamic memory arenas, sequences, and strings to C without
the use of any compiler extensions.

# Examples

```C
// sum.c

#include "../circa.h"

void puti(int n) { printf("%i\n", n); }

int add(int a, int b) { return a + b; }

int main() {
  Seq(int) ns = seq_lit(int, 1, 2, 3, 4);
  seq_do(int, ns, puti);
  int n = 0;
  seq_reduce(int, ns, add, n);
  puts("Sum:");
  puti(n);
  seq_del(int, ns);
  return 0;
}
```
```C
// odd.c

#include "../circa.h"

void puti(int n) { printf("%i\n", n); }

bool odd(int n) { return n & 1; }

int main() {
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(int, a, odd, b);
  seq_do(int, b, puti);
  seq_del(int, a);
  seq_del(int, b);
  return 0;
}
```

# Installation
Circa isn't ready for installation yet, sorry! Nor is the library in a particularly complete state.

As soon as everything is set up instructions will be put here.
