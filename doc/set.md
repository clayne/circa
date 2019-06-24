# The Circa Library Set

## Sets

Sets are analogous to those from the eponymous field of set theory; they simply
store a set of values, in which there may be no duplicates.

### Types

#### Externals

```C
#define Set(T) Map(T, bool)
typedef void *Set;
```

Under the hood sets are implemented as a map from the target type to a boolean,
which is reflected by their type definition here.

#### Internals

Same internals as maps.

### Accessors

#### set

```C
static inline MapData *set(Set e);
```

This function returns a pointer to the internal structure of a set for easy
modification. This is the diiomatic way of obtaining the capacity or length of a
set.

```C
Set(int) e = set_alloc(int, 5);
printf("%zu\n", set(e)->cap); // usz_primegt(5)
set_free(e);
```

The following errors may be thrown:

- `CE_ARG`: if `e` is NULL. (Debug builds only.)
