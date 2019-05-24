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

This means that Circa generates very space-efficient code that has good
cache locality and fast compile times, granting you simplicity with no
extra costs.

---

## Examples

*WIP*
