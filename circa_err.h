#ifndef CIRCA_ERR_H
#define CIRCA_ERR_H

#include "circa_mac.h"

typedef enum {
  CE_NONE,
  CE_NULL,
  CE_ZERO,
  CE_ONE,
  CE_OOM,
  CE_OOB,
  CE_READ,
  CE_WRITE,
  CE_FMT,
  CE_LENGTH
} circa_err;

#ifndef CIRCA_N_LOG
  #define circa_log(FMT, ...) fprintf(stderr, "[circa]: " FMT "\n", ##__VA_ARGS__)
#else
  #define circa_log(FMT, ...) ((void) 0)
#endif

#define circa_throw(E, FMT, ...) return (circa_log("%s: " FMT, __func__, ##__VA_ARGS__), E)

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

#define circa_nullck(X) circa_if_nullck(!(X)) circa_throw(CE_NULL, "A null argument was passed.")

#ifndef CIRCA_N_ZERO
  #define circa_if_zerock(X) if_debug(X)
#else
  #define circa_if_zerock(X) if (0)
#endif

#define circa_zerock(X) circa_if_zerock(!(X)) circa_throw(CE_ZERO, "A zero argument was passed.")

#ifndef CIRCA_N_ONE
  #define circa_if_oneck(X) if_debug(X)
#else
  #define circa_if_oneck(X) if (0)
#endif

#define circa_oneck(X) circa_if_oneck((X) < 2) circa_throw(CE_ONE, "A one or less argument was passed.")

#ifndef CIRCA_N_OOM
  #define circa_if_oomck(X) if_debug (X)
#else
  #define circa_if_oomck(X) if (0)
#endif

#define circa_oomck(X) circa_if_oomck(!(X)) circa_throw(CE_OOM, "Ran out of memory.")

#ifndef CIRCA_N_OOB
  #define circa_if_oobck(X) if_debug (X)
#else
  #define circa_if_oobck(X) if (0)
#endif

#define circa_oobck(X) circa_if_oobck(!(X)) circa_throw(CE_OOB, "Out of bounds.")

#endif /* CIRCA_ERR_H */
