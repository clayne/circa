/*
** dict.c | The Circa Library Set | Dynamic Robin Hood Dictionaries
** https://github.com/davidgarland/circa
*/

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wused-but-marked-unused"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wbad-function-cast"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wpadded"
#endif

#define XXH_INLINE_ALL
#define XXH_NAMESPACE CIRCA
#include "../../lib/xxhash/xxhash.h"

#include "../h/dict.h"

/*
** Accessors
*/

Dict dict_set_(size_t siz, Dict d, char *a, void *v) {
  if (!siz || !d || !a || !v)
    return (CE = CE_ARG, d);

  // Set up a swap bucket.
  struct bucket_data swp = {
    .data    = malloc(siz),
    .key     = calloc(strlen(a) + 1, 1),
    .probe   = 0,
    .deleted = false
  };

  if (!swp.data || !swp.key)
    return (CE = CE_OOM, d);

  memcpy(swp.data, v, siz);
  strcpy(swp.key, a);

  // Calculate the starting position using xxHash.
  size_t hash = (sizeof(size_t) == 8) ? XXH64(a, strlen(a), 0) 
              : XXH32(a, strlen(a), 0);
  size_t addr = hash % dict(d)->cap;

  // Traverse the dictionary in search of a position for it.
  size_t i;
  bool found = false;
  for (i = addr; i < dict(d)->cap; i++) {
    if (dict(d)->buckets[i].key) {
      if (!strcmp(dict(d)->buckets[i].key, swp.key)) {
        found = true;
        break;
      } else if (dict(d)->buckets[i].probe < swp.probe) {
        struct bucket_data tmp;
        tmp = dict(d)->buckets[i];
        dict(d)->buckets[i] = swp;
        swp = tmp;
      }
    } else {
      dict(d)->len++;
      found = true;
      break;
    }
    swp.probe++;
  }

  // If found, place the swap bucket. Otherwise, recurse.
  if (found) {
    dict(d)->buckets[i] = swp;
  } else {
    d = dict_realloc_(siz, d, dict(d)->cap + 1);
    d = dict_set_(siz, d, swp.key, swp.data);
    free(swp.key);
    free(swp.data);
  }

  return d;
}

void *dict_get_(size_t siz, Dict d, char *a) {
  if (!siz || !d || !a)
    return (CE = CE_ARG, NULL);

  size_t hash = (sizeof(size_t) == 8) ? XXH64(a, strlen(a), 0)
              : XXH32(a, strlen(a), 0);
  size_t addr = hash % dict(d)->cap;

  for (size_t i = addr; i < dict(d)->cap; i++)
    if (dict(d)->buckets[i].key)
      if (!strcmp(dict(d)->buckets[i].key, a))
        return dict(d)->buckets[i].data;

  return (CE = CE_OOB, NULL);
}

/*
** Allocators
*/

Dict dict_alloc_(size_t siz, size_t cap) {
  if (!siz || !cap)
    return (CE = CE_ARG, NULL);
  cap = usz_primegt(cap);
  struct dict_data *dd = calloc(sizeof(*dd) + cap * sizeof(struct bucket_data), 1);
  dd->cap = cap;
  return dd->buckets;
}

Dict dict_realloc_(size_t siz, Dict d, size_t cap) {
  if (!siz || !d || !cap)
    return (CE = CE_ARG, d);
  
  // Allocate a temporary array of buckets.
  struct bucket_data *bd = malloc(dict(d)->len * sizeof(*bd));
  if (!bd)
    return (CE = CE_OOM, d);

  // Load the dictionary's buckets into the temporary bucket array.
  for (size_t i = 0; i < dict(d)->cap; i++)
    if (dict(d)->buckets[i].key)
      *(bd++) = dict(d)->buckets[i];
  bd -= dict(d)->len;

  // Reallocate the original dictionary to be the new specified size.
  cap = usz_primegt(cap);
  struct dict_data *dd = realloc(dict(d), sizeof(*dd) + cap * sizeof(*bd));
  if (!dd)
    return (CE = CE_OOM, d);
  d = dd->buckets;
  dict(d)->cap = cap;

  // Clear the dictionary's memory out.
  const size_t len = dict(d)->len;
  dict(d)->len = 0;
  memset(d, 0, cap * sizeof(*bd));

  // Load the temporary array of buckets back in to the dictionary.
  for (size_t i = 0; i < len; i++)
    d = dict_set_(siz, d, bd[i].key, bd[i].data);

  // Free the temporary bucket data.
  free(bd);

  return d;
}

Dict dict_require_(size_t siz, Dict d, size_t cap) {
  if (!siz || !d || !cap)
    return (CE = CE_ARG, d);
  return d;
}

Dict dict_free_(Dict d) {
  if (d) {
    for (size_t i = 0; i < dict(d)->cap; i++) {
      free(dict(d)->buckets[i].data);
      free(dict(d)->buckets[i].key);
    }
    free(dict(d));
  }
  return NULL;
}

#ifdef __clang__
  #pragma clang diagnostic pop
  #pragma clang diagnostic pop
  #pragma clang diagnostic pop
#endif
