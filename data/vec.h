typedef struct {
  T *data;
  size_t len;
  size_t cap;
} C2(vec, T);

static void C3(vec, T, alloc)(C2(vec, T) *);
static void C3(vec, T, prealloc)(C2(vec, T) *, size_t);
static void C3(vec, T, amort)(C2(vec, T) *);
static inline void C3(vec, T, free)(C2(vec, T) *);

static inline T *C3(vec, T, lookup)(C2(vec, T) *, size_t);

static inline void C3(vec, T, set_v)(C2(vec, T) *, size_t, T);
static inline void C3(vec, T, set_r)(C2(vec, T) *, size_t , T *);
static inline T C3(vec, T, get_v)(C2(vec, T) *, size_t);
static inline void C3(vec, T, get_r)(C2(vec, T) *, size_t, T *);

static inline void C3(vec, T, snoc_v)(C2(vec, T) *, T);
static inline void C3(vec, T, snoc_r)(C2(vec, T) *, T *);

static
void C3(vec, T, alloc)(C2(vec, T) *v) {
  assert(v);
  v->data = malloc(2 * sizeof(T));
  v->len = 0;
  v->cap = 2;
}

static
void C3(vec, T, prealloc)(C2(vec, T) *v, size_t cap) {
  assert(v);
  assert(cap > 1);
  v->data = malloc(cap * sizeof(T));
  v->len = 0;
  v->cap = cap;
}

static
void C3(vec, T, amort)(C2(vec, T) *v) {
  assert(v);
  if (unlikely(v->len == v->cap)) {
    v->cap += (v->cap >> 1);
    v->data = realloc(v->data, v->cap * sizeof(T));
  }
}

static inline
void C3(vec, T, free)(C2(vec, T) *v) {
  assert(v);
  free(v->data);
}

static inline
T *C3(vec, T, lookup)(C2(vec, T) *v, size_t idx) {
  assert(v);
  assert(idx < v->len);
  return v->data + idx;
}

static inline
void C3(vec, T, set_v)(C2(vec, T) *v, size_t i, T x) {
  assert(v);
  assert(i < v->len);
  v->data[i] = x;
}

static inline
void C3(vec, T, set_r)(C2(vec, T) *v, size_t i, T *x) {
  assert(v);
  assert(i < v->len);
  assert(x);
  v->data[i] = *x;
}

static inline
T C3(vec, T, get_v)(C2(vec, T) *v, size_t i) {
  assert(v);
  assert(i < v->len);
  return v->data[i];
}

static inline
void C3(vec, T, get_r)(C2(vec, T) *v, size_t i, T *x) {
  assert(v);
  assert(i < v->len);
  assert(x);
  *x = v->data[i];
}

static inline
void C3(vec, T, snoc_v)(C2(vec, T) *v, T x) {
  assert(v);
  C3(vec, T, amort)(v);
  v->data[v->len++] = x;
}

static inline
void C3(vec, T, snoc_r)(C2(vec, T) *v, T *x) {
  assert(v);
  assert(x);
  C3(vec, T, amort)(v);
  v->data[v->len++] = *x;
}
