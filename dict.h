/*
** dict.h | Circa | A robin-hood-hashing dictionary using xxHash.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_DICT_H
#define CIRCA_DICT_H

/*
** Compatability Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/dict.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/dict.h]: C99 is required."
#endif

#ifdef __TINYC__
  #error "[circa/dict.h]: TCC is not supported."
#endif

/*
** Dependencies
*/

/* Vendored */

#ifdef __GNUC__
  #pragma GCC diagnostic push /* Mute xxHash casting warnings. */
  #pragma GCC diagnostic ignored "-Wbad-function-cast"
  #pragma GCC diagnostic push /* Mute xxHash padding warnings. */
  #pragma GCC diagnostic ignored "-Wpadded"
#endif
    #ifndef XXH_INLINE_ALL
      #define XXH_INLINE_ALL
    #endif
    #include "lib/xxHash/xxhash.h"
#ifdef __GNUC__
  #pragma GCC diagnostic pop
  #pragma GCC diagnostic pop
#endif

/* Internal */

#include "core.h"
#include "types.h"
#include "bits.h"

/*
** Type Definitions
*/

#define Dict(T) T*
typedef Dict(void) Dict;

struct dict_bucket {
  char *key;
  size_t probe;
  bool deleted;
};

struct dict_data {
  size_t cap;
  size_t len;
  struct dict_bucket *buckets;
  char data[];
};

/*
** Function Prototypes
*/

/* Accessors */

#define dict(D) dict_((D), __FILE__, _circa_str_(__LINE__))
_circa_ struct dict_data *dict_(Dict d, circa_msg fname, circa_msg line);

#define dict_set_iso(T, D, A, V) (D) = dict_set_(sizeof(T), (D), (A), &(T){(T) V}, __FILE__, _circa_str_(__LINE__))
#define dict_set(D, A, V) dict_set_iso(typeof(*D), (D), (A), (V))
_circa_ _circa_rets_ Dict dict_set_(size_t siz, Dict d, char *a, void *v, circa_msg fname, circa_msg line);

#define dict_get_iso(T, D, A) (*((T*) dict_get_(sizeof(T), (D), (A), __FILE__, _circa_str_(__LINE__))))
#define dict_get(D, A) dict_get_iso(typeof(*D), (D), (A))
_circa_ void *dict_get_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);

#define dict_has_iso(T, D, A) dict_has_(sizeof(T), (D), (A), __FILE__, _circa_str_(__LINE__))
#define dict_has(D, A) dict_has_iso(typeof(*D), (D), (A))
_circa_ bool dict_has_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);

/* Allocators */

#define dict_new_iso(T, C) dict_new_(sizeof(T), (C), __FILE__, _circa_str_(__LINE__))
#define dict_new(T, C) dict_new_iso(T, C)
_circa_ _circa_alcs_ Dict dict_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line);

#define dict_rsz_iso(T, D, C) (D) = dict_rsz_(sizeof(T), (D), (C), __FILE__, _circa_str_(__LINE__))
#define dict_rsz(D, C) dict_rsz_iso(typeof(*D), (D), (C))
_circa_ _circa_rets_ Dict dict_rsz_(size_t siz, Dict d, size_t cap, circa_msg fname, circa_msg line);

#define dict_del_iso(T, D) (D) = dict_del_(sizeof(T), (D), __FILE__, _circa_str_(__LINE__))
#define dict_del(D) dict_del_iso(typeof(*D), (D))
_circa_ Dict dict_del_(size_t siz, Dict d, circa_msg fname, circa_msg line);

/*
** Accessors
*/

/*
** -- dict_ --
** Description
**   Returns a pointer to the structure of a dictionary.
** Arguments
**   d: Dictionary (Dict)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Dictionary Structure Pointer (struct dict_data*)
*/

_circa_
struct dict_data *dict_(Dict d, circa_msg fname, circa_msg line) {
  {
    circa_assert(d != NULL, fname, line);
  }
  return ((struct dict_data*) d) - 1;
}

