# Circa
## A core library set for C.

Circa is a library set written for various standards of the C programming
language with the intent of making C less painful to use for common tasks.

# Library Set

| Library         | Description           | Standard | Revision | Dependencies           |
| --------------- | --------------------- | -------- | -------- | ---------------------- |
| circa/core.h    | Core utils & config.  | ISO      | C89      | n/a                    |
| circa/types.h   | Terse type names.     | ISO      | C99      | core                   |
| circa/hash.h    | FNV1a hashing.        | ISO      | C99      | core, types            |
| circa/simd.h**  | Portable SIMD.        | GNU      | C99      | core, types            |
| circa/seq.h     | Dynamic sequences.    | ISO*     | C99      | core                   |
| circa/str.h     | Dynamic strings.      | ISO      | C99      | core, types, hash      |
| circa/dict.h**  | Dynamic dictionaries. | ISO      | C99      | core, types, hash, str |

`* Some features may not be available in ISO C.`
`** Not implemented fully yet. Some features may be usable.`

# Examples

## Types

```C
#include <stdio.h>

#include "circa/types.h"

int main()
{
  i32 i = 123;
  u32 u = (u32) i;
  f32 f = 1.02;
  usize u = sizeof(i);
  printf("%i (%zu bytes)\n", i, u);
  return 0;
}
```

## Sequences

```C
#include "circa/seq.h"

static inline
void puti(int n)
{
  printf("%i\n", n);
}

static inline
bool odd(int n)
{
  return n & 1;
}

int main()
{
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(a, odd, b);
  seq_do(b, puti);
  seq_del(a);
  seq_del(b);
  return 0;
}
```

## Strings

```C
#include <stdio.h>

#include "circa/str.h"

int main()
{
  Str hello = str_lit("Hello, ");
  Str world = str_lit("World!");
  str_cat(hello, world);
  puts(hello);
  str_del(hello);
  str_del(world);
  return 0;
}
```

