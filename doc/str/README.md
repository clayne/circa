# Circa

## Strings

Strings (`Str`) are Circa's way of storing and processing text. Unlike the
default C strings, they have a stored capacity and length, meaning checking the
length of a string is now O(1) instead of O(n), and reallocations can be avoided
because it isn't naiively assumed that the length is the capacity.

Circa's strings are also binary compatible with C strings, being `char*` under
the hood and still having a null terminator, so you can pass them to C string
functions. This is inspired by the [SDS](https://github.com/antirez/sds) library
by Antirez.

Unlike SDS, however, Circa avoids what they call "Disadvantage #2"; each string
function call is actually a macro which sets the string to the result of the
function, and as a result you can't forget to assign to them. "Disadvantage #1",
that string references don't sync and may be invalid, could not be fixed without
double indirection, however, so that problem persists. (Suggested solution:
don't use pointer references so much.)
