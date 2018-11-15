/*
** dict.h | The Circa Library Set | Dict(T) Header
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

/*
** Dependencies
*/

/* Vendored */

#define XXH_INLINE_ALL
#include "../../lib/xxHash/xxhash.h"

/* Internal */

#include "core.h"
#include "bits.h"

/*
** Types
*/

#define Dict(T) T*
typedef Dict(void) Dict;

struct dict_bucket {
  char *key;
  size_t probe;
  char data[];
};

struct dict_data {
  size_t cap, len;
  char data[];
};

/*
** Prototypes
*/

/* Accessors */

#define dict(D) dict_((D), CIRCA_DBGI)
struct dict_data *dict_(Dict d, CIRCA_ARGS);

#define dict_set_lit_iso(T, D, A, V) (D) = dict_set_(sizeof(T), (D), (A), &(V), CIRCA_DBGI)
#define dict_set_lit(D, A, V) dict_set_lit_iso(typeof(*D), D, A, V)
#define dict_set_iso(T, D, A, V) (D) = dict_set_(sizeof(T), (D), (A), &(T){V}, CIRCA_DBGI)
#define dict_set(D, A, V) dict_set_iso(typeof(*D), D, A, V)
_circa_rets_ Dict dict_set_(size_t siz, Dict d, char *a, void *v, CIRCA_ARGS);

#define dict_get_iso(T, D, A) (*((T*) dict_get_(sizeof(T), (D), (A), CIRCA_DBGI)))
#define dict_get(D, A) dict_get_iso(typeof(*D), D, A)
void *dict_get_(size_t siz, Dict d, char *a, CIRCA_ARGS);

#define dict_has_iso(T, D, A) dict_has_(sizeof(T), (D), (A), CIRCA_DBGI)
#define dict_has(D, A) dict_has_iso(typeof(*D), D, A)
bool dict_has_(size_t siz, Dict d, char *a, CIRCA_ARGS);

/* Allocators */

#define dict_new_iso(T, C) dict_new_(sizeof(T), (C), CIRCA_DBGI)
#define dict_new(T, C) dict_new_iso(T, C)
_circa_alcs_ Dict dict_new_(size_t siz, size_t cap, CIRCA_ARGS);

#define dict_rsz_iso(T, D, C) (D) = dict_rsz_(sizeof(T), (D), (C), CIRCA_DBGI)
#define dict_rsz(D, C) dict_rsz_iso(typeof(*D), D, C)
_circa_rets_ Dict dict_rsz_(size_t siz, Dict d, size_t cap, CIRCA_ARGS);

#define dict_rqr_iso(T, D, C) (D) = dict_rqr_(sizeof(T), (D), (C), CIRCA_DBGI)
#define dict_rqr(D, C) dict_rqr_iso(typeof(*D), D, C)
_circa_rets_ Dict dict_rqr_(size_t siz, Dict d, size_t cap, CIRCA_ARGS);

#define dict_shr_iso(T, D) (D) = dict_shr_(sizeof(T), (D), CIRCA_DBGI)
#define dict_shr(D) dict_shr_iso(typeof(*D), D)
_circa_rets_ Dict dict_shr_(size_t siz, Dict d, CIRCA_ARGS);
#define dict_del_iso(T, D) (D) = dict_del_(sizeof(T), (D), CIRCA_DBGI)
#define dict_del(D) dict_del_iso(typeof(*D), D)
_circa_rets_ Dict dict_del_(size_t siz, Dict d, CIRCA_ARGS);

#endif /* CIRCA_DICT_H */
