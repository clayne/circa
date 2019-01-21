# The Circa Library Set

## Text

Text (`Txt`) is an implementation of dynamic ASCII strings with stored capacity
and length, still null-terminated to allow easy backwards compatibility with C
string functions. This allows for avoiding unnecessary reallocations and saves
the mental overhead of having to pass around `_cap` variables in structures
or functions.
