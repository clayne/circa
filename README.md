# Circa
## A core library set for C.

Circa is a set of libraries ranging in versions from ISO C89 to GNU C11 that
help to make using C less of a chore for tasks requiring dynamic sequences,
dynamic strings, or dynamic tables. Portable functions for CPU intrinsic
functions such as SIMD and bit twiddling are also provided.

| Header  | Standard | Version | Description                                  | Includes           | Depends |
| ------- | -------- | ------- | -------------------------------------------- | ------------------ | ------- |
| core.h  | ANSI/ISO | C89/C90 | Core utilities and configs.                  | n/a                | n/a     |
| types.h | ISO      | C99     | Portable Rust-style typedefs. i8, u32, etc.  | core               | n/a     |
| bits.h  | ISO      | C99     | Portable bit twiddling. CLZ, CTZ, etc.       | core, types        | n/a     |
| simd.h  | GNU      | C99     | Portable SIMD intrinsics. i8x2, f32x4, etc.  | core, types        | n/a     |
| dict.h  | ISO/GNU  | C99     | Dynamic Robin Hood hash tables using xxHash. | core, types, bits  | xxHash  |
| seq.h   | ISO/GNU  | C99     | Dynamic sequences.                           | core               | n/a     |
| str.h   | ISO      | C99     | Dynamic strings.                             | core               | n/a     |

# Examples

## Types

To Be Done

## Bit Twiddling

To Be Done

## SIMD

To Be Done

## Dictionaries

A toy example storing and printing some fruit quantities:

```C
#include "circa/dict.h"

int main() {
  Dict(int) fruits = dict_new(int, 2);
  dict_set(fruits, "orange", 5);
  dict_set(fruits, "apple", 100);
  dict_set(fruits, "peach", 25);
  dict_set(fruits, "banana", 10);
  dict_set(fruits, "mango", 500);
  dict_set(fruits, "pear", 2);
  dict_foreach(fruits, k, v, {
    printf("%s: %i\n", k, v);
  });
  dict_del(fruits);
  return 0;
}
```

## Sequences

Printing the odd numbers in 1-10:

```C
#include "circa/seq.h"

void puti(int n) {
  printf("%i\n", n);
}

bool odd(int n) {
  return n & 1;
}

int main() {
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(a, odd, b);
  seq_do(b, puti);
  seq_del(a);
  seq_del(b);
  return 0;
}
```

## Strings

Printing out the README in the current directory:

```C
#include "circa/str.h"

int main() {
  FILE *fp = fopen("README.md", "r");
  if (fp != NULL) {
    Str readme = str_new(1);
    str_read(readme, fp);
    printf("%s", readme);
    str_del(readme);
    fclose(fp);
  }
  return 0;
}
```

# Why Circa?

## Code Duplication

Circa offers polymorphic types, such as `Seq(T)` and `Dict(T)`. This allows you
to stop wasting time implementing your own array-of-type allocators and custom
hash tables, and offers nicer control functions for said structures, such as
`map`, `filter`, `reduce`, `apply`, and `do`.

## Ease of Use

Circa is a header-only library, which means you can tack it into a project
without changing your build script at all. (Though pre-compiled headers may
lead to a marginal speed-up, this is up to you.)

## Compile Times

Although veteran C++ users may frown upon the header-only architecture of
Circa, this is actually due to what header-only libraries in C++ generally
indicate: the use of template metaprogramming.

C++ templates and C "type-definer" polymorphism works by re-defining the type
and the functions for *every single type that is used*, and that is done *for
every compilation unit that uses the header.* It's no wonder that'd be slow.

However, Circa doesn't use this approach-- instead, functions are written to be
type-generic with only one declaration, taking a void pointer, and a type size
argument, doing all data shuffling with memcpy, memset, etc.

Additionally, Circa is designed to not be a huge "mono-library." Rather, Circa
tries to compartmentalize each library, with the main dependencies being the
more generic modules, such as `core` and `types`, neither of which define any
actual functions-- they're purely type definitions and macros.

As a result, Circa actually compiles at a fine speed, and the benefits for ease
of use and performance, detailed below, arguably outweigh the extremely small
costs induced by recompiling.

## Performance

Because Circa is header-only, all of its code can be statically linked and
therefore inlined, allowing the compiler to optimize better and removing the
overhead created by function calls.

