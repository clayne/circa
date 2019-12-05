/*
** map.c | The Circa Library Set | Dynamic maps.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Standard */

#include <string.h>

/* Circa */

#include "../h/core.h"
#include "../h/debug.h"
#include "../h/bits.h"
#include "../h/map.h"

/* Vendored */

#ifdef CIRCA_HEADER_ONLY
  #define XXH_INLINE_ALL
#else
  #define XXH_STATIC_LINKING_ONLY
#endif

#include "../../lib/xxhash/xxhash.h"

/*
** Accessors
*/

CIRCA CIRCA_RETURNS
Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v) {
  circa_guard (!sizk || !sizv || !m || !k || !v)
    return (circa_throw(CE_ARG), m);

  intmax_t swp_probe = 0, tmp_probe;

  #ifdef CIRCA_VLA
    char swp_data[sizv], tmp_data[sizv];
    char swp_key[sizk],  tmp_key[sizk];
  #else
    char *pool = CIRCA_MALLOC((sizv * 2) + (sizk * 2));

    if (!pool)
      return (circa_throw(CE_OOM), m);

    char *swp_data = pool + (0 * sizk) + (0 * sizv);
    char *tmp_data = pool + (0 * sizk) + (1 * sizv);
    char *swp_key  = pool + (0 * sizk) + (2 * sizv);
    char *tmp_key  = pool + (1 * sizk) + (2 * sizv);
  #endif

  memcpy(swp_data, v, sizv);
  memcpy(swp_key,  k, sizk);

  size_t hash = XXH3_64bits(k, sizk) % map(m)->cap;

  size_t i;
  bool found = false;
  for (i = hash; i < map(m)->cap; i++) {
    if (map(m)->probe[i] != -1) {
      if (!memcmp(map(m)->key + (i * sizk), swp_key, sizk)) {
        found = true;
        break;
      } else if (map(m)->probe[i] < swp_probe) {
        // c = a
        tmp_probe = map(m)->probe[i];
        memcpy(tmp_data, map(m)->data + (i * sizv), sizv);
        memcpy(tmp_key,  map(m)->key  + (i * sizk), sizk);
        // a = b
        map(m)->probe[i] = swp_probe;
        memcpy(map(m)->data + (i * sizv), swp_data, sizv);
        memcpy(map(m)->key  + (i * sizk), swp_key,  sizk);
        // b = c
        swp_probe = tmp_probe;
        memcpy(swp_data, tmp_data, sizv);
        memcpy(swp_key,  tmp_key,  sizk);
      }
    } else {
      map(m)->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    map(m)->probe[i] = swp_probe;
    memcpy(map(m)->data + (i * sizv), swp_data, sizv);
    memcpy(map(m)->key  + (i * sizk), swp_key,  sizk);
  } else {
    m = map_realloc_(sizk, sizv, m, map(m)->cap + 1);
    if (!CE)
      m = map_set_(sizk, sizv, m, swp_key, swp_data);
  }

  #ifndef CIRCA_VLA
    CIRCA_FREE(pool);
  #endif

  return m;
}

CIRCA
bool map_del_(size_t sizk, size_t sizv, Map m, void *k) {
  circa_guard (!sizk || !sizv || !m || !k)
    return (circa_throw(CE_ARG), m);

  size_t hash = XXH3_64bits(k, sizk) % map(m)->cap;

  for (size_t i = hash; i < map(m)->cap; i++) {
    if (map(m)->probe[i] != -1) {
      if (!memcmp(map(m)->key + (i * sizk), k, sizk)) {
        map(m)->len--;
        map(m)->probe[i] = -1;
        for (i++; i < map(m)->cap; i++) {
          map(m)->probe[i - 1] = -1;
          
          if (map(m)->probe[i] <= 0)
            return true;

          map(m)->probe[i - 1] = map(m)->probe[i] - 1;
          memcpy(map(m)->data + sizv * (i - 1), map(m)->data + sizv * i, sizv);
          memcpy(map(m)->key + sizk * (i - 1), map(m)->key + sizk * i, sizk); 
          map(m)->probe[i] = -1;
        }
        return true;
      }
    } else {
      return false;
    }
  }

  return false;
}

CIRCA
bool map_has_(size_t sizk, size_t sizv, Map m, void *k) {
  circa_guard (!sizk || !sizv || !m || !k)
    return (circa_throw(CE_ARG), false);
  
  void *p = map_get_(sizk, sizv, m, k);

  if (CE) {
    if (CE == CE_OOB)
      CE = CE_OK;
    circa_log("caught CE_OOB from map_get; no worries.");
    return false;
  }
  
  return p != NULL;
}

CIRCA
void *map_get_(size_t sizk, size_t sizv, Map m, void *k) {
  circa_guard (!sizk || !sizv || !m || !k)
    return (circa_throw(CE_ARG), NULL);

  size_t hash = XXH3_64bits(k, sizk) % map(m)->cap;

  for (size_t i = hash; i < map(m)->cap; i++) {
    bool ok_probe = (i == hash) || (map(m)->probe[i] > 0);
    if ((map(m)->probe[i] != -1) && ok_probe) {
      if (!memcmp(map(m)->key + (i * sizk), k, sizk)) {
        return map(m)->data + (i * sizv);
      }
    } else {
      return (circa_throw(CE_OOB), NULL);
    }
  }

  return (circa_throw(CE_OOB), NULL);
}

/*
** Allocators
*/

CIRCA CIRCA_ALLOCS
Map map_alloc_(size_t sizk, size_t sizv, size_t cap) {
  circa_guard (!sizk || !sizv || !cap)
    return (circa_throw(CE_ARG), NULL);

  cap = usz_primegt(cap);

  MapData *md = CIRCA_CALLOC(sizeof(*md) + cap * sizk, 1);

  if (!md)
    return (circa_throw(CE_OOM), NULL);

  md->probe = CIRCA_MALLOC(cap * sizeof(*md->probe));
  md->data  = CIRCA_MALLOC(cap * sizv);

  if (!md->probe || !md->data) { 
    CIRCA_FREE(md->probe);
    CIRCA_FREE(md->data);
    CIRCA_FREE(md);
    return (circa_throw(CE_OOM), NULL);
  }

  for (size_t i = 0; i < cap; i++)
    md->probe[i] = -1;

  md->cap = cap;

  return md->key;
}

CIRCA CIRCA_RETURNS
Map map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap) {
  circa_guard (!sizk || !sizv || !m || !cap)
    return (circa_throw(CE_ARG), m);

  // Calculate the sizes of the input map.
  register const size_t m_cap = map(m)->cap;
  register const size_t m_probe_len = m_cap * sizeof(*map(m)->probe);
  register const size_t m_key_len   = m_cap * sizk;
  register const size_t m_data_len  = m_cap * sizv;

  // Allocate a temporary memory pool to store the map.
  char *pool = CIRCA_MALLOC(m_probe_len + m_key_len + m_data_len);
  if (!pool)
    return (circa_throw(CE_OOM), m);

  // Set pointers to parts of the pool for temporary storage.
  size_t *probe = (size_t*) pool;
  char   *key   = ((char*) probe) + m_probe_len;
  char   *data  = key   +   m_key_len;

  // Copy the map's members into the pool.
  memcpy(probe, map(m)->probe, m_probe_len);
  memcpy(key,   m,               m_key_len);
  memcpy(data,  map(m)->data,   m_data_len);

  // Calcualte the sizes of the replacement map.
  register const size_t m2_cap       = usz_primegt(cap);
  register const size_t m2_probe_len = m2_cap * sizeof(*map(m)->probe);
  register const size_t m2_key_len   = m2_cap * sizk;
  register const size_t m2_data_len  = m2_cap * sizv;

  // Reallocate the map to the new sizes.
  MapData *md = CIRCA_REALLOC(map(m), sizeof(*md) + m2_key_len);

  if (!md) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), m);
  }

  md->probe = CIRCA_REALLOC(md->probe, m2_probe_len);

  if (!md->probe) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), m);
  }

  md->data = CIRCA_REALLOC(md->data, m2_data_len);
  
  if (!md->data) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), m);
  }

  m = md->key;
  md->cap = m2_cap;

  // Clear out the map's memory.
  for (size_t i = 0; i < m2_cap; i++)
    md->probe[i] = -1;
  memset(m, 0, m2_key_len);
  memset(md->data, 0, m2_data_len);

  // Re-insert the members into the map.
  for (size_t i = 0; i < m_cap; i++)
    if (probe[i] != -1)
      m = map_set_(sizk, sizv, m, key + (i * sizk), data + (i * sizv));

  CIRCA_FREE(pool);

  return m;
}

CIRCA CIRCA_RETURNS
Map map_require_(size_t sizk, size_t sizv, Map m, size_t cap) {
  return map(m)->cap < cap ? map_realloc_(sizk, sizv, m, cap) : m;
}

CIRCA CIRCA_RETURNS
Map map_free_(size_t sizk, size_t sizv, Map m) {
  if (m) {
    // Zero out freed memory for security. We don't bother with probe count.
    memset(m, 0, map(m)->cap * sizk);
    memset(map(m)->data, 0, map(m)->cap * sizv);

    // Free the members of the map, then the map itself.
    CIRCA_FREE(map(m)->probe);
    CIRCA_FREE(map(m)->data);
    CIRCA_FREE(map(m));
  }

  return NULL;
}
