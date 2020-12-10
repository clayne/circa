# The Circa Library Set

## `macro/cat.h`

### Description

Macros `C2(A, B)`, `C3(A, B, C)`, `C4(A, B, C, D)` that concatenate the given
inputs with underscores between them. This is used for generic headers like
`data/vec.h`, where it's necessary to concatenate identifiers like `C2(vec, T)`
so that when the file is included with `#define T int`, you get `vec_int`.
