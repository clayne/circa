# circa

my personal library set for standard c99 (with support for some gnu99 extensions
if detected).

## features

- polymorphic dynamic arrays
- polymorphic dynamic hashed array trees
- dynamic ascii strings (no real utf-8 support yet)

## how to use

include the `.h` files for whatever structures you need in the given file,
defining all of their parameters beforehand. for instance in any module where
you want an array of ints, you would do the following:

```C
#define T int
  #include "circa_arr.h"
#undef T
```

unless `CIRCA_STATIC` is defined, this will act like a normal header, just
giving you the type definitions and function signatures. in order to generate
the code you must instead include the corresponding `.c` file in another
translation unit, like so:

```C
#define T int
  #include "circa_arr.c"
#undef T
```

because this can only be done once per type per statically linked program,
including these `.c` files in non-dynamic library code is not advisable unless
you created the type you are generating the code for; those of you familiar with
typeclasses may recognize this as being equivalent to the
[orphan instances problem](https://wiki.haskell.org/Orphan_instance). Solutions
here would be deferring the codegen to the end user or only using the code with
static linkage (by instead defining `CIRCA_STATIC` when including the `.h` file)
so as to avoid conflict and complication, at the expense of potential code size
bloat.

## how to build

you don't. all parts of the library are just meant to be includables into your
own project; circa does not need to be (and cannot be) installed onto a machine
as a library, nor can it be "built" on its own. there are tests in the `tests`
directory, which can be built with a simple `cc tests/arr.c` for example.

## alternatives

to justify my creation of this library versus the existing landscape, and
potentially point people who dislike what they see here in the right direction,
here are some alternatives 

### ctl

[ctl](https://github.com/glouw/ctl), the c template library.

this is the library I would say is closest to circa in goal and implementation.

there are a few differences in the principles of our libraries:

- ctl does not support the non-static use case, whereas circa does.
- ctl is far more focused, implementing only data structures and operations on them. this minimalist vibe is nice.
- ctl has a memory ownership model that may improve safety. for my own use cases I think this could introduce friction, especially with use of other peoples' types for which orphan instances would be needed, but I see the appeal.
- ctl will never support unordered containers or maps, at the expense of speed.

### stb

[stb](https://github.com/nothings/stb), sean's toolbox.

- stretchy buffers are minimalist to the point of being less useful than circa's arr type, and invoke undefined behavior. yuck.
- stb is massively useful for many different problem domains, especially those involving media. circa has no such libraries for this; in this sense circa and stb are not in direct competition, but stretchy buffers in specific felt worth noting.

### sds

[sds](https://github.com/antirez/sds), simple dynamic strings. created by antirez, the guy who made redis.

- sds invokes undefined behavior due to its pointer-to-array behavior, in the same way stretchy buffers do. yuck.
- sds is more battle tested than circa, seeing use in redis. it also has some neat features that circa's `str` does not.

### glib

[glib](https://gitlab.gnome.org/GNOME/glib), gnome's datastructure library.

- glib uses `void*`-style polymorphism, which is inherently less performant due to indirection.
- it's GNU code. ehhhh. this also means LGPL rather than MIT licensing.

### cello

[cello](http://libcello.org/), a high level language atop C.

- cello introduces a GC, which immediately makes it a deal breaker for many people using C.
- cello uses the same "fat pointer" model that I deducted points from stb/sds for due to UB. bad.
- cello's scope is MUCH wider than circa and has some interesting features that circa will never have.
