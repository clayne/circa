# The Circa Library Set

## Documentation Home

### Dynamic Datatypes

These "sub-libraries" each implement different dynamically allocated datatypes.

- [seq](seq.md): Singly indirected sequences. Work with normal array functions.
- [txt](txt.md): Singly indirected strings. Work with normal string functions.
- [map](map.md): Value-to-value map, using Robin Hood hashing, linear probing, and the xxHash hash function.
- [seqmap](seqmap.md): Similar to map, but with a seq as the key.
- [set](set.md): A collection of values; implemented as SeqMap(T, bool) underneath.
- [dict](dict.md): Similar to map, but with a normal C string (not txt, necessarily) as the key.

### Miscellaneous

These are some less "interesting" parts of the library, but if you plan on
writing anything robust using Circa these are definitely things you should
take a look at.

- [bits](bits.md): Various bit-twiddling functions.
- [slice](slice.md): A flexible "slice" type that is essentially just two size_t values.
- [error](errors.md): How to handle errors that Circa functions throw.
