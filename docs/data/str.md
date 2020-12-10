# The Circa Library Set

## `data/str.h`

### Description

An implementation of a dynamic string type, comparable to `char*`. As it stands
there are no special UTF-8 helper functions or anything; it's assumed either
that you're using ASCII or will provide your own operations for this. This may
change in the future.

To use the string library, simply import it and the required dependencies:

```C
#include "circa/data/str.h"
```

### Type Definition

```C
typedef struct {
  size_t cap;
  size_t len;
  char *data;
} str;
```

### Allocation Functions

#### alloc

```C
void str_alloc(str *);
```

Allocate the heap memory required for a string.

```C
str s;
str_alloc(&s); // allocate the heap memory for s
// ...
str_free(&s);
```

#### free

```C
void str_free(str *);
```

Free the heap memory required for a string.

```C
str s;
str_alloc(&s);
// ...
str_free(&s); // free the heap memory used by s
```

### TODO
