# The Circa Library Set

## `math/imath.h`

### Description

Portable versions of integer math functions, done in such a way that it
compiles down to scalar instructions on platforms with no intrinsics. To
clarify, this doesn't generate portable *executables* like you would get with
[function multi-versioning](https://lwn.net/Articles/691932/); rather it's just
the *code* that is portable.


