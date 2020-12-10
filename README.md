# The Circa Library Set
## 0.4.0

### Introduction

Circa is a library set for standard or GNU C11 intended for my personal use in
writing desktop applications like compilers or games, where you tend to want
efficient collections and portable wrappers over intrinsics.

It assumes a unity build structure for your project, which for intermediate
sized projects will generally be faster to compile while having very good
performance thanks to being able to inline without the use of LTO. In addition,
unity builds allow polymorphic specialization to be done in a way that avoids
duplication. As an added bonus, you don't necessarily need a build system.
However, you don't get parallelized compilation. This may be a deal-breaker for
some people who have massive projects and powerful CPU, not that I think they
would care much for this library anyways.

Circa is not exhaustively tested, the API is unstable, and many abstractions are
intentionally leaky for the sake of simplicity and performance. Caveat emptor.

### Features

If you can stomach all of that, you'll get a little toolkit of performance
focused data structures and functions, shown below.

```
circa
|- data - Data structure implementations.
|  |- hatc.h - A sequential hashed array tree with constant-sized sub-blocks. O(1) read/write/snoc. O(n) wasted space.
|  |- hatd.h - A sequential hashed array tree with doubling-size sub-blocks. O(1) read/write/snoc. O(n) wasted space.
|  |- str.h - A dynamic ASCII string. O(1) read/write, O(1) amortized snoc, O(n) wasted space.
|  |- vec.h - A sequential vector. O(1) read/write, O(1) amortized snoc, O(n) wasted space.
|- math - Mathematical functions.
|  |- imath.h - Integer math operations. ctz, clz, popcount, the list goes on.
|  |- simd.h - SIMD types and operations.
|- macro - Macro utilities.
|  |- cat.h - Tools for concatenating identifiers. Useful for polymorphism.
|  |- gnu.h - Macros for optional GNU extension usage. (Still works in standard C.)
|- misc
|  |- error.h - Provides an error enum for use in other headers.
|  |- threadpool.h - Abstraction over threading to allow multi-word CAS and other trickery.
```

### Potential To-Do List

More will be added as time goes on. With no particular order, priority, or
guarantee of delivery:

- Floating point function approximations.
- FNV1a hashing.
- Hashmaps.
- Concurrent vectors.
- Concurrent queues.
- Concurrent hashmaps.
- A `hatq` hashed array tree type with only `O(sqrt n)` wasted space.
- Functions for handling UTF-8 data in strings.
- More proper benchmark and test suites to demonstrate the library.