Circa is also designed with performance in mind, research being done on which
algorithm is the best for any given problem. For example, consider `Dict(T)`--
Circa uses the [xxHash](https://github.com/Cyan4973/xxHash) hash function for
fast hashing,
uses [Open Addressing](https://en.wikipedia.org/wiki/Open_addressing) to avoid 
linked lists for better cache locality, and
[Robin Hood Hashing](https://andre.arko.net/2017/08/24/robin-hood-hashing/) for
far better worst-case lookup times. Even the function that finds the prime size
for the hash table was optimized to be O(1) by aligning the number to the next
power of two, taking the 2's logarithm using CPU intrinsics, then using that
to index a lookup table of prime numbers slightly larger than the corresponding
power of two.

## Debugging

Under the hood, Circa makes extensive use of assertions, even going so far as
to give you file names and line numbers for where you called the function. For
performance builds, you can disable this by defining `CIRCA_N_DBG` or `NDEBUG`,
either directly by doing `#define CIRCA_N_DBG` or `#define NDEBUG` before
including a Circa library, or by passing `-DCIRCA_N_DBG` or `-DNDEBUG` to the
compiler. Note that defining `NDEBUG` will also disable normal C assertions--
`CIRCA_N_DBG` will disable only Circa's.

Additionally, because you don't have to implement these structures yourself,
you're saved from creating more bugs.

# How Does It Work?

Circa makes extensive use of macros for user convenience and debug info. Let's
take a look at how that actually works.

Consider the following code:

```C
#include "circa/seq.h"

void puti(int n) {
  printf("%i\n", n);
}

bool odd(int n) {
  return n & 1;
}

int main() {
  Seq(int) a = seq_lit(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  Seq(int) b = seq_new(int, 1);
  seq_filter(a, odd, b);
  seq_do(b, puti);
  seq_del(a);
  seq_del(b);
  return 0;
}
```

Seems simple enough, but if you know much about C you'll know this shouldn't be
possible normally-- C doesn't have generics or parametric types, so what does
this translate to?

Welllll...

```C
#include "circa/seq.h"

void puti(int n) {
  printf("%i\n", n);
}

_Bool odd(int n) {
  return n & 1;
}

int main() {
  int* a = seq_lit_(sizeof(int), sizeof((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) / sizeof(int), &(int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, "odd.c", "17");
  int* b = seq_new_(sizeof(int), (1), "odd.c", "18");
  do {
    typeof(*a) V; 
    (b) = seq_rqr_(sizeof(typeof(*a)), b, seq_(a, "odd.c", "19")->len, "odd.c", "19");
    seq_(b, "odd.c", "19")->len = 0;
    for (size_t I = 0; I < seq_(a, "odd.c", "19")->len; I++) {
      (V) = (*((typeof(*a)*) seq_get_(sizeof(typeof(*a)), a, I, "odd.c", "19")));
      if (odd(V)) {
        (b) = seq_push_(sizeof(typeof(*a)), b, &V, "odd.c", "19"); 
      } 
    }
  } while (0);
  do {
    for (size_t I = 0; I < seq_(b, "odd.c", "20")->len; I++) {
      puti((*((typeof(*b)*) seq_get_(sizeof(typeof(*b)), b, I, "odd.c", "20"))));
    }
  } while (0);
  ((a)) = seq_del_(sizeof(typeof(*a)), ((a)), "odd.c", "21");
  ((b)) = seq_del_(sizeof(typeof(*b)), ((b)), "odd.c", "22");
  return 0;
}
```

So we see that `Seq(int)` is actually `int*` in reality, which shouldn't be
suprising; but there's more to it.

Each function is boiled down to a form that has an underscore following it,
which are how they're actually defined in the library itself. These take the raw
size of the type, pointers to the inputs, and debug info.

The control flow and functional operations, such as `filter` and `do`, instead
compile down to multiple lines of code, encapsulated in a `do { ... } while(0);`
which should be a familiar idiom to anyone who does macro programming as a way
of safely requiring a semicolon after the block.

The reason `int*` is used underneath isn't only for binary compatiblity, either.
Even `Dict(T)`, which doesn't gurantee binary compatibility, does this. The main
reason is because it allows for use of `typeof()` to get the type of the data
structure, that way you don't have to pass the type explicitly like you do with
the `_iso` versions of the functions.
