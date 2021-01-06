#ifndef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

circa_static
circa_err C3(hatc, T, alloc)(C2(hatc, T) *h) {
  circa_nullck(h);
  h->l = NULL;
  h->m = malloc(sizeof(*h->m));
  h->h = malloc(sizeof(*h->h) << 1);
  h->m_cap = 1;
  h->l_len = 0;
  h->m_len = 0;
  h->h_len = 0;
  h->len = 0;
}

circa_static
circa_err C3(hatc, T, free)(C2(hatc, T) *h) {
  circa_nullck(h);
  for (size_t i = 0; i < h->m_len; i++)
    free(h->m[i]);
  free(h->l);
  free(h->m);
  free(h->h);
}

#undef circa_static
