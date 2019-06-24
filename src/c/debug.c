/*
** debug.c | The Circa Library Set | Debugging utilities.
** https://github.com/davidgarland/circa
*/

#include "../h/debug.h"

CIRCA_STATIC _Thread_local CircaError CE;

CIRCA_STATIC char *const restrict CE_MSG[CE_LENGTH] = {
  [CE_OK] = "CE_OK: Everything is OK.",
  [CE_NOK] = "CE_NOK: Placeholder value.",
  [CE_ARG] = "CE_ARG: Bad argument.",
  [CE_OOM] = "CE_OOM: Out of memory.",
  [CE_OOB] = "CE_OOB: Out of bounds.",
  [CE_FMT] = "CE_FMT: Formatting string failed.",
  [CE_FILE_READ] = "CE_FILE_READ: Error reading from a file.",
  [CE_FILE_WRITE] = "CE_FILE_WRITE: Error writing to a file."
};
