# The Circa Library Set

## Sequences

Sequences (`Seq(T)`) are an implementation of an array type with a dynamic
capacity and length, allowing for avoidance of unnecessary allocations and
making code clearer, as you no longer have to pass around `_cap` variables.
