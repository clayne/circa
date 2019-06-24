# The Circa Library Set

## Text

Texts are the dynamic string type which Circa offers, largely inspired by the
wonderful [sds](https://github.com/antirez/sds) library (which I would heavily
advise using over Circa if you *only* need strings.)

### Types

#### Externals

```C
typedef char *Txt;
```

Texts are binary compatible with normal C strings and don't need to be generic,
and the type definition reflects this. There's not much else to be said here.

#### Internals

```C
typedef struct {
  size_t cap; // Space allocated.
  size_t len; // Space used.
  char data[];
} TxtData;
```

Texts are internally identical to Sequences-- in fact, they are passable as a
`Seq(char)` and vice-versa. If you'd like to know a little bit more about how
they work, you should look at the sequence documentation.

Note that `len` will always be at most `cap - 1` because these strings are still
null-terminated for compatibility.

### Accessors

#### txt

```C
static inline TxtData *txt(Txt t);
```

This function returns a pointer to the internal structure of a text for easy
modification. This is the idiomatic way of obtaining the capacity or length of a
text.

```C
Txt t = txt_alloc(10); // Allocate a text of 10 characters.
printf("%zu\n", txt(t)->cap); // Print the capacity, in this case 10.
txt_free(t); // Free the text's memory.
```

The following errors may be thrown:

`CE_ARG`: if `t` is NULL. (Debug builds only.)

#### txt_set

```C
void txt_set(Txt t, size_t a, char c);

Txt txt_set_(Txt t, size_t a, char c);
```

This function sets a character of a text at a certain index.

```C
Txt t = txt_alloc(3);
txt_set(t, 0, 'h'); // Analogous to t[0] = 'h', but with book-keeping.
txt_set(t, 1, 'i'); // Analogous to t[1] = 'i', but with book-keeping.
puts(t); // hi
txt_free(t);
```

The following errors may be thrown:

- `CE_ARG`: if `t` is NULL or `c` is `\0`. (Debug builds only.)

#### txt_get

```C
char txt_get(Txt t, size_t a);

char txt_get_(Txt t, size_t a);
```

This function retreives the character from a specific index in a text. Unlike
`seq_get`, this can never result in a `NULL` dereference, so this is always
safe, even without you doing a bounds check beforehand-- however, `CE_OOB` will
still be thrown so you can detect when an out-of-bounds does happen. `\0` will
be returned in this instance.

```C
Txt t = txt_alloc(2);
txt_set(t, 0, 'h');
printf("%c\n", txt_get(t, 0)); // Analogous to t[0], for the most part.
txt_free(t);
```

The following errors may be thrown:

- `CE_ARG`: if `t` is NULL. (Debug builds only.)
- `CE_OOB`: if `a > txt(t)->len`.

### Allocators

#### txt_alloc

```C
Txt txt_alloc(size_t cap);

Txt txt_alloc_(size_t cap);
```

This function allocates a text of a given capacity.

```C
Txt t = txt_alloc(10); // Allocate a text of 10 characters.
txt_free(t); // Free the memory.
```

The following errors may be thrown:

- `CE_ARG`: if `cap` is 0. (Debug builds only.)
- `CE_OOM`: If memory allocation fails.

#### txt_realloc

```C
void txt_realloc(Txt t, size_t cap);

Txt txt_realloc_(Txt t, size_t cap);
```

This function reallocates a text to a given capacity.

```C
Txt t = txt_alloc(5); // Allocate a text of 5 characters.
txt_realloc(t, 10); // Reallocate the text to 10 characters.
txt_free(t); // Free the memory.
```

The following errors may be thrown:

- `CE_ARG`: If `t` is NULL or `cap` is 0.
- `CE_OOM`: If memory reallocation fails.

#### txt_require

```C
void txt_require(Txt t, size_t cap);

Txt txt_require_(Txt t, size_t cap);
```

Shorthand for `cap < txt(t)->cap ? txt_realloc_(t, cap + CIRCA_TXT_PREALLOC) : t`.

```C
Txt t = txt_alloc(5); // Allocate a text of 5 elements.
printf("%zu\n", txt(t)->cap); // 5
txt_require(t, 3); // Require that the text has at least 3 elements.
printf("%zu\n", txt(t)->cap); // 5
txt_require(t, 7); // Require that the text has at least 7 elements.
printf("%zu\n", txt(t)->cap); // 7 + CIRCA_SEQ_PREALLOC
```

The following errors may be thrown:

- `CE_ARG`: if `t` is NULL or `cap` is 0.
- `CE_OOM`: if the internal call to `txt_realloc_` fails.

#### txt_free

```C
void txt_free(Txt t);

Txt txt_free_(Txt t);
```

Frees the memory used by a text. Passing in `NULL` is safe, and texts passed in
via the macro will be made `NULL`; this way double-free is usually not a concern.

```C
Txt t = txt_alloc(10); // Allocate a text of 10 characters.
txt_free(t); // Free the memory used by a text.
```

The following errors may be thrown:

- None.
