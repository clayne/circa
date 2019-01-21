/*
** map.c | The Circa Library Set | Dual-Generic Maps
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
#define XXH_NAMESPACE CIRCA_MAP
#include "../../lib/xxhash/xxhash.h"

#include "../h/map.h"

Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v) {
  if (!sizk || !sizv || !m || !k || !v)
    return (CE = CE_ARG, m);

  // Set up a swap bucket.
  struct map_bucket swp = {
    .data    = malloc(sizv),
    .key     = malloc(sizk),
    .probe   = 0,
  };

  if (!swp.data || !swp.key)
    return (CE = CE_OOM, m);

  memcpy(swp.data, v, sizv);
  memcpy(swp.key, k, sizk);

  // Calculate the starting position using xxHash.
  size_t hash = (sizeof(size_t) == 8) ? XXH64(k, sizk, 0)
              : XXH32(k, sizk, 0);
  size_t addr = hash % map(m)->cap;

  // Traverse the map in search of a position for `swp`.
  size_t i;
  bool found = false;
  for (i = addr; i < map(m)->cap; i++) {
    if (map(m)->buckets[i].key) {
      if (!memcmp(map(m)->buckets[i].key, swp.key, sizk)) {
        found = true;
        break;
      } else if (map(m)->buckets[i].probe < swp.probe) {
        struct map_bucket tmp;
        tmp = map(m)->buckets[i];
        map(m)->buckets[i] = swp;
        swp = tmp;
      }
    } else {
      map(m)->len++;
      found = true;
      break;
    }
    swp.probe++;
  }

  // If found, place the swap bucket. Otherwise, recurse.
  if (found) {
    free(map(m)->buckets[i].key);
    free(map(m)->buckets[i].data);
    map(m)->buckets[i] = swp;
  } else {
    m = map_realloc_(sizk, sizv, m, map(m)->cap + 1);
    m = map_set_(sizk, sizv, m, swp.key, swp.data);
    free(swp.key);
    free(swp.data);
  }

  return m;
}

bool map_has_(size_t sizk, size_t sizv, Map m, void *k) {
  return map_get_(sizk, sizv, m, k) ? true
       : (CE = CE_OK, false);
}

void *map_get_(size_t sizk, size_t sizv, Map m, void *k) {
  if (!sizk || !sizv || !m || !k)
    return (CE = CE_ARG, NULL);

  // Calculate the starting position using xxHash.
  size_t hash = (sizeof(size_t) == 8) ? XXH64(k, sizk, 0)
              : XXH32(k, sizk, 0);
  size_t addr = hash % map(m)->cap;

  // Traverse the map for a match.
  for (size_t i = addr; i < map(m)->cap; i++)
    if (map(m)->buckets[i].key)
      if (!memcmp(map(m)->buckets[i].key, k, sizk))
        return map(m)->buckets[i].data;

  // If no match is found, throw an out of bounds exception.
  return (CE = CE_OOB, NULL);
}

Map map_alloc_(size_t sizk, size_t sizv, size_t cap) {
  if (!sizk || !sizv || !cap)
    return (CE = CE_ARG, NULL);

  cap = usz_primegt(cap);
  struct map_data *md = calloc(sizeof(*md) + cap * sizeof(struct map_bucket), 1);
  md->cap = cap;
  return md->buckets;
}

Map map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap) {
  if (!sizk || !sizv || !m || !cap)
    return (CE = CE_ARG, m);

  // Allocate a temporary array of buckets.
  struct map_bucket *bd = malloc(map(m)->len * sizeof(*bd));
  if (!bd)
    return (CE = CE_OOM, m);

  // Load the map's buckets into the temporary bucket array.
  for (size_t i = 0; i < map(m)->cap; i++)
    if (map(m)->buckets[i].key)
      *(bd++) = map(m)->buckets[i];
  bd -= map(m)->len;

  // Reallocate the original map to be the new specified size.
  cap = usz_primegt(cap);
  struct map_data *md = realloc(map(m), sizeof(*md) + cap * sizeof(*bd));
  if (!md)
    return (CE = CE_OOM, m);
  m = md->buckets;
  map(m)->cap = cap;

  // Clear the map's memory out.
  const size_t len = map(m)->len;
  map(m)->len = 0;
  memset(m, 0, cap * sizeof(*bd));

  // Load the temporary array of buckets back into the map.
  for (size_t i = 0; i < len; i++) {
    m = map_set_(sizk, sizv, m, bd[i].key, bd[i].data);
    free(bd[i].data);
    free(bd[i].key);
  }

  // Free the temporary bucket data.
  free(bd);

  return m;
}

Map map_require_(size_t sizk, size_t sizv, Map m, size_t cap) {
  if (!sizk || !sizv || !m || !cap)
    return (CE = CE_ARG, m);
  return map(m)->cap < cap ? map_realloc_(sizk, sizv, m, cap) : m;
}

Map map_free_(Map m) {
  if (m) {
    for (size_t i = 0; i < map(m)->cap; i++) {
      free(map(m)->buckets[i].data);
      free(map(m)->buckets[i].key);
    }
    free(map(m));
  }
  return NULL;
}

#ifdef __clang__
  #pragma clang diagnostic pop
  #pragma clang diagnostic pop
  #pragma clang diagnostic pop
  #pragma clang diagnostic pop
#endif
