# Circa Changelog

## 0.3.1/2020-11-28-hotfix1

Apparently `str_read` did not insert a null terminator. Whoops.

## 0.3.0/2020-11-28

Two large breaking changes to the API have been made, namely including return
types being changed to a `circa_err` enum and doing some renaming to structure
accessor functions to clarify whether the parameter is passed by reference or
by value.

Aside from that an ASCII string type was added, thread pools with preliminary
support for multi-word CAS have been added, and very very basic SIMD types
have been added.

- Replaced some definitions.
  * functions:
    - For `S` in {`hatc, hatd, vec`}: Split `S_get` into `S_get_v` and `S_get_r`, `S_set` into `S_set_v` and `S_set_r`, `S_snoc` into `S_snoc_v` and `S_snoc_r`.
- Added some definitions.
  * `vec_prealloc`.
- Added `algo/simd.h`.
  * types: for `N` in `2..4` and `T` in `{float, double, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t}`: `T_xN`.
  * functions:
    - For `N` in `2..4` and `T` in `{float, double, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t}`: `T_xN_set`, `T_xN_set1`, `T_xN_add`, `T_xN_sub`, `T_xN_mul`, `T_xN_div`, `T_xN_add1`, `T_xN_sub1`, `T_xN_mul1`, `T_xN_div1`, `T_xN_1sub`, `T_xN_1div`.
    - For `N` in `2..4` and `T` in `{float, double}`: `T_xN_fma`, `T_xN_fms`.
  * macros: `x`, `y`, `z`, `w`, `at`.
- Added `data/str.h`.
  * types: `str`.
  * functions: `str_alloc`, `str_prealloc`, `str_require`, `str_amort`, `str_free`, `str_lookup`, `str_set_v`, `str_set_r`, `str_get_v`, `str_get_r`, `str_snoc_v`, `str_snoc_r`, `str_read`, `str_write`.
- Added `misc/threadpool.h`.
  * types: `thread_id`, `cas_row`, `mcas_helper`.
  * functions: `threadpool_alloc`, `threadpool_free`, `threadpool_spawn`, `threadpool_join`, `threadpool_detach`.

## 0.2.0/2020-11-19

All of the old code has been scrapped in favor of a new unity build philosophy
with no external dependencies. A lot of features are now missing as a result,
but the resulting codebase should be a lot cleaner and less error-prone, since
I've generally improved since I last touched the library.

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
