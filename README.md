# Circa
## A core library for C99 and up.
Circa is a library written in the C99 programming language
taking advantage of macros and fat pointers in order to add
dynamic memory arenas, sequences, and strings to C without
the use of any compiler extensions.

# Examples

Summing Numbers
```C
#include <stdio.h>
#include <circa.h>

void puti(int n) { 
  printf("%i\n", n); 
}

int add(int a, int b) {
  return a + b; 
}

int main() {
  Seq(int) ns = seq_lit(int, 1, 2, 3, 4);
  seq_do(int, ns, puti);
  int n = 0;
  seq_reduce(int, ns, add, n);
  puti(n);
  return 0;
}
```

# Installation
Circa isn't ready for installation yet, sorry! Nor is the library in a particularly complete state.

As soon as everything is set up instructions will be put here.
