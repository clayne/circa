/*
** bits.c | The Circa Library Set | Integer operations.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Circa */

#include "../h/bits.h"

/*
** Global Definitions
*/

CIRCA_STATIC uint32_t circa_primes[] = {
  2,       2,       5,       11,     // 1,       2,       4,       8
  17,      37,      67,      131,    // 16,      32,      64,      128,
  257,     521,     1031,    2053,   // 256,     512,     1024,    2048
  4099,    8209,    16411,   32771,  // 4096,    8192,    16384,   32768,
  65537,   131101,  262147,  524309, // 65536,   131072,  262144,  524288
  1048583, 2097169, 4194319, 8388617 // 1048576, 2097152, 4194304, 8388608
};