/*
** -- dict_set_ --
** Description
**   Sets a dictionary's value for a given key.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   a: Address Key (char*)
**   v: Value (void*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_set_(size_t siz, Dict d, char *a, void *v, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
    circa_assert(v != NULL, fname, line);
    circa_assert(dict(d)->cap != 0, fname, line);
  } 
  /* Create a temporary swap bucket. */
  void *ds = NULL;
  while (ds == NULL)
    ds = calloc(1, siz);
  /* Create a temporary swap bucket dataset. */
  struct dict_bucket *bs = NULL;
  while (bs == NULL)
    bs = calloc(1, sizeof(struct dict_bucket));
  while (bs->key == NULL) // or this one?
    bs->key = calloc(strlen(a) + 1, 1); // the offending line
  /* Load the key/value into the swap values. */
  memcpy(bs->key, a, strlen(a) + 1);
  memcpy(ds, v, siz);
  /* Find the hash address. */
  size_t hash = (sizeof(size_t) == 8) ? XXH64(a, strlen(a), 0)
                                      : XXH32(a, strlen(a), 0);
  size_t addr = hash % dict(d)->cap;
  /* Seek the proper bucket starting at the hash address. */
  size_t i;
  for (i = addr; i < dict(d)->cap; i++) {
    if (dict(d)->buckets[i].key == NULL)
      break;
    if (!strcmp(dict(d)->buckets[i].key, bs->key)) {
      dict(d)->len--;
      break;
    }
    if (dict(d)->buckets[i].probe < bs->probe) {
      /* Swap the bucket datasets. */
      struct dict_bucket swap = *bs;
      *bs = dict(d)->buckets[i];
      dict(d)->buckets[i] = swap;
      /* Swap the buckets. */
      size_t bytes = siz;
      char byte;
      while (bytes --> 0) {
        byte = ((char*) ds)[bytes];
        ((char*) ds)[bytes] = (((char*) d) + (i * siz))[bytes];
        (((char*) d) + (i * siz))[bytes] = byte;
      }
    }
    bs->probe++;
  }
  dict(d)->len++;
  /* Insert the swap bucket and bucket data. */
  if (i != dict(d)->cap) {
    /* The final bucket was found, insert. */
    memmove(&(dict(d)->buckets[i]), bs, sizeof(struct dict_bucket));
    memcpy(((char*) d) + (i * siz), ds, siz);
  } else {
    /* Re-hash and re-insert. */
    d = dict_rsz_(siz, d, dict(d)->cap + 1, fname, line);
    d = dict_set_(siz, d, bs->key, ds, fname, line);
  }
  /* Free the temporary arrays. */
  free(bs);
  free(ds);
  /* Return the dictionary. */
  return d;
}

/*
** -- dict_get_ --
** Description
**   Gets the value of the dictionary at a given address key.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   a: Address Key (char*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Value (void*)
*/

_circa_
void *dict_get_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(a != NULL, fname, line);
    circa_assert(dict(d)->cap != 0, fname, line);
  }
  /* Find the hash address. */
  size_t hash = (sizeof(size_t) == 8) ? XXH64(a, strlen(a), 0)
                                      : XXH32(a, strlen(a), 0);
  size_t addr = hash % dict(d)->cap;
  /* Search for the index. */
  for (size_t i = addr; i < dict(d)->cap; i++)
    if (!strcmp(dict(d)->buckets[i].key, a))
      return ((char*) d) + (siz * i);
  return NULL;
}

/*
** -- dict_has_ --
** Description
**   Checks if a dictionary has an address key.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   a: Address Key (char*)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Boolean (bool)
*/

_circa_
bool dict_has_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
  }
  if (a == NULL) return false;
  return dict_get_(siz, d, a, fname, line) != NULL;
}

/*
** Allocators
*/

/*
** -- dict_new_ --
** Description
**   Allocates a new dictionary of at least a given capacity.
** Arguments
**   siz: Type Size (size_t)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Dictionary (Dict)
*/

_circa_ _circa_alcs_
Dict dict_new_(size_t siz, size_t cap, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(cap > 0, fname, line);
  }
  /* Find a suitable capacity. */
  cap = (size_t) u32_primegt((u32) cap); // TODO: usize_primegt() ?
  /* Allocate the main array. */
  struct dict_data *dd = NULL;
  while (dd == NULL)
    dd = calloc(sizeof(*dd) + (cap * siz), 1);
  dd->cap = cap;
  /* Allocate the buckets. */
  while (dd->buckets == NULL)
    dd->buckets = calloc(cap, sizeof(struct dict_bucket));
  /* Return the dictionary. */
  return dd->data;
}

