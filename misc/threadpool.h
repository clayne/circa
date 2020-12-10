typedef struct {
  size_t idx;
} thread_id;

static _Thread_local struct {
  thread_id id;
} thread_info;

typedef struct {
  thread_id id;
  int (*fn)(void*);
  void *arg;
} thread_arg;

static struct {
  size_t num_threads;
  size_t id_counter;
  thrd_t *threads;
  thread_arg *args;
} threadpool;

struct mcas_helper;

typedef struct {
  void *addr;
  uint64_t expected;
  uint64_t new;
  struct mcas_helper *mch;
} cas_row;

typedef struct mcas_helper {
  cas_row *cr;
} mcas_helper;

static void threadpool_alloc(size_t);
static void threadpool_free(void);

static int threadpool_spawn(thread_id *, thrd_start_t, void *);
static void threadpool_join(thread_id, int *);
static void threadpool_detach(thread_id);

static
void threadpool_alloc(size_t num_threads) {
  threadpool.num_threads = num_threads;
  threadpool.id_counter = 0;
  threadpool.threads = malloc(num_threads * sizeof(*threadpool.threads));
  threadpool.args = malloc(num_threads * sizeof(*threadpool.args));
}

static
void threadpool_free(void) {
  free(threadpool.threads);
  free(threadpool.args);
}

int thread_info_init(thread_arg *arg) {
  thread_info.id = arg->id;
  return arg->fn(arg->arg);
}

static
int threadpool_spawn(thread_id *id, thrd_start_t fn, void *arg) {
  id->idx = threadpool.id_counter++;
  threadpool.args[id->idx] = (thread_arg) {*id, fn, arg};
  return thrd_create(threadpool.threads + id->idx, (int(*)(void *)) thread_info_init, threadpool.args + id->idx);
}

static
void threadpool_join(thread_id id, int *res) {
  thrd_join(threadpool.threads[id.idx], res);
}

static
void threadpool_detach(thread_id id) {
  thrd_detach(threadpool.threads[id.idx]);
}
