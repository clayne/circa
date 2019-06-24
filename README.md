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

This means that Circa yields very space-efficient code that has good
cache locality and fast compile times, granting you simplicity with little to no
additional cost.

---

# Examples

## Dictionaries

```C
#include <circa.h>

int main() {
  Dict(int) fruits = dict_alloc(int, 1);

  dict_set(fruits, "oranges", &(int){7});
  dict_set(fruits, "peaches", &(int){5});
  dict_set(fruits, "apples",  &(int){10});

  dict_foreach(fruits, k, v)
    printf("%s: %i\n", k, v);

  dict_free(fruits);

  return 0;
}
```

## Sequences

```C
#include <circa.h>

int main() {
  Seq(int) xs = seq_alloc(int, 10);
  
  for (int i = 0; i < 10; i++)
    seq_push(xs, &i);

  seq_filter(xs, it & 1); // Keep only the odd numbers.

  seq_map(xs, it * it); // Square every number.

  seq_foreach(xs, x)
    printf("%i\n", x);
  
  seq_free(xs);

  return 0;
}
```

## Text

```C
#include <circa.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    puts("Pass files in and this program will act like cat.");
    exit(1);
  }
  
  Txt t = txt_alloc(1);

  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (!fp) {
      txt_fmt(t, "txt.o: %s: No such file\n", argv[i]);
      txt_write(t, stdout);
      exit(1);
    }
    txt_read(t, fp);
    txt_write(t, stdout);
    fclose(fp);
  }

  txt_free(t);

  return 0;
}
```

---

# Documentation

Incomplete thus far; you can find what is already done [here](https://github.com/davidgarland/circa/blob/master/doc/README.md).

---

# Integration

Circa is recommended to be used as a git subtree (if you just recoiled, note
that these are different than git submodules; look it up) and statically linked
so that no user-side installation is needed, code bloat is reduced, and runtime
performance is improved.

In order to do this, you should first add Circa as a remote:

```Bash
git remote add -f circa https://github.com/davidgarland/circa
```

Next, we can add it as a subtree:

```Bash
git subtree add --prefix lib/circa circa master --squash
```

(Of course, you can swap `lib/circa` out for wherever you want to put it in
your project.)

To update at a later date, all you need to do is:

```Bash
git fetch circa master
git subtree pull --prefix lib/circa circa master --squash
```

Now that Circa is installed, you can use it one of two ways, given below.

---

## Header-Only Mode

Header-only is the simplest way of using C libraries, and so Circa supports
this-- it also has good performance implications in some cases due to everything
being inlined. However, it can notably be bad for icache because it leads to a
lot of code duplication at the assembly level, so be wary of this if you're
using the more heavyweight utilities like `Map(K, V)` and such.

In order to do this, all you have to do is put the following in *before* doing
the `#include` for Circa:

```C
#define CIRCA_HEADER_ONLY
```

---

## Statically Linked via Make

Static linkage is only slightly more complicated to set up.

If you're using a Makefile, it should be as simple as doing the following
in your "build libraries" recipe or wherever:

```Makefile
cd lib/circa && $(MAKE) <build_type>
```

Where `<build_type>` is one of:

```
| Build Type | Description                                                 |
| ---------- | ----------------------------------------------------------- |
| fast       | a speed-focused build with no debug features.               |
| small      | a size-focused build with no debug features.                |
| build      | a balanced build with debugging features and optimizations. |
| debug      | a debugging focused build.                                  |
| sanitized  | a debugging build with ubsan, asan, and lsan.               |
```

Finally, just add `-llib/circa` to your `LDFLAGS`, assuming that's where you
put it.
