/*
** dict.c | The Circa Library Set | Dynamic dictionaries.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Standard */

/* Circa */

#include "../h/bits.h"
#include "../h/dict.h"

/* Vendored */

#define XXH_STATIC_LINKING_ONLY
#include "../../lib/xxhash/xxhash.h"

/*
** Accessors
*/

CIRCA CIRCA_RETURNS
Dict dict_set_(size_t siz, Dict d, char *k, void *v, bool reuse_key) {
  circa_guard (!siz || !d || !k || !v)
    return (circa_throw(CE_ARG), d);

  register const size_t k_len = strlen(k);

  register size_t swp_probe = 0, tmp_probe;

  #ifdef CIRCA_VLA
    char swp_data[siz], tmp_data[siz];
  #else
    char *restrict pool = CIRCA_MALLOC(siz * 2);
    char *restrict swp_data = pool;
    char *restrict tmp_data = pool + siz;

    if (!pool)
      return (circa_throw(CE_OOM), d);
  #endif

  char *swp_key, *tmp_key;

  memcpy(swp_data, v, siz);
  
  if (reuse_key) {
    swp_key = k;
  } else {
    swp_key = CIRCA_CALLOC(k_len + 1, 1);
    memcpy(swp_key, k, k_len);
  }

  register const size_t hash = XXH3_64bits(k, k_len) % dict(d)->cap;

  size_t i;
  bool found = false;
  for (i = hash; i < dict(d)->cap; i++) {
    if (dict(d)->key[i] != NULL) {
      if (!strcmp(dict(d)->key[i], swp_key)) {
        circa_log("found equal");
        found = true;
        break;
      } else if (dict(d)->probe[i] < swp_probe) {
        circa_log("swapping");
        // c = a
        tmp_probe = dict(d)->probe[i];
        memcpy(tmp_data, dict(d)->data + (i * siz), siz);
        tmp_key = dict(d)->key[i];
        // a = b
        dict(d)->probe[i] = swp_probe;
        memcpy(dict(d)->data + (i * siz), swp_data, siz);
        dict(d)->key[i] = swp_key;
        // b = c
        swp_probe = tmp_probe;
        memcpy(swp_data, tmp_data, siz);
        swp_key = tmp_key;
      }
    } else {
      circa_log("found empty");
      dict(d)->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    circa_log("found, inserting");
    CIRCA_FREE(dict(d)->key[i]);
    dict(d)->probe[i] = swp_probe;
    memcpy(dict(d)->data + (i * siz), swp_data, siz);
    dict(d)->key[i] = swp_key;
  } else {
    circa_log("not found, reallocating");
    d = dict_realloc_(siz, d, dict(d)->cap + 1);
    if (!CE)
      d = dict_set_(siz, d, swp_key, swp_data, true);
  }

  #ifndef CIRCA_VLA
    CIRCA_FREE(pool);
  #endif

  return d;
}

CIRCA
bool dict_del_(size_t siz, Dict d, char *k) {
  circa_guard (!siz || !d || !k)
    return (circa_throw(CE_ARG), false);

  register const size_t k_len = strlen(k);

  size_t hash = XXH3_64bits(k, k_len) % dict(d)->cap;

  for (size_t i = hash; i < dict(d)->cap; i++) {
    if (dict(d)->key[i] != NULL) {
      if (!strcmp(dict(d)->key[i], k)) {
        dict(d)->len--;
        dict(d)->probe[i] = 0;
        for (i++; i < dict(d)->cap; i++) {
          if (dict(d)->probe[i] == 0)
            return true;

          dict(d)->probe[i - 1] = dict(d)->probe[i] - 1;
          memcpy(dict(d)->data + siz * (i - 1), dict(d)->data + siz * i, siz);
          CIRCA_FREE(dict(d)->key[i - 1]);
          dict(d)->key[i - 1] = dict(d)->key[i];
          dict(d)->probe[i] = 0;
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
bool dict_has_(size_t siz, Dict d, char *k) {
  circa_guard (!siz || !d || !k)
    return (circa_throw(CE_ARG), false);

  void *p = dict_get_(siz, d, k);

  if (CE) {
    if (CE == CE_OOB) {
      CE = CE_OK;
      circa_log("caught CE_OOB from dict_get; no worries.");
    }
    return false;
  }
  
  return p != NULL;
}

CIRCA
void *dict_get_(size_t siz, Dict d, char *k) {
  circa_guard (!siz || !d || !k)
    return (circa_throw(CE_ARG), NULL);

  register const size_t k_len = strlen(k);

  size_t hash = XXH3_64bits(k, k_len) % dict(d)->cap;

  for (size_t i = hash; i < dict(d)->cap; i++) {
    bool ok_probe = (i == hash) || (dict(d)->probe[i] > 0);
    if ((dict(d)->key[i] != NULL) && ok_probe) {
      if (!strcmp(dict(d)->key[i], k)) {
        return dict(d)->data + (i * siz);
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
Dict dict_alloc_(size_t siz, size_t cap) {
  circa_guard (!siz || !cap)
    return (circa_throw(CE_ARG), NULL);

  cap = usz_primegt(cap);

  DictData *dd = CIRCA_CALLOC(sizeof(*dd) + cap * sizeof(char*), 1);

  if (!dd)
    return (circa_throw(CE_OOM), NULL);

  dd->probe = CIRCA_CALLOC(cap, sizeof(*dd->probe));
  dd->data = CIRCA_MALLOC(cap * siz);

  if (!dd->probe || !dd->data) {
    CIRCA_FREE(dd->probe);
    CIRCA_FREE(dd->data);
    CIRCA_FREE(dd);
    return (circa_throw(CE_OOM), NULL);
  }

  dd->cap = cap;

  return dd->key;
}

CIRCA CIRCA_RETURNS
Dict dict_realloc_(size_t siz, Dict d, size_t cap) {
  circa_guard (!siz || !d || !cap)
    return (circa_throw(CE_ARG), d);

  // Calculate the sizes of the input dict.
  register const size_t d_cap = dict(d)->cap;
  register const size_t d_probe_len = d_cap * sizeof(*dict(d)->probe);
  register const size_t d_key_len   = d_cap * sizeof(char*);
  register const size_t d_data_len  = d_cap * siz;

  // Allocate a temporary pool to store the dict.
  char *pool = CIRCA_MALLOC(d_probe_len + d_key_len + d_data_len);
  if (!pool)
    return (circa_throw(CE_OOM), d);

  // Set pointers to parts of the pool for temporary storage.
  size_t *probe = (size_t*) pool;
  char  **key   = (char**) (((char*) probe) + d_probe_len);
  char   *data  = ((char*) key)   + d_key_len;

  // Copy the dict's members into the pool.
  memcpy(probe, dict(d)->probe, d_probe_len);
  memcpy(key,   d,              d_key_len);
  memcpy(data,  dict(d)->data,  d_data_len);

  // Calculate the sizes of the replacement dict.
  register const size_t d2_cap = usz_primegt(cap);
  register const size_t d2_probe_len = d2_cap * sizeof(*dict(d)->probe);
  register const size_t d2_key_len   = d2_cap * sizeof(char*);
  register const size_t d2_data_len  = d2_cap * siz;

  // Reallocate the dict to the new sizes.
  DictData *dd = CIRCA_REALLOC(dict(d), sizeof(*dd) + d2_key_len);
  
  if (!dd) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), d);
  }

  dd->probe = CIRCA_REALLOC(dd->probe, d2_probe_len);

  if (!dd->probe) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), dd->key);
  }

  dd->data = CIRCA_REALLOC(dd->data, d2_data_len);

  if (!dd->data) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), dd->key);
  }

  d = dd->key;
  dd->cap = d2_cap;

  // Clear out the dict's memory.
  memset(dd->probe, 0, d2_probe_len);
  memset(d,         0,   d2_key_len);
  memset(dd->data,  0,  d2_data_len);

  // Re-insert the members into the dict.
  for (size_t i = 0; i < d_cap; i++)
    if (key[i] != NULL)
      d = dict_set_(siz, d, key[i], data + (i * siz), true);

  CIRCA_FREE(pool);

  return d;
}

CIRCA CIRCA_RETURNS
Dict dict_require_(size_t siz, Dict d, size_t cap) {
  circa_guard (!siz || !d || !cap)
    return (circa_throw(CE_ARG), d);

  return (cap > dict(d)->cap) ? dict_realloc_(siz, d, cap) : d;
}

CIRCA CIRCA_RETURNS
Dict dict_free_(size_t siz, Dict d) {
  circa_guard (!siz)
    return (circa_throw(CE_ARG), d);

  if (d) {
    // Free keys and zero out freed memory for security.
    for (size_t i = 0; i < dict(d)->cap; i++) {
      CIRCA_FREE(dict(d)->key[i]);
      dict(d)->key[i] = NULL;
    }
    memset(dict(d)->data, 0, dict(d)->cap * siz);
    
    // Free the members of the map, then the map itself.
    CIRCA_FREE(dict(d)->probe);
    CIRCA_FREE(dict(d)->data);
    CIRCA_FREE(dict(d));
  }

  return NULL;
}
