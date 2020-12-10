# The Circa Library Set

## `data/vec.h`

### Description

An implementation of dynamic vectors with an amortized growth rate of 1.5. This
is analogous to C++ `std::vector`, Nim `seq`, Java `ArrayList`, et cetera.

To generate the type `vec_int`, you would do the following:

```C
#define T int
  #include "circa/data/vec.h"
#undef T
```

### Type Definition

```C
typedef struct {
  size_t cap;
  size_t len;
  T *data;
} vec_T;
```

### Allocation Functions

#### alloc

```C
void vec_T_alloc(vec_T *);
```

Allocate the heap memory required for a vector.

```C
vec_int v;
vec_int_alloc(&v); // allocate the memory required for v
// ...
vec_int_free(&v);
```

### TODO
