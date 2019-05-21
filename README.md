# The Circa Library Set
## 0.1.0

Circa is a library set for C11 that aims to make common tasks
faster and less error prone by providing the generic datatypes
that C is commonly criticized for missing, and doing it right--
with speed in mind.

All of Circa's generic datatypes and functions are also *actually* generic;
no weird template or overloader macro stuff is being done. Instead, functions
are designed in such a way that they just shuffle data around type-agnostically
with `void*` and data sizes, hidden behind some nice macros that hide the dirty
stuff.

This means that Circa generates very space-efficient code that has good
cache locality and fast compile times, granting you simplicity with no
extra costs.

---

## Examples

*WIP*

---

## Features

- Nice debugging features like `ce_debug` to see what went wrong, and `CE` for handling.
- Fast, portable implementations of integer operations like CLZ and CTZ.
- A simple Slice type for indexing ranges of sequences and strings simply.
- Dynamic, generic sequences via `Seq(T)`.
- Dynamic ASCII strings via `Txt`, reusing the binary interface of `Seq(T)`.
- Dynamic, generic maps via `Map(K, V)` using Robin Hood Hashing, xxHash, and a custom prime number lookup function.
- Dynamic, generic maps of seq keys via `SeqMap(K, V)` with the same algorithms as `Map(K, V)`.
- Dynamic, generic dictionaries via `Dict(T)`, reusing the binary interface of `SeqMap(K, V)`.
- Dynamic, generic sets via `Seq(T)`, reusing the binary interface of `Map(K, V)`.
- Dynamic, generic sets of seq keys via `SeqSet(K, V)`, reusing the binary interface of `SeqMap(K, V)`.
