# [Circa](README.md)

## How It Works

Circa has a good few tricks up its sleeve that allow it to be as ergonomic as it
is, and understanding these tricks can help you avoid a few gotchas.

### Datatypes

All of the dynamic data types in Circa are implemented using a certain variation
on what are known as "fat pointers." This is the same approach used by
[SDS](https://github.com/antirez/sds) and
[Cello](http://libcello.org/learn/a-fat-pointer-library) as well, and if the
explanation I'm about to give is inadequate I would advise looking at those
instead; they are very well written.

Essentially, Circa's fat pointers are pointers to a block of data with a certain
amount of extra space allocated beforehand; this allows us to use negative
indexing to retrieve any information the type may need to store.

```
| ------ |
| Header |
| ------ | <- Pointer
| Elem 0 |
| ------ |
| Elem 1 |
| ------ |
| etc.   |
| ------ |
```

In order to access this header, you'd just have to use the function of the same
name as the datatype; `seq` will return the header of `Seq(T)`, `txt` for `Txt`,
etc.

This pattern allows certain datatypes like `Seq(T)` and `Str` to act like `T*`
and `char*` respectively for backwards compatibility with normal C array and
string functions, which is a really nice property to have.

So then the question remains: what is this actually good for? Well, having
a stored capacity and length makes finding string length an `O(1)` operation,
and because capacity and length are separate values, we can avoid unnecessary
reallocations if we shrink and then grow an array. Another benefit is that you
no longer have to keep track of a bunch of `_cap` or `_len` variables that you
would otherwise have to pass around; they're just part of the data type itself.

But surely there has to be a catch, right? Well, as the SDS page puts it, there
are two main disadvantage, but I'm going to extend that to three.

The first disadvantage SDS gives is that functions will have to return
references to datatypes as a result of taking only a singly indirected reference
to the datatype as an argument. For example:

```C
Dict(int) d = dict_alloc_(sizeof(int), 10);
d = dict_realloc_(sizeof(int), d, 20);
```

However, you may notice these functions have an weird extra underscore at the
end.. why is that?

Well, to put it simply, this isn't at all a problem in Circa, because this isn't
how you're meant to use it; we get around it using [macros](#macros).
This ergonomically puts Circa above SDS, at the cost of a bit more wizardry to
the library obscuring what's going on. The resulting code looks like so:

```C
Dict(int) d = dict_alloc(int, 10);
dict_realloc(d, 20);
```

So then what are the other two disadvantages? Well, for one, outside references
still don't automatically update; any time you do an operation on a datatype
that could possibly realloc, you need to update the reference by returning
said datatype or modifiying a pointer to it's reference.

The other disadvantage is a bit more subtle, and maybe not as big of a problem
as the other, but because our capacity and length are stored in the block of
data rather than on the stack alongside the pointer, there is an extra layer
of indirection to fetching those values.

### Macros

Almost any function in Circa that doesn't have an extra underscore tacked onto
the end isn't actually a function-- it's a macro.

For example, let's take `seq_realloc`.

```C
#define seq_realloc_iso(T, S, C) (S) = seq_realloc_(sizeof(T), (S), (C))
#define seq_realloc(S, C) seq_realloc_iso(typeof(*S), S, C)
Seq seq_realloc_(size_t siz, Seq s, size_t cap);
```

As we can see here, `seq_realloc_iso` and `seq_realloc` are actually macros that
call `seq_realloc_`, but handle `sizeof` and/or `typeof` for us, and also allow
us to not have to remember to re-set the sequence to the result of the
reallocation.

However, this obviously isn't some magic all-powerful solution; it's possible,
then, for references to the sequence to become unsynced or not work. If your
code reallocs or could possibly realloc a sequence or a dictionary, it has to
update any external references to it outside of the current scope, either by
returning it or modifiying a pointer to said reference.

For those who are wondering, the `_iso` variant exists because `typeof` is not
actually a standard C construct; `_iso` is for any C compiler that does not
implement `typeof`.
