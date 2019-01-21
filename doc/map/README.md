# Circa

## Maps

Maps (`Map(K, V)`) are an implementation of an
[open addressing](https://en.wikipedia.org/wiki/Open_addressing) hash table
using [Robin Hood hashing](http://andre.arko.net/2017/08/24/robin-hood-hashing/)
and [xxHash](http://cyan4973.github.io/xxHash/) which maps `K` to `V`.

All in all this allows for a hash table design that has far lower amounts of
indirection compared to a standard linear probing hash table, and keeps the
probe count of every bucket very low, making for faster inserts and reads.
