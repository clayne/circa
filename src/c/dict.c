/*
** dict.c | The Circa Library Set | Dynamic Robin Hood dictionaries.
** https://github.com/davidgarland/circa
*/

#include "../h/dict.h"

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
  struct bucket_data *bd = malloc(1);
  if (!bd)
    return (CE = CE_OOM, d);
  return NULL;
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
