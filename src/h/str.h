/*
** str.h | The Circa Library Set | Str Header
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_STR_H
#define CIRCA_STR_H

/*
** Dependencies
*/

/* Internal */

#include "core.h"

/*
** Types
*/

typedef char *Str;

struct str_data {
  size_t cap;  // The capacity of the string.
  size_t len;  // The string's length.
  char data[]; // The string's data.
};

/*
** Prototypes
*/

/* Accessors */

#define str(S) str_((S), CIRCA_DBGI)
_circa_ struct str_data *str_(Str s, CIRCA_ARGS);

#define str_set(S, A, V) (S) = str_set_((S), (A), (V), CIRCA_DBGI)
_circa_rets_ Str str_set_(Str s, size_t a, char c, CIRCA_ARGS);

#define str_get(S, A) str_get_((S), (A), CIRCA_DBGI)
char str_get_(Str s, size_t a, CIRCA_ARGS);

/* Allocators */

#define str_new(C) str_new_((C), CIRCA_DBGI)
_circa_alcs_ Str str_new_(size_t cap, CIRCA_ARGS);

#define str_lit(CS) str_lit_((CS), CIRCA_DBGI)
_circa_alcs_ Str str_lit_(const char *const restrict cs, CIRCA_ARGS);

#define str_from(S) str_from_((S), CIRCA_DBGI)
_circa_alcs_ Str str_from_(Str s, CIRCA_ARGS);

#define str_wrap(CS) str_wrap_((CS), (strlen(CS)), CIRCA_DBGI)
#define str_wrap_len(CS, L) str_wrap_((CS), (L), CIRCA_DBGI)
_circa_alcs_ Str str_wrap_(char *cs, size_t len, CIRCA_ARGS);

#define str_rsz(S, C) (S) = str_rsz_((S), (C), CIRCA_DBGI)
_circa_rets_ Str str_rsz_(Str s, size_t cap, CIRCA_ARGS);

#define str_rqr(S, C) (S) = str_rqr((S), (C), CIRCA_DBGI)
_circa_rets_ Str str_rqr_(Str s, size_t cap, CIRCA_ARGS);

#define str_shr(S) (S) = str_shr_((S), CIRCA_DBGI)
_circa_rets_ Str str_shr_(Str s, CIRCA_ARGS);

#define str_del(S) (S) = str_del_((S), CIRCA_DBGI)
_circa_rets_ Str str_del_(Str s, CIRCA_ARGS);

/* Evaluators */

#define str_eq(A, B) str_eq_((A), (B), CIRCA_DBGI)
bool str_eq_(Str a, Str b, CIRCA_ARGS);

/* Stack Ops */

#define str_push(S, V) (S) = str_push_((S), (V), CIRCA_DBGI)
_circa_rets_ Str str_push_(Str s, char c, CIRCA_ARGS);

#define str_tos(S) str_pop_((S), 0, CIRCA_DBGI)
#define str_pop(S) str_pop_((S), 1, CIRCA_DBGI)
char str_pop_(Str s, size_t n, CIRCA_ARGS);

/* String Ops */

#endif /* CIRCA_STR_H */
