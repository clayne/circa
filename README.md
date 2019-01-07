# The Circa Library Set
## NOTE: This is a work in progress. Do not use for anything critical.

Circa is a library set for C aiming to make the language easier to use for
common tasks that would benefit from dynamically sized generic datatypes.

To avoid namespace pollution, decently unique names are used for the main types Circa
implements-- no "vector", "hashmap", or "string" here. A table of all the types
can be seen below.

| Type    | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| Dict(T) | A Robin Hood hash table of `char* -> T` using open addressing.    |
| Seq(T)  | A dynamically sized array of `T`. Analogous to C++ `std::vector`. |
| Txt     | A dynamically sized string. Analogous to C++ `std::string`.       |
| Slice   | A generic range type consisting of two `size_t` values.           |