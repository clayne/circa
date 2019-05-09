/*
** seqmap.c | The Circa Library Set | Dual-Generic Seq-Keyed Maps
** https://github.com/davidgarland/circa
*/

#define XXH_STATIC_LINKING_ONLY
#include "xxhash.h"

#include "seqmap.h"

SeqMap seqmap_set_(size_t sizk, size_t sizv, SeqMap sm, Seq k, void *v) {
  ce_guard (!sizk || !sizv || !sm || !k || !v)
    return (CE = CE_ARG, NULL);

  // Let's not call this too many times.
  struct seqmap_data *smd = seqmap(sm);

  // Or dereference.
  const size_t sm_cap = smd->cap;

  // Construct two buckets for shuffling values around. Use VLAs if possible.
  size_t swp_probe = 0, tmp_probe;
  #ifdef CIRCA_VLA
    char swp_data[sizv], tmp_data[sizv];
  #else
    char *swp_data, *tmp_data;
    
    swp_data = malloc(sizv);
    tmp_data = malloc(sizv);

    if (!swp_data || !tmp_data) {
      free(swp_data);
      free(tmp_data);
      return (CE = CE_OOM, sm);
    }
  #endif

  Seq swp_key, tmp_key;

  // Copy the key and value into the swap bucket.
  memcpy(swp_data, v, sizv);
  swp_key = seq_wrap_(sizk, seq(k)->len, k);
  if (CE) {
    #ifndef CIRCA_VLA
      free(swp_data);
      free(tmp_data);
    #endif
    seq_free(swp_key);
    return sm;
  }

  // Calculate the starting position using xxHash.
  const size_t hash = (sizeof(size_t) == 8) ? XXH64(k, seq(k)->len * sizk, 0)
                    : XXH32(k, seq(k)->len * sizk, 0);
  const size_t addr = hash % smd->cap;
  
  size_t i;
  bool found = false;
  for (i = addr; i < sm_cap; i++) {
    if (smd->key[i]) {
      if (seq_cmp_(sizk, smd->key[i], swp_key)) {
        found = true;
        break;
      } else if (smd->probe[i] < swp_probe) {
        // c := a
        tmp_probe = smd->probe[i];
        memcpy(tmp_data, smd->data + (i * sizv), sizv);
        tmp_key = smd->key[i];
        // a := b
        smd->probe[i] = swp_probe;
        memcpy(smd->data + (i * sizv), swp_data, sizv);
        smd->key[i] = swp_key;
        // b := c
        swp_probe = tmp_probe;
        memcpy(swp_data, tmp_data, sizv);
        swp_key = tmp_key;
      }
    } else {
      smd->len++;
      found = true;
      break;
    }
    swp_probe++;
  }

  if (found) {
    smd->probe[i] = swp_probe;
    memcpy(smd->data + (i * sizv), swp_data, sizv);
    smd->key[i] = swp_key;
  } else {
    sm = seqmap_realloc_(sizk, sizv, sm, sm_cap + 1);
    if (CE) {
      #ifndef CIRCA_VLA
        free(swp_data);
        free(tmp_data);
      #endif
      seq_free_(swp_key);
      return sm;
    }
    sm = seqmap_set_(sizk, sizv, sm, swp_key, swp_data);
    seq_free_(swp_key);
    return sm;
  }

  #ifndef CIRCA_VLA
    free(swp_data);
    free(tmp_data);
  #endif

  return sm;
}

bool seqmap_has_(size_t sizk, size_t sizv, SeqMap sm, Seq k) {
  void *p = seqmap_get_(sizk, sizv, sm, k);
  if (CE) {
    if (CE == CE_OOB)
      CE = CE_OK;
    return false;
  } else {
    return p != NULL;
  }
}

