#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
  #include "circa_map.h"
#endif

circa_static
circa_err C4(map, K, V, alloc)(C3(map, K, V) *const restrict m) {
  m->keys = malloc(2 * sizeof(K));
  circa_oomck(m->keys);
  m->vals = malloc(2 * sizeof(V));
  circa_if_oomck(!m->vals) {
    free(m->keys);
    circa_throw(CE_OOM);
  }
  m->psls = calloc(2, sizeof(size_t));
  circa_if_oomck(!m->psls) {
    free(m->keys);
    free(m->vals);
    circa_throw(CE_OOM);
  }
  m->cap = 2;
  m->len = 0;
  return CE_NONE;
}

circa_static
circa_err C4(map, K, V, free)(C3(map, K, V) *m) {
  circa_nullck(m);
  free(m->keys);
  free(m->vals);
  free(m->psls);
}
