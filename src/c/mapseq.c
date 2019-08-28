/*
** mapseq.c | The Circa Library Set | Dynamic sequence-valued maps.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include "../h/bits.h"
#include "../h/mapseq.h"

/* Vendored */

#define XXH_STATIC_LINKING_ONLY
#include "../../lib/xxhash/xxhash.h"

/*
** Accessors
*/

// TODO: Allow reuse of vals, similar to dict_set_.

CIRCA CIRCA_RETURNS
MapSeq mapseq_set_(size_t sizk, size_t sizv, MapSeq ms, void *k, Seq v) {
  circa_guard (!sizk || !sizv || !ms || !k || !v)
    return (circa_throw(CE_ARG), ms);

  size_t swp_probe = 0, tmp_probe;

  #ifdef CIRCA_VLA
    char swp_key[sizk], tmp_key[sizk];
  #else
    char *pool = CIRCA_MALLOC(sizk * 2);

    if (!pool)
      return (circa_throw(CE_OOM), ms);

    char *swp_key = pool;
    char *tmp_key = pool + sizk;
  #endif

  Seq swp_data, tmp_data;

  memcpy(swp_key, k, sizk);
  swp_data = seq_wrap_(sizv, v, seq(v)->len);

  size_t hash = XXH3_64bits(k, sizk) % mapseq(ms)->cap;

  size_t i;
  bool found = false;
  for (i = hash; i < mapseq(ms)->cap; i++) {
    if (mapseq(ms)->data[i] != NULL) {
      if (!memcmp(mapseq(ms)->key + (i * sizk), swp_key, sizk)) {
        circa_log("found equal");
        found = true;
        break;
      } else if (mapseq(ms)->probe[i] < swp_probe) {
        circa_log("swapping");
        // c = a
        tmp_probe = mapseq(ms)->probe[i];
        tmp_data = mapseq(ms)->data[i];
        memcpy(tmp_key, mapseq(ms)->key + (i * sizk), sizk);
        // a = b
        mapseq(ms)->probe[i] = swp_probe;
        mapseq(ms)->data[i] = swp_data;
        memcpy(mapseq(ms)->key + (i * sizk), swp_key, sizk);
        // b = c
        swp_probe = tmp_probe;
        swp_data = tmp_data;
        memcpy(swp_key, tmp_key, sizk);
      }
    } else {
      circa_log("found empty");
      mapseq(ms)->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    circa_log("found, inserting");
    mapseq(ms)->data[i] = seq_free_(sizv, mapseq(ms)->data[i]);
    memcpy(mapseq(ms)->key + (i * sizk), swp_key, sizk);
    mapseq(ms)->probe[i] = swp_probe;
    mapseq(ms)->data[i] = swp_data;
  } else {
    circa_log("not found, reallocating");
    ms = mapseq_realloc_(sizk, sizv, ms, mapseq(ms)->cap + 1);
    if (!CE)
      ms = mapseq_set_(sizk, sizv, ms, swp_key, swp_data);
    swp_data = seq_free_(sizv, swp_data);
  }

  #ifndef CIRCA_VLA
    CIRCA_FREE(pool);
  #endif

  return ms;
}

CIRCA
bool mapseq_del_(size_t sizk, size_t sizv, MapSeq ms, void *k) {
  circa_guard (!sizk || !sizv || !ms || !k)
    return (circa_throw(CE_ARG), false);

  size_t hash = XXH3_64bits(k, sizk) % mapseq(ms)->cap;

  for (size_t i = hash; i < mapseq(ms)->cap; i++) {
    if (mapseq(ms)->data[i] != NULL) {
      if (!memcmp(mapseq(ms)->key + (i * sizk), k, sizk)) {
        mapseq(ms)->len--;
        mapseq(ms)->probe[i] = 0;
        for (i++; i < mapseq(ms)->cap; i++) {
          mapseq(ms)->data[i - 1] = seq_free_(sizv, mapseq(ms)->data[i - 1]);
          mapseq(ms)->data[i - 1] = NULL;
          if (mapseq(ms)->probe[i] == 0)
            return true;
          mapseq(ms)->probe[i - 1] = mapseq(ms)->probe[i] - 1;
          mapseq(ms)->data[i - 1] = mapseq(ms)->data[i];
          memcpy(mapseq(ms)->key + sizk * (i - 1), mapseq(ms)->key + sizk * i, sizk);
          mapseq(ms)->probe[i] = 0;
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
bool mapseq_has_(size_t sizk, size_t sizv, MapSeq ms, void *k) {
  circa_guard (!sizk || !sizv || !ms || !k)
    return (circa_throw(CE_ARG), false);

  void *p = mapseq_get_(sizk, sizv, ms, k);

  if (CE) {
    if (CE == CE_OOB) {
      CE = CE_OK;
      circa_log("caught CE_OOB from mapseq_get; no worries.");
    }
    return false;
  }

  return p != NULL;
}

CIRCA
Seq mapseq_get_(size_t sizk, size_t sizv, MapSeq ms, void *k) {
  circa_guard (!sizk || !sizv || !ms || !k)
    return (circa_throw(CE_ARG), NULL);

  size_t hash = XXH3_64bits(k, sizk) % mapseq(ms)->cap;

  for (size_t i = hash; i < mapseq(ms)->cap; i++) {
    bool ok_probe = (i == hash) || (mapseq(ms)->probe[i] > 0);
    if ((mapseq(ms)->data[i] != NULL) && ok_probe) {
      if (!memcmp(mapseq(ms)->key + (i * sizk), k, sizk)) {
        return mapseq(ms)->data[i];
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
MapSeq mapseq_alloc_(size_t sizk, size_t sizv, size_t cap) {
  circa_guard (!sizk || !sizv || !cap)
    return (circa_throw(CE_ARG), NULL);

  cap = usz_primegt(cap);

  MapSeqData *msd = CIRCA_CALLOC(sizeof(*msd) + cap * sizk, 1);

  if (!msd)
    return (circa_throw(CE_OOM), NULL);

  msd->probe = CIRCA_CALLOC(cap, sizeof(*msd->probe));
  msd->data = CIRCA_CALLOC(cap, sizeof(Seq));

  if (!msd->probe || !msd->data) {
    CIRCA_FREE(msd->probe);
    CIRCA_FREE(msd->data);
    CIRCA_FREE(msd);
    return (circa_throw(CE_OOM), NULL);
  }

  msd->cap = cap;

  return msd->key;
}

CIRCA CIRCA_RETURNS
MapSeq mapseq_realloc_(size_t sizk, size_t sizv, MapSeq ms, size_t cap) {
  circa_guard (!sizk || !sizv || !ms || !cap)
    return (circa_throw(CE_ARG), ms);

  // Calculate the sizes of the input map.
  register const size_t ms_cap = mapseq(ms)->cap;
  register const size_t ms_probe_len = ms_cap * sizeof(*mapseq(ms)->probe);
  register const size_t ms_key_len   = ms_cap * sizk;
  register const size_t ms_data_len  = ms_cap * sizeof(Seq);

  // Allocate a temporary memory pool to store the map.
  char *pool = CIRCA_MALLOC(ms_probe_len + ms_key_len + ms_data_len);
  if (!pool)
    return (circa_throw(CE_OOM), ms);

  // Set pointers to parts of the pool for temporary storage.
  size_t *probe = (size_t*) pool;
  char   *key   = ((char*) probe) + ms_probe_len;
  Seq    *data  = (Seq*) (key + ms_key_len);

  // Copy the map's members into the pool.
  memcpy(probe, mapseq(ms)->probe, ms_probe_len);
  memcpy(key,   ms,                  ms_key_len);
  memcpy(data,  mapseq(ms)->data,   ms_data_len);

  // Calculate the sizes of the replacement map.
  register const size_t ms2_cap = usz_primegt(cap);
  register const size_t ms2_probe_len = ms2_cap * sizeof(*mapseq(ms)->probe);
  register const size_t ms2_key_len   = ms2_cap * sizk;
  register const size_t ms2_data_len  = ms2_cap * sizeof(Seq);

  // Reallocate the map to the new sizes.
  MapSeqData *msd = CIRCA_REALLOC(mapseq(ms), sizeof(*msd) + ms2_key_len);

  if (!msd) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), ms);
  }

  msd->probe = CIRCA_REALLOC(msd->probe, ms2_probe_len);

  if (!msd->probe) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), ms);
  }

  msd->data = CIRCA_REALLOC(msd->data, ms2_data_len);

  if (!msd->data) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), ms);
  }

  ms = msd->key;
  msd->cap = ms2_cap;

  // Clear out the map's memory.
  memset(msd->probe, 0, ms2_probe_len);
  memset(ms,         0, ms2_key_len);
  memset(msd->data,  0, ms2_data_len);

  // Re-insert the members into the map.
  circa_log("rehashing...");
  for (size_t i = 0; i < ms_cap; i++) {
    if (data[i] != NULL) {
      ms = mapseq_set_(sizk, sizv, ms, key + (i * sizk), data[i]);
      data[i] = seq_free_(sizv, data[i]);
    }
  }
  circa_log("finished rehashing.");

  CIRCA_FREE(pool);

  return ms;
}

CIRCA CIRCA_RETURNS
MapSeq mapseq_require_(size_t sizk, size_t sizv, MapSeq ms, size_t cap) {
  circa_guard (!sizk || !sizv || !ms || !cap)
    return (circa_throw(CE_ARG), ms);

  return (cap > mapseq(ms)->cap) ? mapseq_realloc_(sizk, sizv, ms, cap) : ms;
}

CIRCA CIRCA_RETURNS
MapSeq mapseq_free_(size_t sizk, size_t sizv, MapSeq ms) {
  circa_guard (!sizk || !sizv)
    return (circa_throw(CE_ARG), ms);

  if (ms) {
    // Free values and zero out freed memory for security.
    for (size_t i = 0; i < mapseq(ms)->cap; i++)
      mapseq(ms)->data[i] = seq_free_(sizv, mapseq(ms)->data[i]);
    memset(mapseq(ms)->key, 0, mapseq(ms)->cap * sizk);

    // Free the mem bers of the map, then the map itself.
    CIRCA_FREE(mapseq(ms)->probe);
    CIRCA_FREE(mapseq(ms)->data);
    CIRCA_FREE(mapseq(ms));
  }

  return NULL;
}
