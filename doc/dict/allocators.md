# Circa

## Dictionaries

### Allocators

#### dict_new

```C
Dict dict_new(<type>, size_t);
Dict dict_new_iso(<type>, size_t);
Dict dict_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line);
```

Allocate a new dictionary of a given type and starting capacity. Note that the
capacity will be rounded up to the closest higher power of two, then to the
closest prime. If you need to know what number this comes out to be, you can
find this using the `u32_primegt` function found in `circa/bits.h`.

```C
Dict(int) d = dict_new(int, 10);
printf("%zu\n", dict(d)->cap); // 17
```

#### dict_rsz

```C
void dict_rsz(Dict, size_t);
void dict_rsz_iso(<type>, Dict, size_t);
Dict dict_rsz_(size_t siz, Dict d, size_t cap, circa_msg fname, circa_msg line);
```

Reallocate a dictionary to a specified capacity that's greater than the current
capacity. Note that the capacity will be rounded up to the closest higher power
of two, then to the closest prime. If you need to know what number this comes
out to be, you can find this using the `u32_primegt` function found in
`circa/bits.h`.

```C
Dict(int) d = dict_new(int, 10);
dict_rsz(d, 20);
printf("%zu\n", dict(d)->cap); // 37
```

#### dict_del

```C
void dict_del(Dict);
void dict_del_iso(<type>, Dict);
Dict dict_del_(size_t siz, Dict d, circa_msg fname, circa_msg line);
```

Delete a dictionary and set it to NULL. All freed memory will be zeroed first.
Deleting a dictionary twice will not result in an error.

```C
Dict(int) d = dict_new(int, 10);
dict_del(d);
dict_del(d);
```