void *seqmap_get_(size_t sizk, size_t sizv, SeqMap sm, Seq k) {
  ce_guard (!sizk || !sizv || !sm || !k)
    return (CE = CE_ARG, NULL);

  struct seqmap_data *smd = seqmap(sm);

  const size_t sm_cap = smd->cap;

  const size_t hash = (sizeof(size_t) == 8) ? XXH64(k, seq(k)->len * sizk, 0)
                    : XXH32(k, seq(k)->len * sizk, 0);
  const size_t addr = hash % sm_cap;

  for (size_t i = addr; i < sm_cap; i++) {
    if (smd->key[i]) {
      if (seq_cmp_(sizk, smd->key[i], k)) {
        return smd->data + (i * sizv);
      }
    } else {
      return (CE = CE_OOB, NULL);
    }
  }

  return (CE = CE_OOB, NULL);
}

SeqMap seqmap_alloc_(size_t sizk, size_t sizv, size_t cap) {
  ce_guard (!sizk || !sizv)
    return (CE = CE_ARG, NULL);

  cap = usz_primegt(cap);

  struct seqmap_data *smd = calloc(sizeof(*smd) + cap * sizeof(Seq), 1);

  if (!smd)
    return (CE = CE_OOM, NULL);

  smd->probe = calloc(cap, sizeof(size_t));
  smd->data  = calloc(cap, sizv);

  if (!smd->probe || !smd->data) {
    free(smd->probe);
    free(smd->data);
    free(smd);
    return (CE = CE_OOM, NULL);
  }

  smd->cap = cap;

  return smd->key;
}

SeqMap seqmap_realloc_(size_t sizk, size_t sizv, SeqMap sm, size_t cap) {
  ce_guard (!sizk || !sizv || !sm || !cap)
    return (CE = CE_ARG, NULL);

  // Let's not call this a thousand times.
  struct seqmap_data *smd = seqmap(sm);

  // Calculate the sizes of the map.
  const size_t sm_cap  = smd->cap;
  const size_t sm_data = sm_cap * sizv;
  const size_t sm_key  = sm_cap * sizeof(Seq);

  // Construct a temporary "fake" map based on these sizes.
  struct {
    char *data;
    void **key; 
  } tmp = {
    .data = malloc(sm_data),
    .key  = malloc(sm_key)
  };

  if (!tmp.data || !tmp.key) {
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, sm);
  }

  // Copy the data from the map into the temporary "fake" one.
  memcpy(tmp.data, smd->data, sm_data);
  memcpy(tmp.key,  smd->key,  sm_key);

  // Calculate the new sizes for the original map.
  const size_t sm2_cap   = usz_primegt(cap);
  const size_t sm2_probe = sm2_cap * sizeof(size_t);
  const size_t sm2_data  = sm2_cap * sizv;
  const size_t sm2_key   = sm2_cap * sizeof(Seq);

  // Reallocate the original map to the new sizes.
  smd = realloc(smd, sizeof(*smd) + sm2_key);

  if (!smd) {
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, sm);
  }

  smd->probe = realloc(smd->probe, sm2_probe);

  if (!smd->probe) {
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, sm);
  }

  smd->data  = realloc(smd->data,  sm2_data);

  if (!smd->data) {
    free(tmp.data);
    free(tmp.key);
    return (CE = CE_OOM, sm);
  }

  sm = smd->key;
  smd->cap = sm2_cap;

  // Clear out the map's memory.
  memset(smd->probe, 0, sm2_probe);
  memset(smd->data,  0, sm2_data);
  memset(smd->key,   0, sm2_key);

  for (size_t i = 0; i < sm_cap; i++)
    if (tmp.key[i])
      sm = seqmap_set_(sizk, sizv, sm, tmp.key[i], tmp.data + (i * sizv));

  free(tmp.data);
  free(tmp.key);

  return sm;
}

SeqMap seqmap_require_(size_t sizk, size_t sizv, SeqMap sm, size_t cap) {
  ce_guard (!sizk || !sizv || !sm || !cap)
    return (CE = CE_ARG, NULL);
  return cap > seqmap(sm)->cap ? seqmap_realloc_(sizk, sizv, sm, cap) : sm;
}

SeqMap seqmap_free_(SeqMap sm) {
  if (sm) {
    for (size_t i = 0; i < seqmap(sm)->cap; i++)
      seq_free_(seqmap(sm)->key[i]);
    free(seqmap(sm)->probe);
    free(seqmap(sm)->data);
    free(seqmap(sm));
  }
  return NULL;
}
