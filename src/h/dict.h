/*
** dict.h | The Circa Library Set | Dynamic Robin Hood dictionaries.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

#include "core.h"
#include "bits.h"

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpadded"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wcast-align"
#endif

/*
** Unlike the `Str` or `Seq` types, `Dict` has a slightly more complicated
** structure because of how hash tables work. Each value in the array is
** stored in a "bucket" which holds the data, the key, a "probe count", and
** a boolean indicating if the bucket has been deleted.
**
** The probe count just says how far the bucket is from its "ideal" location,
** determined by using a hash function on the key. This is stored in order to
** allow us to more efficiently organize the buckets using Robin Hood hashing.
**
** The "deleted" flag is so we can continue to index the dictionary normally
** with robin hood hashing without having to rehash the entire dictionary,
** which would be extremely expensive performance-wise.
*/

struct bucket_data {
  void *data; // TODO: Pack `data` and `key` into one array, if possible.
  char *key;
  size_t probe;
  bool deleted;
};

/*
** From here on out, though, `Dict` follows the same cookie-cutter structure
** as `Seq` and `Str`. Just a fat pointer with a capacity, a length, and a
** main data section-- this time the data section isn't intended to be indexed
** like a normal C array though. Instead, the only reason `Dict(T)` expands to
** `T*` is to allow the non-`_iso` versions of the functions to infer the type
** of the dictionary when it is passed in.
*/

struct dict_data {
  size_t cap;
  size_t len;
  struct bucket_data buckets[];
};

/*
** The same trick as `Seq`: defining `Dict(T)` as `T*` and then declaring `Dict`
** to be `Dict(void)`.
*/

#define Dict(T) T*
typedef Dict(void) Dict;

/*
** Now we need a structure accessor and a way of indexing the dictionary. Much
** like `Seq`, macros are needed for just about every function because of the
** genericity of the functions in regard to type size.
*/

static inline struct dict_data *dict(Dict d);

#define dict_set_iso(T, D, K, V) (D) = dict_set_(sizeof(T), (D), (K), &(T){V})
#define dict_set(D, K, V) dict_set_iso(typeof(*D), D, K, V)
Dict dict_set_(size_t siz, Dict d, char *k, void *v);

#define dict_has_iso(T, D, K) dict_has_(sizeof(T), (D), (K))
#define dict_has(D, K) dict_has_iso(typeof(*D), D, K)
bool dict_has_(size_t siz, Dict d, char *k);

#define dict_get_iso(T, D, K) (*((T*) dict_get_(sizeof(T), (D), (K))))
#define dict_get(D, K) dict_get_iso(typeof(*D), D, K)
void *dict_get_(size_t siz, Dict d, char *k);

/*
** Then we need some allocators to get our dictionaries built.
*/

#define dict_alloc_iso(T, C) dict_alloc_(sizeof(T), (C))
#define dict_alloc(T, C) dict_alloc_iso(T, C)
Dict dict_alloc_(size_t siz, size_t cap);

#define dict_realloc_iso(T, D, C) (D) = dict_realloc_(sizeof(T), (D), (C))
#define dict_realloc(D, C) dict_realloc_iso(typeof(*D), D, C)
Dict dict_realloc_(size_t siz, Dict d, size_t cap);

#define dict_require_iso(T, D, C) (D) = dict_require_(sizeof(T), (D), (C))
#define dict_require(D, C) dict_require_iso(typeof(*D), D, C)
Dict dict_require_(size_t siz, Dict d, size_t cap);

#define dict_free_iso(T, D) (D) = dict_free_((D))
#define dict_free(D) dict_free_iso(typeof(*D), D)
Dict dict_free_(Dict d);

/*
** And finally we need to implement the static structure accessor function.
** Just like with `Seq` and `Str`, it is important for performance reasons to do
** it in the header and not the C file.
*/

static inline
struct dict_data *dict(Dict d) {
  return ((struct dict_data*) d) - 1;
}

#ifdef __clang__
  #pragma clang diagnostic pop // -Wcast-align
  #pragma clang diagnostic pop // -Wpadded
#endif

#endif // CIRCA_DICT_H
