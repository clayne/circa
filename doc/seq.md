# Circa Docs : Sequences

Sequences (`Seq(T)`) are Circa's implementation of dynamic arrays. The main goal
is to reduce the amount of boilerplate required in a given C library. Instead of
passing around a bunch of capacity and length variables and needing to write
your own functions for doing trivial operations on the arrays, sequences do all
of that for you.

## Structure

```C
struct seq_data {
  size_t cap;  // The capacity of the sequence.
  size_t len;  // The length of the sequence.
  char data[]; // The raw, untyped elements.
};
```

Here are the user-facing fields of the `Seq(T)` type. The `data` field is the one
pointed to by `Seq(T)`, and getting a pointer to the structure detailed above
can be done with the `seq()` function.

## Accessors

### seq

```C
struct seq_data *seq(Seq);
struct seq_data *seq_(Seq s, circa_msg fname, circa_msg line);
```

Access the core structure of a dictionary.

```C
Seq(int) a = seq_new(int, 1); // Allocate a new sequence of 1 integer.
seq_push(a, 500);             // Push the value 500 onto the sequence.
seq_push(a, 250);             // Push the value 250 onto the sequence.
printf("%zu\n", seq(a)->len); // Check the length of the sequence. (2)
```

### seq_set

```C
void seq_set_iso(<type>, Seq, size_t addr, <value>);
void seq_set(Seq, size_t addr, <value>);
void seq_set_lit_iso(<type>, Seq, size_t addr, <literal value>);
void seq_set_lit(Seq, size_t addr, <literal value>);
Seq seq_set_(size_t siz, Seq s, size_t addr, void *val, circa_msg fname, circa_msg line);
```

Sets a sequence's value at a given index.

```C
Seq(int) a = seq_new(int, 1);  // Allocate a new sequence of 1 integer.
seq_set_lit(a, 0, 1);          // Set the value at index 0 to the literal 1.
printf("%i\n", seq_get(a, 0)); // Print out the value at index 0. (1)
```

### seq_set_ext

```C
void seq_set_ext_iso(<type>, Seq, size_t addr, <value>, size_t ext);
void seq_set_ext(Seq, size_t addr, <value>, size_t ext);
void seq_set_lit_ext_iso(<type>, Seq, size_t addr, <literal value>, size_t ext);
void seq_set_lit_ext(Seq, size_t addr, <literal value>, size_t ext);
Seq seq_set_ext_(size_t siz, Seq s,s ize_t addr, void val, size_t ext, circa_msg fname, circa_msg line);
```

Sets a sequence's value at a given index.

If the index is out of bounds and the sequence must be reallocated to accomodate
the new value, then `ext` more elements will be allocated than usual. This can
be useful for avoiding reallocation too frequently.

```C
Seq(int) a = seq_new(int, 1); // Allocate a new sequence of 1 integer.
seq_set_ext_lit(a, 1, 2, 5);  // Set the value at the index 1 to 2 with an ext of 5.
printf("%zu\n", seq(a)->cap); // Check the capacity. (7; needed 2, ext 5.)
```

### seq_get

```C
<value> seq_get_iso(<type>, Seq, size_t addr);
<value> seq_get(Seq, size_t addr);
void *seq_get_(size_t siz, Seq s, size_t addr, circa_msg fname, circa_msg line);
```

Gets a sequence's value at a given index.

```C
Seq(int) a = seq_new(int, 1);  // Allocate a new sequence of 1 integer.
seq_set_lit(a, 0, 1);          // Set the value at the index 0 to 1.
printf("%i\n", seq_get(a, 0)); // Print the value at the index 0. (1)
```

## Allocators

### seq_new

```C
Seq seq_new_iso(<type>, size_t cap);
Seq seq_new(<type>, size_t cap);
Seq seq_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line);
```

Allocates a new sequence of a given type and capacity.

The `_iso` variant is the exact same as the GNU variant; it is provided only
as a stylistic convenience.

```C
Seq(int) a = seq_new(int, 10); // Allocate a new sequence of 10 integers.
```

### seq_lit

```C
Seq seq_lit_iso(<type>, ...)
Seq seq_lit(<type>, ...)
Seq seq_lit_(size_t siz, size_t len, void *lits, circa_msg fname, circa_msg line);
```

Allocates a new sequence from a literal initializer list.

The `_iso` variant is the exact same as the GNU variant; it is provided only
as a stylistic convenience.

```C
Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5); // Allocate a sequence holding the values (1, 2, 3, 4, 5).
for (size_t i = 0; i < 5; i++)
  printf("%i\n", seq_get(a, i));          // Print out each element. (1, 2, 3, 4, 5)
```

### seq_from

