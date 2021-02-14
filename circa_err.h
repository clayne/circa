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
  #define circa_log(FMT, ...) fprintf(stderr, "[circa] %s: " FMT "\n", __func__, ##__VA_ARGS__)
#else
  #define circa_log(FMT, ...) ((void) 0)
#endif

#define circa_throw(E, FMT, ...) return (circa_log("%s: " FMT, ##__VA_ARGS__), E)

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

/*
** Copyright 2020 David Garland
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/
