# The Circa Library Set
## NOTE: This is a work in progress. Do not use for anything critical.

Circa is a library set for C aiming to make the language easier to use for
common tasks that would benefit from dynamically sized generic datatypes.

A table of all of the implemented types can be found below, though no concrete
documentation exists for now besides the code itself, which has a few comments
here and there.

| Type      | Description                                                       |
| --------- | ----------------------------------------------------------------- |
| Dict(T)   | A Robin Hood hash table of `char* -> T` using open addressing.    |
| Map(K, V) | Exactly like `Dict(T)` but `K -> V` instead.                      |
| Seq(T)    | A dynamically sized array of `T`. Analogous to C++ `std::vector`. |
| Txt       | A dynamically sized ASCII string. Analogous to C++ `std::string`. |
| Slice     | A generic range type consisting of two `size_t` values.           |
