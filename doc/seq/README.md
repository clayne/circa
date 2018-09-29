# Circa

## Sequences

Sequences (`Seq(T)`) are Circa's version of dynamic arrays, analogous to
(for example) C++'s "vectors." The name is inspired by Nim's `seq[T]` types,
and as such, functional operators such as `map`, `filter`, `reduce`, etc. are
provided.

Sequences store the capacity and length of the sequence, allowing for
unnecessary reallocations to be avoided and de-cluttering your namespace by
keeping everything in one variable. Reallocation also doesn't require any
manual reassignment like `realloc` does-- instead, Circa actually uses macros
to do it for you, removing the opportunity for user error.

Sequences are also binary compatible with an array of the type-- `Seq(int)`,
for example, could be passed to a function taking `int*`. This means direct
indexing is possible, but for safety and automatic reallocation, set and get
functions are provided as well.
