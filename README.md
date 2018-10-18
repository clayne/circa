# Circa
## A core library set for C.

Circa is a set of libraries ranging in versions from ANSI C89 to GNU C11 that
aim to do two things:
- Add the generic data-structures C is missing.
- Make high-performance intrinsics portable.

Below is a list of all of Circa's libraries, with information given that may be
relevant to deciding if a given library would fit into your projects.

| Header  | Standard | Version | Description                                  | Includes           | Depends |
| ------- | -------- | ------- | -------------------------------------------- | ------------------ | ------- |
| core.h  | ANSI/ISO | C89/C90 | Core utilities and configs.                  | n/a                | n/a     |
| types.h | ISO      | C99     | Portable Rust-style typedefs. i8, u32, etc.  | core               | n/a     |
| bits.h  | ISO      | C99     | Portable bit twiddling. CLZ, CTZ, etc.       | core, types        | n/a     |
| simd.h  | GNU      | C99     | Portable SIMD intrinsics. i8x2, f32x4, etc.  | core, types        | n/a     |
| dict.h  | ISO/GNU  | C99     | Dynamic Robin Hood hash tables using xxHash. | core, types, bits  | xxHash  |
| seq.h   | ISO/GNU  | C99     | Dynamic sequences.                           | core               | n/a     |
| str.h   | ISO      | C99     | Dynamic strings.                             | core               | n/a     |

# Links

- [Documentation](doc/README.md)
- [Example Code](ex)
