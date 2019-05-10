/*
** map.c | The Circa Library Set | Dual-Generic Maps
** https://github.com/davidgarland/circa
*/

#define XXH_STATIC_LINKING_ONLY
#include "xxhash.h"

#include "seq.h"
#include "map.h"

Map map_set_(size_t sizk, size_t sizv, Map m, void *k, void *v) {
  ce_guard (!sizk || !sizv || !m || !k || !v)
    return (CE = CE_ARG, m);

  // Avoid redundant calls.
  struct map_data *md = map(m);

  // And avoid redundant dereferences.
  const size_t m_cap = md->cap;

  // Construct two buckets for shuffling values around. Use VLAs if possible.
  bool swp_used = true, tmp_used;
  bool swp_probe = 0, tmp_probe;
  #ifdef CIRCA_VLA
    char  swp_data[sizv], tmp_data[sizv];
    char  swp_key[sizk],  tmp_key[sizk];
  #else
    char *swp_data,      *tmp_data;
    char *swp_key,       *tmp_key;
  
    swp_data = malloc(sizv);
    swp_key  = malloc(sizk);
    tmp_data = malloc(sizv);
    tmp_key  = malloc(sizk);

    if (!swp_data || !swp_key || !tmp_data || !tmp_key) {
      free(swp_data);
      free(swp_key);
      free(tmp_data);
      free(tmp_key);
      return (CE = CE_OOM, m);
    }
  #endif

  // Copy the key and value into the swap bucket.
  memcpy(swp_data, v, sizv);
  memcpy(swp_key,  k, sizk);

  // Calculate the starting position using xxHash.
  const size_t hash = (sizeof(size_t) == 8) ? XXH64(k, sizk, 0)
                    : XXH32(k, sizk, 0);
  const size_t addr = hash % m_cap;

  size_t i;
  bool found = false;
  for (i = addr; i < m_cap; i++) {
    if (md->used[i]) {
      if (!memcmp(md->key + (i * sizk), swp_key, sizk)) {
        found = true;
        break;
      } else if (md->probe[i] < swp_probe) {
        // c := a
        tmp_used  = md->used[i];
        tmp_probe = md->probe[i];
        memcpy(tmp_data, md->data + (i * sizv), sizv);
        memcpy(tmp_key,  md->key + (i * sizk), sizk);
        // a := b
        md->used[i]  = swp_used;
        md->probe[i] = swp_probe;
        memcpy(md->data + (i * sizv), swp_data, sizv);
        memcpy(md->key + (i * sizk), swp_key, sizk);
        // b := c
        swp_used  = tmp_used;
        swp_probe = tmp_probe;
        memcpy(swp_data, tmp_data, sizv);
        memcpy(swp_key, tmp_key, sizk);
      }
    } else {
      md->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    md->probe[i] = swp_probe;
    md->used[i]  = swp_used;
    memcpy(md->data + (i * sizv), swp_data, sizv);
    memcpy(md->key + (i * sizk),  swp_key,  sizk);
  } else {
    m = map_realloc_(sizk, sizv, m, m_cap + 1);
    if (CE) {
      #ifndef CIRCA_VLA
        free(swp_data);
        free(swp_key);
        free(tmp_data);
        free(tmp_key);
      #endif
      return m;
    }
    m = map_set_(sizk, sizv, m, swp_key, swp_data);
  }

  #ifndef CIRCA_VLA
    free(swp_data);
    free(swp_key);
    free(tmp_data);
    free(tmp_key);
  #endif

  return m;
}

bool map_del_(size_t sizk, size_t sizv, Map m, void *k) {
  struct map_data *const md = map(m);

  const size_t m_cap = md->cap;

  const size_t hash = (sizeof(size_t) == 8) ? XXH64(k, sizk, 0)
                    : XXH32(k, sizk, 0);
  const size_t addr = hash % m_cap;

  for (size_t i = addr; i < m_cap; i++) {
    if (md->used[i]) {
      if (!memcmp(md->key + (i * sizk), k, sizk)) {
        map(m)->len--;
        md->used[i] = false;
        md->probe[i] = 0;
        for (i++; i < m_cap; i++) {
          // If the current bucket is not used or is a root, return.
          if (!md->used[i] || md->probe[i] == 0)
            return true;
          // Move the current bucket into the last bucket.
          md->probe[i - 1] = md->probe[i] - 1;
          md->data[i - 1] = md->data[i];
          memcpy(md->key + sizk * (i - 1), md->key + sizk * i, sizk);
          // Make the current bucket appear unused.
          md->used[i]  = false;
          md->probe[i] = 0;
        }
        return true;
      }
    } else {
      return false;
    }
  }

  return false;
}

bool map_has_(size_t sizk, size_t sizv, Map m, void *k) {
  void *p = map_get_(sizk, sizv, m, k);
  if (CE) {
    if (CE == CE_OOB)
      CE = CE_OK;
    return false;
  } else {
    return p != NULL;
  }
}

void *map_get_(size_t sizk, size_t sizv, Map m, void *k) {
  ce_guard (!sizk || !sizv || !m || !k)
    return (CE = CE_ARG, NULL);

  struct map_data *md = map(m);
  
  const size_t m_cap = md->cap;

  const size_t hash = (sizeof(size_t) == 8) ? XXH64(k, sizk, 0)
                    : XXH32(k, sizk, 0);
  const size_t addr = hash % m_cap;

  for (size_t i = addr; i < m_cap; i++) {
    if (md->used[i]) {
      if (!memcmp(md->key + (i * sizk), k, sizk)) {
        return md->data + (i * sizv);
      }
    } else {
      return (CE = CE_OOB, NULL);
    }
  }

  return (CE = CE_OOB, NULL);
}

Map map_alloc_(size_t sizk, size_t sizv, size_t cap) {
  ce_guard (!sizk || !sizv || !cap)
    return (CE = CE_ARG, NULL);

  cap = usz_primegt(cap);
 
  struct map_data *md = calloc(sizeof(*md) + cap * sizk, 1);

  if (!md)
    return (CE = CE_OOM, NULL);

  md->used  = calloc(cap, 1);
  md->probe = calloc(cap, sizeof(size_t));
  md->data  = calloc(cap, sizv);

  if (!md->probe || !md->data) {
    free(md->used);
    free(md->probe);
    free(md->data);
    free(md);
    return (CE = CE_OOM, NULL);
  }

  md->cap = cap;

  return md->key;
}

Map map_realloc_(size_t sizk, size_t sizv, Map m, size_t cap) {
  ce_guard (!sizk || !sizv || !m || !cap)
    return (CE = CE_ARG, NULL);

  // Let's not call this function a million times, sounds good.
  struct map_data *md = map(m);

  // Calculate the size of the current map's fields.
  const size_t m_cap   = md->cap;
  const size_t m_data  = m_cap * sizv;
  const size_t m_key   = m_cap * sizk;

  // Construct a temporary "fake" map based on these sizes.
  struct {
    bool   *used;
    char   *data;
    char   *key;
  } tmp = {
    .used  = malloc(m_cap),
    .data  = malloc(m_data),
    .key   = malloc(m_key)
  };

  // If the allocation doesn't succeed, raise an OOM error.
  if (!tmp.used || !tmp.data || !tmp.key) {
    free(tmp.used);
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, m);
  }

  // If it did, however, load the map into the temporary "fake" map.
  memcpy(tmp.used,  md->used,  m_cap);
  memcpy(tmp.data,  md->data,  m_data);
  memcpy(tmp.key,   md->key,   m_key);

  // Calculate the size of the map's resized fields.
  const size_t m2_cap   = usz_primegt(cap);
  const size_t m2_probe = m2_cap * sizeof(size_t);
  const size_t m2_data  = m2_cap * sizv;
  const size_t m2_key   = m2_cap * sizk;
  
  // Reallocate the original map to the new sizes.
  md = realloc(md, sizeof(*md) + m2_key);
  
  if (!md) {
    free(tmp.used);
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, m);
  }

  md->used  = realloc(md->used,  m2_cap);

  if (!md->used) {
    free(tmp.used);
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, m);
  }

  md->probe = realloc(md->probe, m2_probe);

  if (!md->probe) {
    free(tmp.used);
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, m);
  }

  md->data  = realloc(md->data,  m2_data);

  if (!md->data) {
    free(tmp.used);
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, m);
  }
  
  m = md->key;
  md->cap = m2_cap;

  // Clear out the map's memory.
  memset(md->used,  0, m2_cap);
  memset(md->probe, 0, m2_probe);
  memset(md->data,  0, m2_data);
  memset(m,         0, m2_key);

  // Load the data back into the map.
  for (size_t i = 0; i < m_cap; i++)
    if (tmp.used[i])
      m = map_set_(sizk, sizv, m, tmp.key + (i * sizk), tmp.data + (i * sizv));

  // Free the temporary "fake" map.
  free(tmp.used);
  free(tmp.data);
  free(tmp.key);

  return m;
}

Map map_require_(size_t sizk, size_t sizv, Map m, size_t cap) {
  return map(m)->cap < cap ? map_realloc_(sizk, sizv, m, cap) : m;
}

Map map_free_(Map m) {
  if (m) {
    free(map(m)->used);
    free(map(m)->probe);
    free(map(m)->data);
    free(map(m));
  }
  return NULL;
}
