# [Circa](../README.md)

## Text

Text (`Txt`) is a way of storing a dynamic string with a given capacity and
length so that unnecessary reallocations may be avoided.

### Accessors

#### txt

```C
struct txt_data *txt(Txt t);
```

`txt` returns the pointer to the underlying structure of a text, which is
defined as shown below.

```C
struct txt_data {
  size_t cap; // The text's capacity.
  size_t len; // The text's length.
  char data[];
};
```

This is considered the primary method to get a text's length or capacity:

```C
Txt t = txt_alloc(1);

printf("%zu\n", txt(t)->cap);
printf("%zu\n", txt(t)->len);
```
