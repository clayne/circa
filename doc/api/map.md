# [Circa](../README.md)

## Maps

Maps (`Map(K, V)`) are an implementation of an
[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hash table
using [Robin Hood hashing](http://andre.arko.net/2017/08/24/robin-hood-hashing/)
and [xxHash](http://cyan4973.github.io/xxHash/) which maps `K` to `V`.

All in all this allows for a hash table design that has far lower amounts of
indirection compared to a standard linear probing hash table, and keeps the
probe count of every bucket very low, making for faster inserts and reads.

### Accessors

This section details the allocators for the `Map(K, V)` type in Circa.

#### map

```C
struct map_data *map(Map m);
```

`map` returns the pointer to the underlying structure of a map,
which is defined as shown below.

```C
struct map_data {
  size_t cap; // The map's capacity.
  size_t len; // The map's length.
  struct map_bucket buckets[];
};
```

This is considered the primary method to get a map's length or capacity:

```C
Map(int, int) m = map_alloc(int, int, 1);

printf("%zu\n", map(m)->cap); // usz_primegt(1)
printf("%zu\n", map(m)->len); // 0
```

#### map_set

```C
void map_set_litkv_iso(type K, type V, Map(K, V) m, K k, V v);
void map_set_litkv(Map(K, V) m, K k, V v);
void map_set_litk_iso(type K, type V, Map(K, V) m, K k, V v);
void map_set_litk(Map(K, V) m, K k, V v);
void map_set_litv_iso(type k, type V, Map(K, V) m, K k, V v);
void map_set_litv(Map(K, V) m, K k, V v);
void map_set_iso(type K, type V, Map(K, V) m, K k, V v);
void map_set(Map(K, V) m, K k, V v);
Map  map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v);
```

`map_set` is how you set a value at some index of the map; to put it simply,
hash map inserstion.

`_litkv` variants take a literal key and a literal value.

`_litk` variants take a literal key and a variable value.

`_litv` variants take a variable key and a literal value.

non-`_lit` variants take a variable key and a variable value.

```C
Map(int, int) m = map_alloc(int, int, 1);
int a, b;

map_set_litkv_iso(int, int m, 1, 2);               // 1 -> 2
map_set_litkv(m, 2, 3);                            // 2 -> 3
b = 4;
map_set_litk_iso(m, 3, b);                         // 3 -> 4
b = 5;
map_set_litk(m, 4, b);                             // 4 -> 5
a = 5;
map_set_litv_iso(int, int, m, a, 6);               // 5 -> 6
a = 6;
map_set_litv(m, n, 7);                             // 6 -> 7
a = 7;
b = 8;
map_set_iso(int, int, m, a, b);                    // 7 -> 8
a = 8;
b = 9;
map_set(m, a, b);                                  // 8 -> 9
a = 9;
b = 10;
m = map_set_(sizeof(int), sizeof(int), m, &a, &b); // 9 -> 10
```

#### map_has

```C
bool map_has_litk_iso(type K, type V, Map(K, V) m, K k);
bool map_has_litk(Map(K, V) m, K k);
bool map_has_iso(type K, type V, Map(K, V) m, K k);
bool map_has(Map(K, V) m, K k);
bool map_has_(size_t sizk, size_t sizv, Map m, void *k);
```

`map_has` is a simple way of checking if a map has an index.

`_litk` variants take a literal key.

non-`_lit` variants take a variable key.

```C
Map(int) m = map_alloc(int, int, 1);
int n;

map_set_litkv(m, 1, 2);

if (map_has_litk_iso(int, int, m, 1))
  puts("m has 1, as expected.");

if (map_has_litk(m, 2))
  puts("m shouldn't have 2; something is wrong.");

n = 3;

if (map_has_iso(int, int, m, n))
  puts("m shouldn't have 3; something is wrong.");

n = 4;

if (map_has(m, n))
  puts("m shouldn't have 4; something is wrong.");

n = 5;

if (map_has_(sizeof(int), sizeof(int), m, &n)
  puts("m shouldn't have 5; something is wrong.");
```

#### map_get

```C
V map_get_litk_iso(type K, type V, Map(K, V) m, K k);
V map_get_litk(Map(K, V) m, K k);
V map_get_iso(type K, type V, Map(K, V) m, K k);
V map_get(Map(K, V) d, K k);
void *map_get_(size_t sizk, size_t sizv, Map m, void *k);
```

`map_get` is how you retrieve the value at an index in a map.

```C
Map(int, int) m = map_alloc(int, 1);
int a = 1, b = 2;

map_set(m, a, b);

printf("%i\n", map_get_litk_iso(int, int, m, 1));
printf("%i\n", map_get_litk(m, 1));
printf("%i\n", map_get_iso(int, int, m, a));
printf("%i\n", map_get(m, a));
printf("%i\n", *(int*) map_get_(sizeof(int), sizeof(int), m, &a));
```

### Allocators

This section details the allocators for the `Map(K, V)` type in Circa.

#### map_alloc

```C
Map(K, V) map_alloc_iso(type K, type V, size_t cap);
Map(K, V) map_alloc(type K, type V, size_t cap);
Map map_alloc_(size_t sizk, size_t sizv, size_t cap);
```

`map_alloc` is the primary means for allocating maps, taking in a type and a 
starting capacity. Note that it will not obey the exact capacity you enter;
rather, it will be rounded up to the next greatest prime number.

If you need to find out what prime number `map(m)->cap` will be rounded to,
you can use the `usz_primegt` function found in the `bits` header.

#### map_realloc

```C
void map_realloc_iso(type K, type V, Map(K, V) m, size_t cap);
void map_realloc(Map(K, V) m, size_t cap);
Map  map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap);
```

`map_realloc` is how you reallocate a map to a specified size, larger or smaller
than the current capacity. If it cannot be resized to the specified capacity,
then it will readjust itself accordingly.

As with `map_alloc`, `cap` will be run through `usz_primegt` before usage.

#### map_require

```C
void map_require_iso(type K, type V, Map(K, V) m, size_t cap);
void map_require(Map(K, V) m, size_t cap);
Map  map_require_(size_t sizk, size_t sizv, Map m, size_t cap);
```

`map_require` is essentially a modified version of `map_realloc` that will
only allow you to increase the capacity; if the capacity is lower, then the
function is a no-op. This is useful in cases where you want to require that
a map can handle a certain number of elements without auto-resizing
(which is an expensive operation) but want to avoid resizing if it can already
handle it.

#### map_free

```C
void map_free(Map(K, V) m);
void map_free_iso(Map(K, V) m);
Map  map_free_(Map m);
```

`map_free` is pretty self explanatory. It frees the memory used by the map..

The only meaningful difference is that the macro variations of it, `map_free`
and `map_free_iso`, also set `m` to `NULL`; this prevents double-free errors.
