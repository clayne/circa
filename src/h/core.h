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
  CE_OK = 0, // No errors.
  CE_ARG, // Bad argument.
  CE_OOM, // Out of memory.
  CE_OOB, // Out of bounds.
  CE_FILE_READ,  // Issues reading a file.
  CE_FILE_WRITE  // Issues writing to a file.
};

static _Thread_local enum circa_error CE;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomma"

#endif // CIRCA_CORE_H
