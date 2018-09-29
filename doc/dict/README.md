# Circa

## Dictionaries

Dictionaries (`Dict(T)`) are Circa's implementation of a hash table. Unlike most
other speed-oriented hash table implementations, the capacities are made to be
prime numbers-- usually this has speed disadvantages, but a lookup table is used
to allow fiding a prime capacity in O(1) time. Because the capacity is prime,
there are fewer collisions, and therefore better performance for insertion
and lookup.

Dictionaries are made with
[Open Addressing](https://en.wikipedia.org/wiki/Open_addressing), which allows
for better cache locality by avoiding linked lists under the hood.

For the hash function, dictionaries use
[xxHash](http://cyan4973.github.io/xxHash/), which is the fastest hash function
relative to the quality of the output that could be found for Circa's use.
Additionally, it's header-only, so it fits nicely into the workflow of the rest
of the library.

For the insertion algorithm,
[Robin Hood Hashing](https://andre.arko.net/2017/08/24/robin-hood-hashing/)
is utilized to make the worst-case lookup time much better by more evenly
distributing the buckets relative to their probe counts.

All of these optimizations add up to a hash table implementation which is
extremely speed focused, but with admittedly very little attention paid to
memory footprint. Due to the nature of Open Addressing, more buckets are made
than are technically necessary, and the way Circa does Robin Hood Hashing adds
to this problem by also storing the probe count. The O(1) method for finding
prime numbers also leads to some wastage, as it works by aligning to powers of
two first, meaning size will grow exponentially as the dictionary is filled.
