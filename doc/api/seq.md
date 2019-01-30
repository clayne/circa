# [Circa](../README.md)

## Sequences

Sequences (`Seq(T)`) are a way of storing a dynamic array with a given capacity
and length so that unnecessary reallocations may be avoided.

### Accessors

#### seq

```C
struct seq_data *seq(Seq s);
```

`seq` returns the pointer to the underlying structure of a sequence,
which is defined as shown below.

```C
struct seq_data {
  size_t cap; // The sequence's capacity.
  size_t len; // The sequence's length.
  char data[];
};
```

This is considered the primary method to get a sequence's length or capacity:

```C
Seq(int) s = seq_alloc(int, 1);

printf("%zu\n", seq(s)->cap);
printf("%zu\n", seq(s)->len);
```

#### seq_set

```C

void seq_set_lit_iso(type T, Seq(T) s, size_t a, T v);
void seq_set_lit(Seq(T) s, size_t a, T v);
void seq_set_iso(type T, Seq(T) s, size_t a, T v);
void seq_set(Seq(T) s, size_t a, T v);
Seq seq_set_(size_t siz, Seq s, size_t a, void *v);
```

`seq_set` is how you set a value at some index of the sequence.

```C
Seq(int) xs = seq_new(int, 5);
int x = 0;

seq_set_lit_iso(int, xs, )
```