```C
Seq seq_from_iso(<type>, Seq);
Seq seq_from(Seq);
Seq seq_from_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Allocates a new sequence, copying the contents of another sequence. The new
sequence's capacity will be equal to the other sequence's length.

```C
Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5); // Allocate a sequence holding the values (1, 2, 3, 4, 5).
Seq(int) b = seq_from(a);                 // Make a new sequence from the first.
printf("%i\n", seq_get(b, 2));            // Print out the value at index 2. (3)
```

### seq_wrap

```C
Seq seq_wrap_iso(<type>, <ptr>, size_t len);
Seq seq_wrap(<ptr>, size_t len);
Seq seq_wrap_(size_t siz, void *ptr, size_t len, circa_msg fname, circa_msg line);
```

"Wraps" a normal C array, allocating a new sequence to hold its contents.

```C
int *a = calloc(10, sizeof(int)); // Allocate an array of 10 integers.
a[5] = 2;                         // Set the value at index 5 to 2.
Seq(int) b = seq_wrap(a, 10);     // Allocate a sequence holding the values in the array.
printf("%i\n", seq_get(b, 5));    // Print the value at index 5. (2)
```

### seq_rsz

```C
void seq_rsz_iso(<type>, Seq, size_t cap);
void seq_rsz(Seq, size_t cap);
Seq seq_rsz_(size-t siz, Seq s, size_t cap, circa_msg fname, circa_msg line);
```

Resizes a sequence. For less granular shrinking `seq_shr` would be reccomended,
and if you need only to gurantee that a sequence can hold up to a specific
capacity, `seq_rqr` would be reccomended to avoid unnecessary reallocations.
Nonetheless, `seq_rsz` is provided for the few cases where it might be
needed.

```C
Seq(int) a = seq_new(int, 5); // Allocate a sequence of 5 integers.
seq_rsz(a, 10);               // Resize the sequence from 5 elements to 10.
seq_rsz(a, 2);                // Resize the sequence from 10 elements to 2.
printf("%zu\n", seq(s)->cap); // Print out the capacity of the sequence. (2)
```

### seq_rqr

```C
void seq_rqr_iso(<type>, Seq, size_t cap);
void seq_rqr(Seq, size_t cap);
Seq seq_rqr_(size_t siz, Seq s, size_t cap, circa_msg fname, circa_msg line);
```

Require that a sequence be able to hold at least a given length.
Useful for avoiding unnecessary reallocations compared to `seq_rsz`.

```C
Seq(int) a = seq_new(int, 5); // Allocate a sequence of 5 integers.
seq_rqr(a, 2);                // Require that the sequence be able to hold 2 integers.
printf("%zu\n", seq(a)->cap); // Print out the sequence's capacity. (5)
seq_rqr(a, 10);               // Require that the sequence be able to hold 10 integers.
printf("%zu\n", seq(a)->cap); // Print out the sequence's capacity. (10)
```

### seq_shr

```C
void seq_shr_iso(<type>, Seq);
void seq_shr(Seq);
Seq seq_shr_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Shrinks a sequence's capacity to be equal to its length. This is the simplest
way to save off memory wastage. For more granular shrinking, you may want to use
`seq_rsz`.

In the event that the sequence's length is 0, the capacity will be set to 1.

```C
Seq(int) a = seq_new(int, 100); // Allocate a new sequence of 100 integers.
seq_push(a, 5); // Push the value 5 onto the sequence.
seq_shr(a); // Shrink the sequence to its length.
printf("%zu\n")
```

### seq_del

```C
void seq_del_iso(<type>, Seq);
void seq_del(Seq);
Seq seq_del_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Deletes a sequence.

```C
Seq(int) a = seq_new(int, 1); // Allocate a new sequence of 1 integer.
seq_del(a);                   // Delete the sequence.
seq_del(a);                   // (Subsequent deletions will not error.)
if (a == NULL)
  puts("a is NULL.");         // ((Also, the sequence is set to NULL after deletion.))
