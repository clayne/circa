# The Circa Library Set

## `data/hatd.h`

### Description

A data structure with true O(1) snoc, element-sized O(n) space overhead, and
O(1) indexing with a constant 2 levels of indirection.

Sub-blocks are sized by increasing powers of two, which makes indexing
marginally more complicated (it now requires a reverse bit scan, though that's
a single instruction on most x86 computers). On the other hand, the number of
allocations done per element decreases over time, and spatial locality of data
also gets better later into the structure.

To generate the type `hatd_int`, you would do the following:

```C
#define T int
  #include "circa/data/hatd.h"
#undef T
```

### Type Definition

```C
typedef struct {
  T **l;
  T **m;
  T **h;
  size_t m_cap;
  size_t l_len;
  size_t m_len;
  size_t h_len;
  size_t len;
} hatd_T;
```

### Allocation Functions

#### alloc

```C
void hatd_T_alloc(hatd_T *);
```

Allocate a hashed array tree of length zero.

```C
hatd_int h;
hatd_int_alloc(&h); // allocate the heap memory for a HAT.
// ...
hatd_int_free(&h);
```

#### free

```C
void hatd_T_free(hatd_T *);
```

Free the heap memory occupied by a hashed array tree.

```C
hatd_int h;
hatd_int_alloc(&h);
// ... 
hatd_int_free(&h); // free the memory used by the HAT.
```

### Accessor Functions

#### lookup

```C
T *hatd_T_lookup(hatd_T *, size_t);
```

Get the pointer to the element at the given index in a hashed array tree. This
pointer remains valid until the hashed array tree is shrunk, which currently
cannot happen. TODO: Make that possible.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5);
int *x = hatd_int_lookup(&h, 0); // get the pointer to position 0
printf("%i\n", *x); // should print 5
hatd_int_free(&h);
```

#### set_v

```C
void hatd_T_set_v(hatd_T *, size_t, T);
```

Set the value at the given address in a hashed array tree to the input value.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5);
int *x = hatd_int_lookup(&h, 0);
hatd_int_set_v(&h, 0, 10);
printf("%i\n", *x); // should print 10
hatd_int_free(&h);
```

#### set_r

```C
void hatd_T_set_r(hatd_T *, size_t, T *);
```

Like `set_v`, but takes a pointer as input instead to avoid passing by value.
This can be useful if `T` is a large type that would be relatively expensive to
copy multiple times.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5);
int *x = hatd_int_lookup(&h, 0);
hatd_int_set_r(&v, 0, &(int){10});
printf("%i\n", *x); // should print 10
hatd_int_free(&h);
```

#### get_v

```C
T hatd_T_get_v(hatd_T *, size_t);
```

Get the value at the given address in a hashed array tree.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5);
int x = hatd_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatd_int_free(&h);
```

#### get_r

```C
void hatd_T_get_r(hatd_T *, size_t, T*);
```

Like `get_r`, but uses a pointer for the result instead to avoid returning by
value. This can be useful if `T` is a large type that would be relatively
expensive to copy multiple times.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5);
int x;
hatd_int_get_r(&h, &x);
printf("%i\n", x); // should print 5
hatd_int_free(&h);
```

### Stack Functions

#### snoc_v

```C
void hatd_T_snoc_v(hatd_T *, T);
```

Append a value to the end of a hashed array tree.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_v(&h, 5); // append the value 5 to the end of the HAT
int x = hatd_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatd_int_free(&h);
```

#### snoc_r

```C
void hatd_T_snoc_r(hatd_T *, T *);
```

Like `snoc_v`, but uses a pointer for the input to avoid passing by value. This
can be useful if `T` is a large type that would be relatively expensive to copy
multiple times.

```C
hatd_int h;
hatd_int_alloc(&h);
hatd_int_snoc_r(&h, &(int){5}); // append the value 5 to the end of the HAT
int x = hatd_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatd_int_free(&h);
```
