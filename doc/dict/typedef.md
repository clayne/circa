# Circa

## Dictionaries

### Type Definition

```C
struct dict_data {
  size_t cap;
  size_t len;
  // ...
};
```

Here are the user-facing fields of the `Dict` type, accessed by `dict()`. There
are other fields, but interfacing them directly is highly discouraged.
