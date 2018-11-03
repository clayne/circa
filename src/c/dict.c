/*
** dict.c | The Circa Library Set | Dynamically allocated dictionaries.
** https://github.com/davidgarland/circa
*/

#include "../h/dict.h"

/*
** Internal Function Prototypes
*/

static _circa_const_ size_t dict_xxh(char *str);
static _circa_const_ size_t buck_siz(size_t siz);
static struct dict_bucket *buck_rawget(size_t siz, struct dict_bucket *bp, size_t i);
static struct dict_bucket *dict_rawget(size_t siz, struct dict_data *dp, size_t i);

/*
** Accessors
*/

/* Access the underlying structure of a dictionary. */

struct dict_data *dict_(Dict d, CIRCA_ARGS) {
  {
    circa_assert(d != NULL, fname, line);
  }
  return ((struct dict_data*) d) - 1;
}

/* Set the value at an index from a dictionary. */

_circa_rets_
Dict dict_set_(size_t siz, Dict d, char *a, void *v, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
    circa_assert(v != NULL, fname, line);
  }
  // Allocate a swap bucket and load the key/value into it.
  const size_t bsiz = buck_siz(siz);
  struct dict_bucket *swp = NULL;
  while (swp == NULL)
    swp = calloc(2, bsiz);
  swp->key = calloc(strlen(a), 1);
  memcpy(swp->key, a, strlen(a) + 1);
  memcpy(swp->data, v, siz);
  // Find the hash address.
  const size_t hash = dict_xxh(a);
  const size_t addr = hash % dict(d)->cap;
  // Seek the proper destination bucket.
  struct dict_bucket *b = NULL;
  size_t i;
  for (i = addr; i < dict(d)->cap; i++) {
    b = dict_rawget(siz, d, i);
    if (b->key == NULL) {
      break; // Free bucket found.
    }
    if (!strcmp(b->key, swp->key)) {
      dict(d)->len--;
      break; // Match found.
    }
    if (b->probe < swp->probe) {
      memcpy(swp + 1, swp, bsiz);
      memcpy(swp, b, bsiz);
      memcpy(b, swp + 1, bsiz);
    }
    swp->probe++;
  }
  dict(d)->len++;
  // Exit.
  free(swp);
  return d;
}

/* Get the value at an index from a dictionary. */

void *dict_get_(size_t siz, Dict d, char *a, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
  }
  return NULL;
}

/* Test if a dictionary has an index. */

bool dict_has_(size_t siz, Dict d, char *a, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
  }
  return dict_get_(siz, d, a, fname, line) != NULL;
}

/*
** Allocators
*/

/* Allocate a new dictionary. */

_circa_alcs_
Dict dict_new_(size_t siz, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  register const size_t bsiz = buck_siz(siz);
  struct dict_data *dp = NULL;
  cap = usz_primegt(cap);
  while (dp == NULL)
    dp = calloc(sizeof(*dp) + (cap * bsiz), 1);
  dp->cap = cap;
  return dp->data;
}

/* Resize a dictionary. */

_circa_rets_
Dict dict_rsz_(size_t siz, Dict d, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(cap >= dict(d)->len, fname, line);
  }
  register const size_t bsiz = buck_siz(siz);
  struct dict_data *const restrict dp = dict(d);
  // Allocate a set of temporary buckets.
  struct dict_bucket *bs = NULL;
  while (bs == NULL)
    bs = calloc(dp->cap, bsiz);
  // Load the buckets into the temporary bucket array.
  struct dict_bucket *bp = NULL;
  size_t i, j;
  for (i = 0, j = 0; i < dp->cap; i++) {
    bp = dict_rawget(siz, dp, i);
    if (bp->key != NULL)
      memcpy(buck_rawget(siz, bs, j++), bp, bsiz);
  }
  // Reallocate the dictionary.
  cap = usz_primegt(cap);
  struct dict_data *dp2 = NULL;
  while (dp2 == NULL)
    dp2 = realloc(dp, sizeof(*dp) + (cap * bsiz));
  dp2->cap = cap;
  d = dp2->data;
  memset(d, 0, cap * bsiz);
  // Load the set of temporary buckets into the dictionary.
  for (i = 0; i < j; i++) {
    bp = buck_rawget(siz, bs, i);
    d = dict_set_(siz, d, bp->key, bp->data, fname, line);
  }
  // Free the temporary array and exit.
  free(bs);
  return d;
}

/* Require that a dictionary be able to hold a given capacity. */

_circa_rets_
Dict dict_rqr_(size_t siz, Dict d, size_t cap, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  return (dict(d)->cap < cap) ? dict_rsz_(siz, d, cap, fname, line) : d;
}

/* Shrink a dictionary as much as possible. */

_circa_rets_
Dict dict_shr_(size_t siz, Dict d, CIRCA_ARGS) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(dict(d)->len > 0, fname, line);
  }
  register const size_t len = dict(d)->len;
  return (dict(d)->cap > len) ? dict_rsz_(siz, d, len, fname, line) : d;
}

/*
** Internal Functions
*/

/* A wrapper over XXHash that respects the size of size_t. */

static _circa_const_
size_t dict_xxh(char *str) {
  return (sizeof(size_t) == 8) ? XXH64(str, strlen(str), 0)
                               : XXH32(str, strlen(str), 0);
}

/* Finds the true size of a bucket of a given type size. */

static _circa_const_
size_t buck_siz(size_t siz) {
  const size_t bsiz = sizeof(struct dict_bucket) + siz;
  return (bsiz + 7) & (~7); // Aligns the number to a multiple of 8.
}

/* Gets a pointer to the `i`th bucket in a bucket array. */

static
struct dict_bucket *buck_rawget(size_t siz, struct dict_bucket *bp, size_t i) {
  return (struct dict_bucket *) (((char*) bp) + (i * buck_siz(siz)));
}

/* Gets a pointer to the `i`th bucket in a dictionary. */

static
struct dict_bucket *dict_rawget(size_t siz, struct dict_data *dp, size_t i) {
  return buck_rawget(siz, (struct dict_bucket *) dp->data, i);
}
