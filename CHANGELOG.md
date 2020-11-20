# Circa 0.2.X Changelogs

## 0.2.0

All of the old 0.1.X code has been scrapped in favor of a new unity build
philosophy with no external dependencies. A lot of features are now missing as
a result, but the resulting codebase should be a lot cleaner and less
error-prone, since I've generally improved since I last touched the library.

- Added `macro/cat.h`.
  * macros: `C2`, `C3`, `C4`.
- Added `macro/gnu.h`.
  * macros: `circa_if_gnu`, `circa_std_or_gnu`, `likely`, `unlikely`, `circa_gnu_attr`, `circa_gnu_builtin`.
- Added `algo/imath.h`.
  * functions: for `T` in `{uint8_t, uint16_t, uint32_t, uint64_t, size_t}`: `T_min`, `T_max`, `T_pop`, `T_clz`, `T_bsr`, `T_ctz`.
- Added `data/hatc.h`.
  * types: `hatc_T`, `hatc_T_idx`.
  * constants: `hatc_T_siz`, `hatc_T_mask`.
  * functions: `hatc_T_locate`, `hatc_T_alloc`, `hatc_T_free`, `hatc_T_lookup`, `hatc_T_set`, `hatc_T_get`, `hatc_T_snoc`.
- Added `data/hatd.h`.
  * types: `had_T`, `hatd_T_idx`.
  * functions: `hatd_T_locate`, `hatd_T_alloc`, `hatd_T_free`, `hatd_T_lookup`, `hatd_T_set`, `hatd_T_get`, `hatd_T_snoc`.
- Added `data/vec.h`.
  * types: `vec_T`.
  * functions: `vec_T_alloc`, `vec_T_amort`, `vec_T_free`, `vec_T_lookup`, `vec_T_get`, `vec_T_set`, `vec_T_snoc`.
- Added `test.c`.