/*
** -- dict_rsz_ --
** Description
**   Resizes a dictionary to at least a given capacity. May not shrink.
** Arguments 
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   cap: Capacity (size_t)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Feedback Dictionary (Dict)
*/

_circa_ _circa_rets_
Dict dict_rsz_(size_t siz, Dict d, size_t cap, circa_msg fname,
circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
    circa_assert(d != NULL, fname, line);
    circa_assert(cap > 0, fname, line);
    circa_assert(cap > dict(d)->cap, fname, line);
  }
  /* Allocate a temp array for the buckets. */
  void *ds = NULL;
  while (ds == NULL)
    ds = calloc(dict(d)->len, siz);
  /* Allocate a temp array for the bucket data. */
  struct dict_bucket *bs = NULL;
  while (bs == NULL)
    bs = calloc(dict(d)->len, sizeof(struct dict_bucket));
  /* Load the buckets into the temp arrays. */
  size_t i, j;
  for (i = 0, j = 0; i < dict(d)->cap; i++) {
    if (dict(d)->buckets[i].key != NULL) {
      memcpy(((char*) ds) + (j * siz), ((char*) d) + (i * siz), siz);
      memcpy(bs + j, dict(d)->buckets + i, sizeof(*bs));
      j++;
    }
  }
  /* Find a suitable capacity. */
  cap = (size_t) u32_primegt((u32) cap); // TODO: usize_primegt() ?
  /* Allocate the main dictionary. */
  struct dict_data *dd = NULL;
  while (dd == NULL)
    dd = realloc(dict(d), sizeof(struct dict_data) + (cap * siz));
  dd->cap = cap;
  memset(&dd->len, 0, sizeof(&dd->len) + sizeof(&dd->buckets) + (cap * siz));
  d = dd->data;
  /* Allocate the buckets. */
  while (dd->buckets == NULL)
    dd->buckets = calloc(cap, sizeof(struct dict_bucket));
  /* Load the temp arrays into the dictionary. */
  for (i = 0; i < j; i++)
    if (bs[i].key != NULL)
      d = dict_set_(siz, d, bs[i].key, ((char*) ds) + (i * siz), fname, line);
  // TODO: resize on high load without infinite recursion ?
  /* Free the temp arrays. */
  free(ds);
  free(bs);
  /* Return the dictionary. */
  return d;
}

/*
** -- dict_del_ --
** Description
**   Deletes a dictionary.
** Arguments
**   siz: Type Size (size_t)
**   d: Dictionary (Dict)
**   fname: Filename (circa_msg)
**   line: Line Number (circa_msg)
** Returns
**   Dictionary (Dict)
*/

_circa_
Dict dict_del_(size_t siz, Dict d, circa_msg fname, circa_msg line) {
  {
    circa_assert(siz > 0, fname, line);
  }
  if (d != NULL) {
    for (size_t i = 0; i < dict(d)->cap; i++) {
      char *key = dict(d)->buckets[i].key;
      if (key != NULL)
        free(key);
    }
    free(dict(d)->buckets);
    free(dict(d));
  }
  return NULL;
}

/*
** Functional Ops
*/



/*
** Control Flow
*/

#define dict_foreach_(T, D, K, V, FNAME, LINE, ...) \
do { \
  char *K; \
  T V; \
  for (size_t I = 0; I < dict_(D, FNAME, LINE)->cap; I++) { \
    K = dict_(D, FNAME, LINE)->buckets[I].key; \
    if (K != NULL) { \
      V = (*((T*) dict_get_(sizeof(T), D, K, FNAME, LINE))); \
      __VA_ARGS__ \
    } \
  } \
} while(0)

#define dict_foreach_iso(T, D, K, V, ...) dict_foreach_(T, D, K, V, __FILE__, _circa_str_(__LINE__), __VA_ARGS__)

#define dict_foreach(D, K, V, ...) dict_foreach_iso(typeof(*D), D, K, V, __VA_ARGS__)

#endif /* CIRCA_DICT_H */
