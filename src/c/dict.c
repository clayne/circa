/*
** dict.c | The Circa Library Set | Dynamic Robin Hood dictionaries.
** https://github.com/davidgarland/circa
*/

#include "../h/dict.h"

Dict dict_alloc_(size_t siz, size_t cap) {
  if (!siz || !cap)
    return NULL;
  cap = usz_primegt(cap);
  struct dict_data *dd = calloc(sizeof(*dd) + cap * sizeof(struct bucket_data), 1);
  dd->cap = cap;
  // NOTE: The length doesn't need to be set to zero, `calloc` handles that.
  return dd->buckets;
}

Dict dict_realloc_(size_t siz, Dict d, size_t cap) {
  return d;
}

Dict dict_require_(size_t siz, Dict d, size_t cap) {
  return d;
}

Dict dict_free_(size_t siz, Dict d) {
  if (d) {
    for (size_t i = 0; i < dict(d)->cap; i++) {
      free(dict(d)->buckets[i].data);
      free(dict(d)->buckets[i].key);
    }
    free(dict(d));
  }
  return NULL;
}

#include "../h/dict.h"
