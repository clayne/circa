# The Circa Library Set

## `math/simd.h`

### Description

Portable versions of basic SIMD functionality, done in such a way that it
compiles down to scalar instructions on platforms with no SIMD support. To
clarify, this doesn't generate portable *executables* like you would get with
[function multi-versioning](https://lwn.net/Articles/691932/); rather it's just
the *code* that is portable.

### Type Definitions

The common format is `T_xS` where `T` is the type and `S` is the width. For
instance, a vector of three uint8_ts would be `uint8_t_x3`. This is defined for
all types `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `int8_t`, `int16_t`,
`int32_t`, `int64_t`, `float`, and `double`. The included widths are `2`, `4`,
`8`, `16`, `32`, `64`. Any combination that won't yield a valid size under
AVX-512 are not generated; for instance `double_x16` does not exist
because that would be 1024 bits.

### Macros

The common format here is `V#` where `#` is the index of the element you want to
access. For example `V0(v)` is the 0th element of the SIMD value `v`. This can
also be used for mutation, as in:

```C
V0(v) = 5;
```

If the index is variable, you can also write `VN(v, i)` for element `i`.

### Initialization

#### set

```C
T_xS T_xS_set(/* depends on the width of the type */);
```

Create a SIMD value with all of the lanes' values specified.

```C
uint8_t_x4 u = uint8_t_x4_set(0, 1, 2, 3);
```

#### set1

```C
T_xS T_xS_set1(T);
```

Create a SIMD value with all of the lanes' values set to a single value.

```C
uint8_t_x4 u = uint8_t_x4_set1(0);
```

### Common Operations

#### add

```C
T_xS T_xS_add(T_xS, T_xS);
```

Perform a lanewise addition.

```C
uint8_t_x4 a = uint8_t_x4_set(0, 1, 2, 3);
uint8_t_x4 b = uint8_t_x4_set1(1);
uint8_t_x4 c = uint8_t_x4_add(a, b); // {1, 2, 3, 4}
```

#### add1

```C
T_xS T_xS_add1(T_xS, T);
```

Perform a lanewise addition of a scalar.

```C
uint8_t_x4 a = uint8_t_x4_set(0, 1, 2, 3);
uint8_t_x4 c = uint8_t_x4_add1(a, 1); // {1, 2, 3, 4}
```

#### sub

```C
T_xS T_xS_sub(T_xS, T_xS);
```

Perform a lanewise subtraction.

```C
uint8_t_x4 a = uint8_t_x4_set(1, 2, 3, 4);
uint8_t_x4 b = uint8_t_x4_set1(1);
uint8_t_x4 c = uint8_t_x4_sub(a, b); // {0, 1, 2, 3}
```

#### sub1

```C
T_xS T_xS_sub1(T_xS, T);
```

Perform a lanewise subtraction of a scalar.

```C
uint8_t_x4 a = uint8_t_x4_set(1, 2, 3, 4);
uint8_t_x4 c = uint8_t_x4_sub1(a, 1); // {0, 1, 2, 3}
```

#### 1sub

```C
T_xS T_xS_1sub(T, T_xS);
```

Perform a lanewise subtraction from a scalar.

```C
uint8_t_x4 a = uint8_t_x4_set(1, 2, 3, 4);
uint8_t_x4 c = uint8_t_x4_1sub(4, a); // {3, 2, 1, 0}
```

#### mul

```C
T_xS T_xS_mul(T_xS, T_xS);
```

Perform a lanewise multiplication.

```C
uint8_t_x4 a = uint8_t_x4_set(1, 2, 3, 4);
uint8_t_x4 b = uint8_t_x4_set1(5);
uint8_t_x4 c = uint8_t_x4_mul(a, b); // {5, 10, 15, 20}
```

### TODO