```

## Stack Operations

### seq_push

```C
void seq_push_iso(<type>, Seq, <value>);
void seq_push(Seq, <value>);
void seq_push_lit_iso(<type>, Seq, <literal value>);
void seq_push_lit(Seq, <literal value>);
Seq seq_push_(size_t siz, Seq s, void *val, circa_msg fname, circa_msg line);
```

Performs a stack-like "push" operation.

Equivalent to doing `seq_set(s, seq(s)->len, value)`.

```C
Seq(int) a = seq_new(int, 1);  // Allocates a sequence of 1 integer. 
seq_push(a, 5);                // Pushes the value 5 onto the sequence.
printf("%i\n", seq_get(a, 0)); // Gets the value at index 0. (5)
```

### seq_push_ext

```C
void seq_push_ext_iso(<type>, Seq, <value>, size_t ext);
void seq_push_ext(Seq, <value>, size_t ext);
void seq_push_lit_ext_iso(<type>, Seq, <value>, size_t ext);
void seq_push_lit_ext(Seq, <value>, size_t ext);
```

Performs a stack-like push operation.

Equivalent to doing `seq_set_ext(s, seq(s)->len, value, ext)`.

```C
Seq(int) a = seq_new(int, 1); // Allocates a sequence of 1 integer.
seq_push(a, 5);               // Pushes the value 5 onto the sequence.
seq_push_ext(a, 10, 5);       // Pushes the value 10 onto the sequence with an ext of 5.
printf("%zu\n", seq(a)->cap); // Print the capacity of the sequence. (7; needed 2, ext 5.)
```

### seq_tos

```C
<value> seq_tos_iso(<type>, Seq);
<value> seq_tos(Seq);
void *seq_tos_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Performs a stack-like "top-of-stack" (hence `tos`) operation.

Equivalent to doing `seq_get(s, seq(s)->len - 1)`.

```C
Seq(int) a = seq_new(int, 1); // Allocates a sequence of 1 integer.
seq_push(a, 5);               // Pushes the value 5 onto the sequence.
printf("%i\n", seq_tos(a));   // Print the top of the sequence. (5)
```

### seq_pop

```C
<value> seq_pop_iso(<type>, Seq);
<value> seq_pop(Seq);
void *seq_pop_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Performs a stack-like "pop" operation.

Equivalent to doing `seq_get(s, seq(s)->len - 1)` followed by `seq(s)->len--`.

```C
Seq(int) a = seq_new(int, 5); // Allocate a new sequence of 5 integers.
seq_push(a, 5);               // Push the value 5 onto the sequence.
printf("%zu\n", seq(a)->len); // Print the length of the sequence. (1)
printf("%i\n", seq_pop(a));   // Print and pop the top of the sequence. (5)
printf("%zu\n", seq(a)->len); // Print the length of the sequence. (0)
```

### seq_dup

```C
void seq_dup_iso(<type>, Seq);
void seq_dup(Seq);
Seq seq_dup_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Peforms a stack-like "duplicate" (hence `dup`) operation.

Equivalent to doing `seq_push(seq_tos(s))`.

```C
Seq(int) a = seq_new(int, 2);  // Allocate a new sequence of 2 integers.
seq_push(a, 5);                // Push the value 5 onto the sequence.
seq_dup(a);                    // Duplicate the top of the sequence.
printf("%i\n", seq_get(a, 0)); // Get the value at index 0. (5)
printf("%i\n", seq_get(a, 1)); // Get the value at index 1. (5)
```

## Sequence Operations

### seq_cpy

```C
void seq_cpy_iso(<type>, Seq dst, Seq src);
void seq_cpy(Seq dst, Seq src);
Seq seq_cpy_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line);
```

Copies the values of `src` into `dst`.

```C
Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5); // Allocate a sequence holding the values (1, 2, 3, 4, 5).
Seq(int) b = seq_new(int, 1);             // Allocate a new sequence of 1 integer.
seq_cpy(b, a);                            // Copy the values of the old sequence into the new one.
for (size_t i = 0; i < 5; i++)
  printf("%i\n", seq_get(b, i));          // Print out each value. (1, 2, 3, 4, 5)
```

### seq_cat

```C
void seq_cat_iso(<type>, Seq dst, Seq src);
void seq_cat(Seq dst, Seq src);
Seq seq_cat_(size_t siz, Seq dst, Seq src, circa_msg fname, circa_msg line);
```

Catenate the values of `src` onto `dst`.

```C
Seq(int) a = seq_lit(int, 1, 2, 3); // Allocate a sequence holding the values (1, 2, 3).
Seq(int) b = seq_lit(int, 4, 5, 6); // Allocate a sequence holding the values (4, 5, 6).
seq_cat(a, b);                      // Catenate the new sequence to the old one.
for (size_t i = 0; i < 6; i++)
  printf("%i\n", seq_get(a, i));    // Print out each value. (1, 2, 3, 4, 5, 6)
```

### seq_rvs

```C
void seq_rvs_iso(<type>, Seq);
void seq_rvs(Seq);
Seq seq_rvs_(size_t siz, Seq s, circa_msg fname, circa_msg line);
```

Reverse a sequence.

```C
Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5); // Allocate a sequence holding the values (1, 2, 3, 4, 5).
seq_rvs(a);                               // Reverse the sequence.
for (size_t i = 0; i < 5; i++)
  printf("%i\n", seq_get(a, i));          // Print out each value. (5, 4, 3, 2, 1)
```
