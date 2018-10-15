# Circa Docs : Strings

## NOTE: This file is under construction. Not all information is filled out.

[Home](README.md)
- [Dict(T)](dict.md)
- [Seq(T)](seq.md)
- Str

## Introduction

Strings (`Str`) are the dynamic strings that Circa offers. Gone are the
headaches of managing null terminators and string lengths/capacities yourself!

The underlying structure of `Str` is very similar to that of `Seq(T)`, the only
difference being that the type is not variable-- `Str` is always an array of
characters.

Also similarly to `Seq(T)`, `Str` may be passed as `char*` to any normal C
string function.

## Structure

```C
struct str_data {
  size_t cap;  // The capacity of the string.
  size_t len;  // The length of the string.
  char data[]; // The string.
};
```

These are the user-facing fields of the `Str` type.
The `data` field above is the one pointed to by the `Str` type itself, and a
pointer to the above structure can be obtained with the `str()` function.

## Accessors

### str

```C
struct str_data *str(Str);
struct str_data *str_(Str s, circa_msg fname, circa_msg line);
```

Access the core structure of a string.

```C
Str a = str_new(1);           // Allocate a new string of 1 character.
printf("%zu\n", str(a)->cap); // Print the capacity. (1)
```

### str_set

```C
void str_set(Str, size_t addr, char c);
Str str_set_(Str s, size_t a, char c, circa_msg fname, circa_msg line);
```

### str_get

```C
char str_get(Str, size_t addr);
char str_get_(Str s, size_t a, circa_msg fname, circa_msg line);
```

## Allocators

### str_new

```C
Str str_new(size_t cap);
Str str_new_(size_t cap, circa_msg fname, circa_msg line);
```

### str_lit

```C
Str str_lit(const char *restrict);
Str str_lit_(const char *restrict cs, circa_msg fname, circa_msg line);
```

### str_from

```C
Str str_from(Str);
Str str_from_(Str s, circa_msg fname, circa_msg line);
```

### str_wrap

```C
Str str_wrap(char *);
Str str_wrap_(char *cs, circa_msg fname, circa_msg line);
```

### str_wrap_len

```C
Str str_wrap_len(char *, size_t);
Str str_wrap_len_(char *cs, size_t len, circa_msg fname, circa_msg line);
```

### str_rsz

```C
void str_rsz(Str, size_t);
Str str_rsz_(Str s, size_t cap, circa_msg fname, circa_msg line);
```

### str_rqr

```C
void str_rqr(Str, size_t);
Str str_rqr_(Str s, size_t cap, circa_msg fname, circa_msg line);
```

### str_rqr_ext

```C
void str_rqr_ext(Str, size_t cap, size_t ext);
Str str_rqr_ext_(Str s, size_t cap, size_t ext, circa_msg fname, circa_msg line);
```

### str_shr

```C
void str_shr(Str);
Str str_shr_(Str s, circa_msg fname, circa_msg line);
```

### str_del

```C
void str_del(Str);
Str str_del_(Str s, circa_msg fname, circa_msg line);
```

## Evaluators

### str_eq

```C
bool str_eq(Str, Str);
bool str_eq_(Str a, Str b, circa_msg fname, circa_msg line);
```

### str_eq_len

```C
bool str_eq_len(Str, Str, size_t);
bool str_eq_len_(Str a, Str b, size_t len, circa_msg fname, circa_msg line);
```

### str_eq_lit

```C
bool str_eq_lit(Str, char *);
bool str_eq_lit_(Str s, const char *restrict cs, circa_msg fname, circa_msg line);
```

## Stack Operations

## String Operations

## IO Operations

### str_read

```C
void str_read(Str, FILE *);
Str str_read_(Str, FILE *fp, circa_msg fname, circa_msg line);
```

### str_write

```C
void str_write(Str, FILE *);
void str_write_(Str, FILE *fp, circa_msg fname, circa_msg line);
```
