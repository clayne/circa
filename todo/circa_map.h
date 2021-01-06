typedef struct {
  K *keys;
  V *vals;
  size_t *psls;
  size_t cap;
  size_t len;
} C3(map, K, V);

circa_static circa_err C4(map, K, V, alloc)(C3(map, K, V) *const restrict);
circa_static circa_err C4(map, K, V, free)(C3(map, K, V) *);

#ifdef CIRCA_STATIC
  #include "map.c"
#endif
