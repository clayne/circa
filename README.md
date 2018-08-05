# Circa
## A core library set for C.

Circa is a library set written for various standards of the C programming
language with the intent of making C less painful to use for common tasks.

# Library Set

| Library       | Description          | Standard | Revision | Dependencies |
| ------------- | ------------------   | -------- | -------- | ------------ |
| circa/core.h  | Core utils & config. | ISO      | C89      | n/a          |
| circa/types.h | Terse type names.    | ISO      | C99      | core         |
| circa/simd.h  | Portable SIMD.       | GNU      | C99      | core, types  |
| circa/seq.h   | Dynamic sequences.   | ISO*     | C99      | core         |
| circa/str.h   | Dynamic strings.     | ISO      | C99      | core         |

`* Some features may not be available in ISO C.`

