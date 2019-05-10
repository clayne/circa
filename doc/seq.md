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

#### seq_clear

```C
void seq_clear_iso(Type T, Seq(T) s);
void seq_clear(Seq(T) s);

void seq_clear_(size_t siz, Seq s);
```

This function clears a sequence.

```C
// TODO
```

#### seq_set

```C
void seq_set_iso(Type T, Seq(T) s, size_t a, T *v);
void seq_set(Seq(T) s, size_t a, T *v);

Seq seq_set_(size_t siz, Seq s, size_t a, void *v);
```

This function sets the value of a sequence at a certain index.

```C
// TODO
```

#### seq_has

```C
bool seq_has_iso(Type T, Seq(T) s, size_t a);
bool seq_has(Seq(T) s, size_t a);

bool seq_has_(size_t siz, Seq s, size_t a);
```

This function tests if a sequence has an address within its capacity.

```C
// TODO
```

#### seq_get

```C
T seq_get_iso(Type T, Seq(T) s, size_t a);
T seq_get(Seq(T) s, size_t a);

void *seq_get_(size_t siz, Seq s, size_t a);
```

This function retrieves the value from a sequence at a specific index.

If using the macro variants, error handling can only safely be done via
use of `seq_has` beforehand. (The macros both dereference the pointer
upon receiving it, so they will likely segfault if an error occurs.)

If using the internal definition, an out of bounds index will throw
`CE_OOB`.

```C
// TODO
```

### Allocators

#### seq_alloc

```C
Seq(T) seq_alloc_iso(Type T, size_t cap);
Seq(T) seq_alloc(Type T, size_t cap);

Seq seq_alloc_(size_t siz, size_t cap);
```

This function allocates a sequence of a given type and capacity.

If malloc returns `NULL`, `CE_OOM` wil be thrown.

```C
Seq(int) xs = seq_alloc(int, 10); // Allocate a sequence of 10 integers.
seq_free(xs);                     // Free the memory.
```

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

If the internal `seq_alloc` fails, `CE_OOM` will be thrown.

```C
// TODO
```

#### seq_realloc

```C
void seq_realloc_iso(Type T, Seq(T) s, size_t cap);
void seq_realloc(Seq(T) s, size_t cap);

Seq seq_realloc_(size_t siz, Seq s, size_t cap);
```

Reallocates a sequence to a given capacity.

If realloc returns `NULL`, `CE_OOM` will be thrown.

```C
// TODO
```

#### seq_require

```C
void seq_require_iso(Type T, Seq(T) s, size_t cap);
void seq_require(Seq(T) s, size_t cap);

Seq seq_require_(size_t siz, Seq s, size_t cap);
```

Shorthand for `cap < seq(s)->cap ? seq_realloc(s, cap) : s`.

If the internal `seq_realloc` fails, `CE_OOM` will be thrown.

#### seq_free

```C
void seq_free_iso(Type T, Seq(T) s);
void seq_free(Seq(T) s);

Seq seq_free_(Seq s);
```

Frees the memory used by a sequence.

```C
Seq(int) xs = seq_alloc(int, 10); // Allocate a sequence of 10 integers.
seq_free(xs);                     // Free the sequence.
```

### Sequence Functions

#### seq_cpy

```C
void seq_cpy_iso(Type T, Seq(T) a, Seq(T) b);
void seq_cpy(Seq(T) a, Seq(T) b);

Seq seq_cpy_(size_t siz, Seq a, Seq b);
```

Copies the contents of sequence `b` into sequence `a`.

If the internal `seq_require` fails, `CE_OOM` will be thrown.

```C
Seq(int) as = seq_alloc(int, 10);
for (int i = 0; i < 10; i++)
  seq_push(as, &i);

Seq(int) bs = seq_alloc(int, 1);
seq_cpy(bs, as);

seq_foreach(bs, b)
  printf("%i\n", b);

seq_free(bs);
seq_free(as);
```

#### seq_cpy_slice

```C
void seq_cpy_slice_iso(Type T, Seq(T) a, Seq(T) b, Slice s);
void seq_cpy_slice(Seq(T) a, Seq(T) b, Slice s);

Seq seq_cpy_slice_(size_t siz, Seq a, Seq b, Slice s);
```

Copies a slice of sequence `b` into sequence `a`.

If the internal `seq_require` fails, `CE_OOM` will be thrown.

```C
// TODO
```

### Stack Functions

#### seq_push

```C
void seq_push_iso(Type T, Seq(T) s, T *v);
void seq_push(Seq(T) s, T *v);

Seq seq_push_(size_t siz, Seq s, void *v);
```

Pushes the value `v` onto the sequence `s`.

Note that `v` is a pointer. If you must push a literal, it may be done like so,
for example:

```C
seq_push(xs, &(int){5});
```

If the internal `seq_require` fails, `CE_OOM` will be thrown.

```C
Seq(int) xs = seq_alloc(int, 10);

for (int i = 0; i < 10; i++)
  seq_push(xs, &i);

seq_foreach(xs, x)
  printf("%i\n", x);
```

#### seq_pop / seq_tos / seq_tos_ptr

```C
T *seq_tos_ptr_iso(Type T, Seq(T) s);
T *seq_tos_ptr(Seq(T) s);

T seq_tos_iso(Type T, Seq(T) s);
T seq_tos(Seq(T) s);

T seq_pop_iso(Type T, Seq(T) s);
T seq_pop(Seq(T) s);

void *seq_pop_(size_t siz, Seq s, size_t n);
```

`seq_tos_ptr` yields a pointer to the top of the stack. It is shorthand for
`s + (seq(s)->len - 1)`.

`seq_tos` yields the top value of the stack. It is shorthand for
`s[seq(s)->len - 1]`.

`seq_pop` pops the top value off of the stack.

If the length of the sequence is 0, `CE_OOB` will be thrown when using `seq_tos_ptr`.

```C
// TODO
```

#### seq_pull

```C
T seq_pull_iso(Type T, Seq(T) s);
T seq_pull(Seq(T) s);

void *seq_pull_(size_t siz, Seq s);
```

This function pulls the bottom value from a queue.

```
// TODO
```
