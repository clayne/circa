# [Circa](README.md)

## Dictionaries

Dictionaries (`Dict(T)`) are an implementation of an
[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hash table
using [Robin Hood hashing](http://andre.arko.net/2017/08/24/robin-hood-hashing/)
and [xxHash](http://cyan4973.github.io/xxHash/) which maps `char*` to `T`.

All in all this allows for a hash table design that has far lower amounts of
indirection compared to a standard linear probing hash table, and keeps the
probe count of every bucket very low, making for faster inserts and reads.

### Accessors

#### dict

```C
struct dict_data *dict(Dict d);
```

`dict` returns the pointer to the underlying structure of a dictionary,
which is defined as shown below.

```C
struct dict_data {
  size_t cap; // The dictionary's capacity.
  size_t len; // The dictionary's length.
  struct dict_bucket buckets[];
};
```

This is considered the primary method to get a dictionary's length or capacity.

```C
Dict(int) d = dict_alloc(int, 1);

printf("%zu\n", dict(d)->cap);     // usz_primegt(1)
printf("%zu\n", dict(d)->len);     // 0
```

#### dict_set

```C
void dict_set_lit_iso(type T, Dict(T) d, char *k, T v);
void dict_set_lit(Dict(T) d, char *k, T v);
void dict_set_iso(type T, Dict(T) d, char *k, T v);
void dict_set(Dict(T) d, char *k, T v);
Dict dict_set_(size_t siz, Dict d, char *k, void *v);
```

`dict_set` is how you set a value at some index of the dictionary; to put it
simply, hash map inserstion.

```C
Dict(int) d = dict_alloc(int, 5);

dict_set_lit_iso(int, d, "a", 1);       // d["a"] = 1

dict_set_lit(d, "b", 2);                // d["b"] = 2

int n = 3;
dict_set_iso(int, d, "c", n);           // d["c"] = 3

n = 4;
dict_set(d, "d", n);                    // d["d"] = 4

n = 5;
d = dict_set_(sizeof(int), d, "e", &n); // d["e"] = 5
```

#### dict_has

```C
bool dict_has_iso(Type T, Dict(T) d, char *k);
bool dict_has(Dict(T) d, char *k);
bool dict_has_(size_t siz, Dict d, char *k);
```

`dict_has` is a simple way of checking if a dictionary has an index.

```C
Dict(int) d = dict_alloc(int, 1);

dict_set_lit(d, "a", 1);

if (dict_has_iso(int, d, "a"))
  puts("d has a, as expected.");

if (dict_has(d, "b"))
  puts("d shouldn't have b; something is wrong.");

if (dict_has_(sizeof(int), d, "c"))
  puts("d shouldn't have c; something is wrong.");
```

#### dict_get

```C
T dict_get_iso(type T, Dict(T) d, char *k);
T dict_get(Dict(T) d, char *k);
void *dict_get_(size_t siz, Dict d, char *k);
```

`dict_get` is how you retrieve the value at an index in a dictionary.

```C
Dict(int) d = dict_alloc(int, 1);
dict_set_lit(d, "a", 1);
printf("%i\n", dict_get_iso(int, d, "a"));              // 1
dict_set_lit(d, "b", 2);
printf("%i\n", dict_get(d, "b"));                       // 2
dict_set_lit(d, "c", 3);
printf("%i\n", *(int*) dict_get_(sizeof(int), d, "c")); // 3
```


### Allocators

This file details the allocators for the `Dict(T)` type in Circa.

#### dict_alloc

```C
Dict(T) dict_alloc(type T, size_t cap);
Dict(T) dict_alloc_iso(type T, size_t cap);
Dict dict_alloc_(size_t siz, size_t cap);
```

`dict_alloc` is the primary means for allocating dictionaries, taking in
a type and a starting capacity. Note that it will not obey the exact capacity
you enter; rather, it will be rounded up to the next greatest prime number.

If you need to find out what prime number `dict(d)->cap` will be rounded to,
you can use the `usz_primegt` function found in the `bits` header.

#### dict_realloc

```C
void dict_realloc(Dict(T) d, size_t cap);
void dict_realloc_iso(type T, Dict(T) d, size_t cap);
Dict dict_realloc_(size_t siz, Dict d, size_t cap);
```

`dict_realloc` is how you reallocate a dictionary to a specified size, larger
or smaller than the current capacity. If it cannot be resized to the specified
capacity, then it will readjust itself accordingly.

As with `dict_alloc`, `cap` will be run through `usz_primegt` before usage.

#### dict_require

```C
void dict_require(Dict(T) d, size_t cap);
void dict_require_iso(type T, dict(T) d, size_t cap);
Dict dict_require(size_t siz, Dict d, size_t cap);
```

`dict_require` is essentially a modified version of `dict_realloc` that will
only allow you to increase the capacity; if the capacity is lower, then the
function is a no-op. This is useful in cases where you want to require that
a dictionary can handle a certain number of elements without auto-resizing
(which is an expensive operation) but want to avoid resizing if it can already
handle it.

#### dict_free

```C
void dict_free(Dict(T) d);
void dict_free_iso(Dict(T) d);
Dict dict_free_(Dict d);
```

`dict_free` is pretty self explanatory. It frees the memory used by the
dictionary.

The only meaningful difference is that the macro variations of it, `dict_free`
and `dict_free_iso`, also set `d` to `NULL`; this prevents double-free errors.
