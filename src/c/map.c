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
#include "../h/map.h"

/* Vendored */

#define XXH_STATIC_LINKING_ONLY
#include "../../lib/xxhash/xxhash.h"

/*
** Accessors
*/

CIRCA CIRCA_RETURNS
Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v) {
  circa_guard (!sizk || !sizv || !m || !k || !v)
    return (circa_throw(CE_ARG), m);

  intmax_t swp_probe = -1, tmp_probe;

  #ifdef CIRCA_VLA
    char swp_data[sizv], tmp_data[sizv];
    char swp_key[sizk],  tmp_key[sizk];
  #else
    void *pool = malloc((sizv * 2) + (sizk * 2));

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
    free(pool);
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
          if (map(m)->probe[i] <= 0)
            return true;
          map(m)->probe[i - 1] = map(m)->probe[i] - 1;
          map(m)->data[i - 1] = map(m)->data[i];
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
  } else {
    return p != NULL;
  }
}

CIRCA
void *map_get_(size_t sizk, size_t sizv, Map m, void *k) {
  circa_guard (!sizk || !sizv || !m || !k)
    return (circa_throw(CE_ARG), NULL);

  size_t hash = XXH3_64bits(k, sizk);

  for (size_t i = hash; i < map(m)->cap; i++) {
    if (map(m)->probe[i] != -1) {
      if (!memcmp(map(m)->key + (i * sizk), k, sizk)) {
        return map(m)->data + (i * sizv);
      }
    } else {
      return (circa_throw(CE_OOB), NULL);
    }
  }

  return (circa_throw(CE_OOB), NULL);
}

