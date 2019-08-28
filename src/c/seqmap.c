/*
** seqmap.c | The Circa Library Set | Dynamic sequence-keyed maps.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include "../h/bits.h"
#include "../h/seqmap.h"

/* Vendored */

#define XXH_STATIC_LINKING_ONLY
#include "../../lib/xxhash/xxhash.h"

/*
** Accessors
*/

// TODO: Allow reuse of keys, similar to dict_set_.

CIRCA CIRCA_RETURNS
SeqMap seqmap_set_(size_t sizk, size_t sizv, SeqMap sm, Seq k, void *v) {
  circa_guard (!sizk || !sizv || !sm || !k || !v)
    return (circa_throw(CE_ARG), sm);

  size_t swp_probe = 0, tmp_probe;

  #ifdef CIRCA_VLA
    char swp_data[sizv], tmp_data[sizv];
  #else
    char *pool = CIRCA_MALLOC(sizv * 2);

    if (!pool)
      return (circa_throw(CE_OOM), sm);

    char *swp_data = pool;
    char *tmp_data = pool + sizv;
  #endif

  Seq swp_key, tmp_key;

  memcpy(swp_data, v, sizv);
  swp_key = seq_wrap_(sizk, k, seq(k)->len);

  size_t hash = XXH3_64bits(k, seq(k)->len * sizk) % seqmap(sm)->cap;

  size_t i;
  bool found = false;
  for (i = hash; i < seqmap(sm)->cap; i++) {
    if (seqmap(sm)->key[i] != NULL) {
      if (seq_cmp_(sizk, seqmap(sm)->key[i], swp_key)) {
        circa_log("found equal");
        found = true;
        break;
      } else if (seqmap(sm)->probe[i] < swp_probe) {
        circa_log("swapping");
        // c = a
        tmp_probe = seqmap(sm)->probe[i];
        memcpy(tmp_data, seqmap(sm)->data + (i * sizv), sizv);
        tmp_key = seqmap(sm)->key[i];
        // a = b
        seqmap(sm)->probe[i] = swp_probe;
        memcpy(seqmap(sm)->data + (i * sizv), swp_data, sizv);
        seqmap(sm)->key[i] = swp_key;
        // b = c
        swp_probe = tmp_probe;
        memcpy(swp_data, tmp_data, sizv);
        swp_key = tmp_key;
      }
    } else {
      circa_log("found empty");
      seqmap(sm)->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    circa_log("found, inserting");
    seqmap(sm)->key[i] = seq_free_(sizk, seqmap(sm)->key[i]);
    seqmap(sm)->probe[i] = swp_probe;
    memcpy(seqmap(sm)->data + (i * sizv), swp_data, sizv);
    seqmap(sm)->key[i] = swp_key;
  } else {
    circa_log("not found, reallocating");
    sm = seqmap_realloc_(sizk, sizv, sm, seqmap(sm)->cap + 1);
    if (!CE)
      sm = seqmap_set_(sizk, sizv, sm, swp_key, swp_data);
    swp_key = seq_free_(sizk, swp_key);
  }

  #ifndef CIRCA_VLA
    CIRCA_FREE(pool);
  #endif

  return sm;
}

CIRCA
bool seqmap_del_(size_t sizk, size_t sizv, SeqMap sm, Seq k) {
  circa_guard (!sizk || !sizv || !sm || !k)
    return (circa_throw(CE_ARG), false);

  size_t hash = XXH3_64bits(k, seq(k)->len * sizk) % seqmap(sm)->cap;

  for (size_t i = hash; i < seqmap(sm)->cap; i++) {
    if (seqmap(sm)->key[i] != NULL) {
      if (seq_cmp_(sizk, seqmap(sm)->key[i], k)) {
        seqmap(sm)->len--;
        seqmap(sm)->probe[i] = 0;
        for (i++; i < seqmap(sm)->cap; i++) {
          seqmap(sm)->key[i - 1] = seq_free_(sizk, seqmap(sm)->key[i - 1]);
          seqmap(sm)->key[i - 1] = NULL;
          if (seqmap(sm)->probe[i] == 0)
            return true;
          seqmap(sm)->probe[i - 1] = seqmap(sm)->probe[i] - 1;
          memcpy(seqmap(sm)->data + sizv * (i - 1), seqmap(sm)->data + sizv * i, sizv);
          seqmap(sm)->key[i - 1] = seqmap(sm)->key[i];
          seqmap(sm)->probe[i] = 0;
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
bool seqmap_has_(size_t sizk, size_t sizv, SeqMap sm, Seq k) {
  circa_guard (!sizk || !sizv || !sm || !k)
    return (circa_throw(CE_ARG), false);

  void *p = seqmap_get_(sizk, sizv, sm, k);

  if (CE) {
    if (CE == CE_OOB) {
      CE = CE_OK;
      circa_log("caught CE_OOB from seqmap_get; no worries.");
    }
    return false;
  }

  return p != NULL;
}

CIRCA
void *seqmap_get_(size_t sizk, size_t sizv, SeqMap sm, Seq k) {
  circa_guard (!sizk || !sizv || !sm || !k)
    return (circa_throw(CE_ARG), NULL);

  size_t hash = XXH3_64bits(k, seq(k)->len * sizk) % seqmap(sm)->cap;

  for (size_t i = hash; i < seqmap(sm)->cap; i++) {
    bool ok_probe = (i == hash) || (seqmap(sm)->probe[i] > 0);
    if ((seqmap(sm)->key[i] != NULL) && ok_probe) {
      if (seq_cmp_(sizk, seqmap(sm)->key[i], k)) {
        return seqmap(sm)->data + (i * sizv);
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
SeqMap seqmap_alloc_(size_t sizk, size_t sizv, size_t cap) {
  circa_guard (!sizk || !sizv || !cap)
    return (circa_throw(CE_ARG), NULL);

  cap = usz_primegt(cap);

  SeqMapData *smd = CIRCA_CALLOC(sizeof(*smd) + cap * sizeof(Seq), 1);

  if (!smd)
    return (circa_throw(CE_OOM), NULL);

  smd->probe = CIRCA_CALLOC(cap, sizeof(*smd->probe));
  smd->data = CIRCA_MALLOC(cap * sizv);

  if (!smd->probe || !smd->data) {
    CIRCA_FREE(smd->probe);
    CIRCA_FREE(smd->data);
    CIRCA_FREE(smd);
    return (circa_throw(CE_OOM), NULL);
  }

  smd->cap = cap;

  return smd->key;
}

CIRCA CIRCA_RETURNS
SeqMap seqmap_realloc_(size_t sizk, size_t sizv, SeqMap sm, size_t cap) {
  circa_guard (!sizk || !sizv || !sm || !cap)
    return (circa_throw(CE_ARG), sm);

  // Calculate the sizes of the input map.
  register const size_t sm_cap = seqmap(sm)->cap;
  register const size_t sm_probe_len = sm_cap * sizeof(*seqmap(sm)->probe);
  register const size_t sm_key_len   = sm_cap * sizeof(Seq);
  register const size_t sm_data_len  = sm_cap * sizv;

  // Allocate a temporary memory pool to store the map.
  char *pool = CIRCA_MALLOC(sm_probe_len + sm_key_len + sm_data_len);
  if (!pool)
    return (circa_throw(CE_OOM), sm);

  // Set pointers to parts of the pool for temporary storage.
  size_t *probe = (size_t*) pool;
  Seq    *key   = ((Seq*) (((char*) probe) + sm_probe_len));
  char   *data  = ((char*) key) + sm_key_len;

  // Copy the map's members into the pool.
  memcpy(probe, seqmap(sm)->probe, sm_probe_len);
  memcpy(key,   sm,                  sm_key_len);
  memcpy(data,  seqmap(sm)->data,   sm_data_len);

  // Calculate the sizes of the replacement map.
  register const size_t sm2_cap = usz_primegt(cap);
  register const size_t sm2_probe_len = sm2_cap * sizeof(*seqmap(sm)->probe);
  register const size_t sm2_key_len   = sm2_cap * sizeof(Seq);
  register const size_t sm2_data_len  = sm2_cap * sizv;

  // Reallocate the map to the new sizes.
  SeqMapData *smd = CIRCA_REALLOC(seqmap(sm), sizeof(*smd) + sm2_key_len);

  if (!smd) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), sm);
  }

  smd->probe = CIRCA_REALLOC(smd->probe, sm2_probe_len);

  if (!smd->probe) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), sm);
  }

  smd->data = CIRCA_REALLOC(smd->data, sm2_data_len);

  if (!smd->data) {
    CIRCA_FREE(pool);
    return (circa_throw(CE_OOM), sm);
  }

  sm = smd->key;
  smd->cap = sm2_cap;

  // Clear out the map's memory.
  memset(smd->probe, 0, sm2_probe_len);
  memset(sm, 0, sm2_key_len);
  memset(smd->data, 0, sm2_data_len);

  // Re-insert the members into the map.
  for (size_t i = 0; i < sm_cap; i++) {
    if (key[i] != NULL) {
      sm = seqmap_set_(sizk, sizv, sm, key[i], data + (i * sizv));
      key[i] = seq_free_(sizk, key[i]);
    }
  }


  CIRCA_FREE(pool);

  return sm;
}

CIRCA CIRCA_RETURNS
SeqMap seqmap_require_(size_t sizk, size_t sizv, SeqMap sm, size_t cap) {
  circa_guard (!sizk || !sizv || !sm || !cap)
    return (circa_throw(CE_ARG), sm);

  return (cap > seqmap(sm)->cap) ? seqmap_realloc_(sizk, sizv, sm, cap) : sm;
}

CIRCA CIRCA_RETURNS
SeqMap seqmap_free_(size_t sizk, size_t sizv, SeqMap sm) {
  circa_guard (!sizk || !sizv)
    return (circa_throw(CE_ARG), sm);

  if (sm) {
    // Free keys and zero out freed memory for security.
    for (size_t i = 0; i < seqmap(sm)->cap; i++)
      seqmap(sm)->key[i] = seq_free_(sizk, seqmap(sm)->key[i]);
    memset(seqmap(sm)->data, 0, seqmap(sm)->cap * sizv);

    // Free the members of the map, then the map itself.
    CIRCA_FREE(seqmap(sm)->probe);
    CIRCA_FREE(seqmap(sm)->data);
    CIRCA_FREE(seqmap(sm));
  }

  return NULL;
}
