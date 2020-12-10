# The Circa Library Set

## `data/hatc.h`

### Description

A data structure with true O(1) snoc, pointer-sized O(n) space overhead, and
O(1) indexing with a constant 2 levels of indirection.

Sub-blocks are given a fixed power of two as a size, which makes indexing
more efficient than `hatd` but still less efficient than `vec`; it only
requires a bit shift and a bitwise AND to be done. The constant size means
spatial locality of data should be the same regardless of size, and in practice
though both are "O(n)" this data structure will always waste less space than
`hatd` as the number of elements tends towards infinity, provided the size of
the type is larger than pointer-sized.

For instance, to generate the type `hatc_int` and specify `8` (`2^3`) as the
sub-block size, you would do the following:

```C
#define T int
#define C 3
  #include "circa/data/hatc.h"
#undef C
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
} hatc_T;
```

### Allocation Functions

#### alloc

```C
void hatc_T_alloc(hatc_T *);
```

Allocate a hashed array tree of length zero.

```C
hatc_int h;
hatc_int_alloc(&h); // allocate the heap memory for a HAT.
// ...
hatc_int_free(&h);
```

#### free

```C
void hatc_T_free(hatc_T *);
```

Free the heap memory occupied by a hashed array tree.

```C
hatc_int h;
hatc_int_alloc(&h);
// ... 
hatc_int_free(&h); // free the memory used by the HAT.
```

### Accessor Functions

#### lookup

```C
T *hatc_T_lookup(hatc_T *, size_t);
```

Get the pointer to the element at the given index in a hashed array tree. This
pointer remains valid until the hashed array tree is shrunk, which currently
cannot happen. TODO: Make that possible.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5);
int *x = hatc_int_lookup(&h, 0); // get the pointer to position 0
printf("%i\n", *x); // should print 5
hatc_int_free(&h);
```

#### set_v

```C
void hatc_T_set_v(hatc_T *, size_t, T);
```

Set the value at the given address in a hashed array tree to the input value.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5);
int *x = hatc_int_lookup(&h, 0);
hatc_int_set_v(&h, 0, 10);
printf("%i\n", *x); // should print 10
hatc_int_free(&h);
```

#### set_r

```C
void hatc_T_set_r(hatc_T *, size_t, T *);
```

Like `set_v`, but takes a pointer as input instead to avoid passing by value.
This can be useful if `T` is a large type that would be relatively expensive to
copy multiple times.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5);
int *x = hatc_int_lookup(&h, 0);
hatc_int_set_r(&v, 0, &(int){10});
printf("%i\n", *x); // should print 10
hatc_int_free(&h);
```

#### get_v

```C
T hatc_T_get_v(hatc_T *, size_t);
```

Get the value at the given address in a hashed array tree.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5);
int x = hatc_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatc_int_free(&h);
```

#### get_r

```C
void hatc_T_get_r(hatc_T *, size_t, T*);
```

Like `get_r`, but uses a pointer for the result instead to avoid returning by
value. This can be useful if `T` is a large type that would be relatively
expensive to copy multiple times.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5);
int x;
hatc_int_get_r(&h, &x);
printf("%i\n", x); // should print 5
hatc_int_free(&h);
```

### Stack Functions

#### snoc_v

```C
void hatc_T_snoc_v(hatc_T *, T);
```

Append a value to the end of a hashed array tree.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_v(&h, 5); // append the value 5 to the end of the HAT
int x = hatc_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatc_int_free(&h);
```

#### snoc_r

```C
void hatc_T_snoc_r(hatc_T *, T *);
```

Like `snoc_v`, but uses a pointer for the input to avoid passing by value. This
can be useful if `T` is a large type that would be relatively expensive to copy
multiple times.

```C
hatc_int h;
hatc_int_alloc(&h);
hatc_int_snoc_r(&h, &(int){5}); // append the value 5 to the end of the HAT
int x = hatc_int_get_v(&h, 0);
printf("%i\n", x); // should print 5
hatc_int_free(&h);
```
