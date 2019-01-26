/*
** core.h | The Circa Library Set | Core Library
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_CORE_H
#define CIRCA_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum circa_error {
  CE_OK  = 0,    // Everything is OK!
  CE_NOK = 1,    // A placeholder "Not OK" value for circa_retry.
  CE_ARG,        // Bad argument.
  CE_OOM,        // Out of memory.
  CE_OOB,        // Out of bounds.
  CE_FILE_READ,  // Issues reading a file.
  CE_FILE_WRITE  // Issues writing to a file.
};

extern _Thread_local enum circa_error CE;

#ifdef __TINYC__
  #define _Thread_local
#endif

static const char *const restrict CE_MSG[] = {
  [CE_OK]         = "CE_OK: Everything is O-K.",
  [CE_NOK]        = "CE_NOK: Placeholder value for circa_retry.",
  [CE_ARG]        = "CE_ARG: Bad argument.",
  [CE_OOM]        = "CE_OOM: Out of memory.",
  [CE_OOB]        = "CE_OOB: Out of array bounds.",
  [CE_FILE_READ]  = "CE_FILE_READ: Error reading from a file",
  [CE_FILE_WRITE] = "CE_FILE_WRITE: Error writing to a file."
};

#define ce_retry \
for (CE = CE_NOK; CE; CE = CE_OK)

#define ce_debug \
for (int I = 0; I < 1; I++, CE && (CE != CE_NOK) && printf("[%s:%s:%i] circa_debug: %s\n", __FILE__, __func__, __LINE__, CE_MSG[CE]))

#define ce_critical ce_retry ce_debug

#ifndef NDEBUG
  #define ce_guard(...) if (__VA_ARGS__)
#else
  #define ce_guard(...) if (0)
#endif

// TODO: Do this separately from the core header. It's not nice to do this.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomma"

#endif // CIRCA_CORE_H
