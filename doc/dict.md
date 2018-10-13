# Circa Docs : Dictionaries

Dictionaries (`Dict(T)`) are Circa's implementation of a hash table. Unlike most
other speed-oriented hash table implementations, the capacities are made to be
prime numbers-- usually this has speed disadvantages, but a lookup table is used
to allow fiding a prime capacity in O(1) time. Because the capacity is prime,
there are fewer collisions, and therefore better performance for insertion
and lookup.

Dictionaries are made with
[Open Addressing](https://en.wikipedia.org/wiki/Open_addressing), which allows
for better cache locality by avoiding linked lists under the hood.

For the hash function, dictionaries use
[xxHash](http://cyan4973.github.io/xxHash/), which is the fastest hash function
relative to the quality of the output that could be found for Circa's use.
Additionally, it's header-only, so it fits nicely into the workflow of the rest
of the library.

For the insertion algorithm,
[Robin Hood Hashing](https://andre.arko.net/2017/08/24/robin-hood-hashing/)
is utilized to make the worst-case lookup time much better
(somewhere around O(log n) or O(log log n)
if I read [this](http://luc.devroye.org/robinhood.pdf) correctly)
by more evenly distributing the buckets relative to their probe counts.

## Structure

```C
struct dict_data {
  size_t cap;
  size_t len;
  // ...
};
```

Here are the user-facing fields of the `Dict(T)` type, accessed by `dict()`. There
are other fields, but interfacing them directly is highly discouraged.

## Accessors

### dict

```C
struct dict_data *dict(Dict);
struct dict_data *dict_(Dict d, circa_msg fname, circa_msg line);
```

Access the core structure of a dictionary.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%zu\n", dict(d)->len); // 1
```

### dict_set

```C
void dict_set(Dict, char*, <value>);
void dict_set_iso(<type>, Dict, char*, <value>);
Dict dict_set_(size_t siz, Dict d, char *a, void *v, circa_msg fname, circa_msg line);
```

Sets a dictionary's value at a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%i\n", dict_get(d, "foo")); // 500
```

### dict_get

```C
<value> dict_get(Dict, char*);
<value> dict_get_iso(<type>, Dict, char*);
void *dict_get_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);
```

Gets a dictionary's value at a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%i\n", dict_get(d, "foo")); // 500
```

### dict_has

```C
bool dict_has(Dict, char*);
bool dict_has_iso(<type>, Dict, char*);
bool dict_has_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);
```

Checks if a dictionary has a value at a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
if (dict_has(d, "foo"))
  puts("d has foo");
```

## Allocators

### dict_new

```C
Dict dict_new(<type>, size_t);
Dict dict_new_iso(<type>, size_t);
Dict dict_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line);
```

Allocates a new dictionary of a given type and starting capacity. Note that the
capacity will be rounded up to the closest higher power of two, then to the
closest prime. If you need to know what number this comes out to be, you can
find this using the `u32_primegt` function found in `circa/bits.h`.

```C
Dict(int) d = dict_new(int, 10);
printf("%zu\n", dict(d)->cap); // 17
```

### dict_rsz

```C
void dict_rsz(Dict, size_t);
void dict_rsz_iso(<type>, Dict, size_t);
Dict dict_rsz_(size_t siz, Dict d, size_t cap, circa_msg fname, circa_msg line);
```

Reallocates a dictionary to a specified capacity that's greater than the current
capacity. Note that the capacity will be rounded up to the closest higher power
of two, then to the closest prime. If you need to know what number this comes
out to be, you can find this using the `u32_primegt` function found in
`circa/bits.h`.

```C
Dict(int) d = dict_new(int, 10);
dict_rsz(d, 20);
printf("%zu\n", dict(d)->cap); // 37
```

### dict_del

```C
void dict_del(Dict);
void dict_del_iso(<type>, Dict);
Dict dict_del_(size_t siz, Dict d, circa_msg fname, circa_msg line);
```

Deletes a dictionary and set it to NULL. All freed memory will be zeroed first.
Deleting a dictionary twice will not result in an error.

```C
Dict(int) d = dict_new(int, 10);
dict_del(d);
dict_del(d);
```
