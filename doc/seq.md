# The Circa Library Set

## Sequences

Sequences fundamental "dynamic array" type Circa offers,
inspired by the data structures of the same name from Nim.

### Types

#### Externals

```C
#define Seq(T) T*
typedef Seq(void) Seq;
```

Sequences are normally written as `Seq(T)` where `T` is the
type of the data you want to store, though if you're only using
the `_iso` functions or want to write type-independent functions
like Circa itself contains, then you can write `Seq` instead.

Note that as a result of the use of a macro here, you must be careful not to do
this:

```C
Seq(int) a, b;
```

as `b` will be `int` instead of `int*`.

#### Internals

```C
struct seq_data {
  size_t cap;
  size_t len;
  char data[];
};
```

Sequences are very simple internally; they contain a capacity
(which describes the allocated size) and a length (which describes
how much of that allocated space is currently in use.) The data
is not part of the structure's allocated memory, but rather uses
a C99 feature known as FAM (Flexible Array Members.) When the sequence
is allocated, extra memory is tacked on at the end and this is what
your `Seq(T)` actually points to-- to get access to the struct shown above,
you have to use the `seq` function.

### Accessors

#### seq

```C
static inline struct seq_data *seq(Seq s);
```

This function returns a pointer to the internal structure
of a sequence for easy modification. This is the idiomatic way of
obtaining the capacity or length of a sequence for reading or
modification.

```C
Seq(int) xs = seq_alloc(int, 10); // Allocate 10 integers.
printf("%i\n", seq(xs)->cap);     // Print the capacity, in this case 10.
seq_free(xs);                     // Free the sequence's memory.
```

The following errors may be thrown:

- `CE_ARG`: if `s` is NULL. (Debug builds only.)

#### seq_set

```C
Seq seq_set_iso(Type T, Seq(T) s, size_t a, T *v);
Seq seq_set(Seq(T) s, size_t a, T *v);

Seq seq_set_(size_t siz, Seq s, size_t a, void *v);
```

This function sets the value of a sequence at a certain index.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0, `s` is NULL, or `v` is NULL. (Debug builds only.)

#### seq_get

```C
T seq_get_iso(Type T, Seq(T) s, size_t a);
T seq_get(Seq(T) s, size_t a);

void *seq_get_(size_t siz, Seq s, size_t a);
```

This function retrieves the value from a sequence at a specific index.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0 or `s` is NULL. (Debug builds only.)

### Allocators

#### seq_alloc

```C
Seq(T) seq_alloc_iso(Type T, size_t cap);
Seq(T) seq_alloc(Type T, size_t cap);

Seq seq_alloc_(size_t siz, size_t cap);
```

This function allocates a sequence of a given type and capacity.

```C
Seq(int) xs = seq_alloc(int, 10); // Allocate a sequence of 10 integers.
seq_free(xs);                     // Free the memory.
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0 or `cap` is 0. (Debug builds only.)
- `CE_OOM`: if memory allocation fails.

#### seq_from / seq_wrap

```C
Seq(T) seq_from_iso(Type T, Seq(T) s);
Seq(T) seq_from(Seq(T) s);

Seq(T) seq_wrap_iso(Type T, size_t n, T *v);
Seq(T) seq_wrap(size_t n, T *v);

Seq seq_wrap_(size_t siz, size_t n, void *v);
```

`seq_from` is shorthand for allocating a new sequence then
copying the contents of the passed-in sequence to that sequence.

`seq_wrap` allows for easy conversion from a normal C array
to a sequence of a stated capacity.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0, `v` is NULL, or `cap` is 0. (Debug builds only.)
- `CE_OOM`: if the internal call to `seq_alloc_` fails.

#### seq_realloc

```C
void seq_realloc_iso(Type T, Seq(T) s, size_t cap);
void seq_realloc(Seq(T) s, size_t cap);

Seq seq_realloc_(size_t siz, Seq s, size_t cap);
```

Reallocates a sequence to a given capacity.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0, `s` is NULL, or `cap` is 0. (Debug builds only.)
- `CE_OOM`: if memory reallocation fails.

#### seq_require

```C
void seq_require_iso(Type T, Seq(T) s, size_t cap);
void seq_require(Seq(T) s, size_t cap);

Seq seq_require_(size_t siz, Seq s, size_t cap);
```

Shorthand for `cap < seq(s)->cap ? seq_realloc(s, cap) : s`.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0, `s` is NULL, or `cap` is 0. (Debug builds only.)
- `CE_OOM`: if the internal call to `seq_realloc_` fails.

#### seq_free

```C
void seq_free_iso(Type T, Seq(T) s);
void seq_free(Seq(T) s);

Seq seq_free_(Seq s);
```

Frees the memory used by a sequence. Passing in `NULL` is safe, and sequences
passed in via the macros will be made `NULL`; this way double-free is usually
not a concern.

```C
// TODO
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0. (Debug builds only.)
