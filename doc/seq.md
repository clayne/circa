# The Circa Library Set

## Sequences

Sequences are the fundamental "dynamic array" type Circa offers,
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
typedef struct {
  size_t cap; // Space allocated.
  size_t len; // Space used.
  char data[];
} SeqData;
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
static inline SeqData *seq(Seq s);
```

This function returns a pointer to the internal structure
of a sequence for easy modification. This is the idiomatic way of
obtaining the capacity or length of a sequence.

```C
Seq(int) xs = seq_alloc(int, 10); // Allocate 10 integers.
printf("%i\n", seq(xs)->cap);     // Print the capacity, in this case 10.
seq_free(xs);                     // Free the sequence's memory.
```

The following errors may be thrown:

- `CE_ARG`: if `s` is NULL. (Debug builds only.)

#### seq_set

```C
void seq_set_iso(Type T, Seq(T) s, size_t a, T *v);
void seq_set(Seq(T) s, size_t a, T *v);

Seq seq_set_(size_t siz, Seq s, size_t a, void *v);
```

This function sets the value of a sequence at a certain index.

```C
Seq(int) xs = seq_alloc(int, 5);
for (int i = 0; i < 5; i++)
  seq_set(xs, i, &i); // Analogous to xs[i] = i, but with book-keeping.
seq_free(xs);
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0, `s` is NULL, or `v` is NULL. (Debug builds only.)

#### seq_get

```C
T seq_get_iso(Type T, Seq(T) s, size_t a);
T seq_get(Seq(T) s, size_t a);

void *seq_get_(size_t siz, Seq s, size_t a);
```

This function retrieves the value from a sequence at a specific index. Note that
this is not entirely necessary for anything other than nice debug info; it's
plenty safe to just index directly as long as you're using a properly denoted
sequence (that is, its type is `Seq(int)` for example, rather than `Seq`.)

```C
Seq(int) xs = seq_alloc(int, 5);
for (int i = 0; i < 5; i++)
  seq_set(xs, i, &i);
for (int i = 0; i < 5; i++)
  printf("%i\n", seq_get(xs, i)); // Analogous to xs[i].
seq_free(xs);
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
int *xs = malloc(10 * sizeof(int));

for (int i = 0; i < 10; i++)
  xs[i] = i;

Seq(int) ys = seq_wrap(10, xs);

Seq(int) zs = seq_from(ys);

for (int i = 0; i < 10; i++)
  printf("%i\n", seq_get(zs, i));

seq_free(zs);
seq_free(ys);
free(xs);
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
Seq(int) xs = seq_alloc(int, 5); // Allocate a sequence of 5 elements.
printf("%zu\n", seq(xs)->cap); // 5
seq_realloc(xs, 10); // Reallocate the sequence to 10 elements.
printf("%zu\n", seq(xs)->cap); // 10
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

Shorthand for `cap < seq(s)->cap ? seq_realloc_(siz, s, cap + CIRCA_SEQ_PREALLOC) : s`.

```C
Seq(int) xs = seq_alloc(int, 5); // Allocate a sequence of 5 elements.
printf("%zu\n", seq(xs)->cap); // 5
seq_require(xs, 3); // Require that the sequence has at least 3 elements.
printf("%zu\n", seq(xs)->cap); // 5
seq_require(xs, 7); // Require that the sequence has at least 7 elements.
printf("%zu\n", seq(xs)->cap); // 7 + CIRCA_SEQ_PREALLOC
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
Seq(int) xs = seq_alloc(int, 10); // Allocate a sequence of 10 integers.
seq_free(xs); // Free the memory used by the sequence.
```

The following errors may be thrown:

- `CE_ARG`: if `siz` is 0. (Debug builds only.)
