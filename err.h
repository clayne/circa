#ifndef BUFSIZ
  #error "circa/err.h requires <stdio.h>. Please include it beforehand."
#endif

#define CIRCA_ERR

typedef enum {
  CE_NONE,
  CE_NULL,
  CE_ZERO,
  CE_ONE,
  CE_OOM,
  CE_OOB,
  CE_LENGTH
} circa_err;

static const char *circa_err_name[CE_LENGTH] = {
  [CE_NONE] = "CE_NONE",
  [CE_NULL] = "CE_NULL",
  [CE_ZERO] = "CE_ONE",
  [CE_ONE] = "CE_ONE",
  [CE_OOM] = "CE_OOM",
  [CE_OOB] = "CE_OOB",
};

static const char *circa_err_explain[CE_LENGTH] = {
  [CE_NONE] = "None",
  [CE_NULL] = "A null parameter was passed where nonnull was required",
  [CE_ZERO] = "A zero parameter was passed where nonzero was required",
  [CE_ONE] = "A parameter of one or less was passed where >1 was required",
  [CE_OOM] = "Out of memory; malloc or realloc returned NULL"
};

#ifndef CIRCA_N_LOG
  #define circa_log(FMT, ...) fprintf(stderr, "[circa]: " FMT "\n", ##__VA_ARGS__)
#else
  #define circa_log(FMT, ...) ((void) 0)
#endif

#define circa_throw(E) return (circa_log("%s threw %s; %s", __func__, circa_err_name[E], circa_err_explain[E]), E)

#ifndef if_debug
  #ifndef CIRCA_N_DEBUG
    #define if_debug(X) if_unlikely(X)
  #else
    #define if_debug(X) if (0)
  #endif
#endif

#ifndef CIRCA_N_NULL
  #define circa_if_nullck(X) if_debug(X)
#else
  #define circa_if_nullck(X) if (0)
#endif

#define circa_nullck(X) circa_if_nullck(!(X)) circa_throw(CE_NULL)

#ifndef CIRCA_N_ZERO
  #define circa_if_zerock(X) if_debug(X)
#else
  #define circa_if_zerock(X) if (0)
#endif

#define circa_zerock(X) circa_if_zerock(!(X)) circa_throw(CE_ZERO)

#ifndef CIRCA_N_ONE
  #define circa_if_oneck(X) if_debug(X)
#else
  #define circa_if_oneck(X) if (0)
#endif

#define circa_oneck(X) circa_if_oneck((X) < 2) circa_throw(CE_ONE)

#ifndef CIRCA_N_OOM
  #define circa_if_oomck(X) if_debug (X)
#else
  #define circa_if_oomck(X) if (0)
#endif

#define circa_oomck(X) circa_if_oomck(!(X)) circa_throw(CE_OOM)

#ifndef CIRCA_N_OOB
  #define circa_if_oobck(X) if_debug (X)
#else
  #define circa_if_oobck(X) if (0)
#endif
