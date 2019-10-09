/*
** buck.h | The Circa Library Set | Dynamic buckets.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_BUCK_H
#define CIRCA_BUCK_H

/*
** Dependencies
*/

/* Standard */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Circa */

#include "core.h"
#include "debug.h"
#include "slice.h"

/*
** Type Definitions
*/

typedef struct buck_data {
  struct buck_data *next;
  char data[];
} BuckData;

typedef struct {
  size_t cap;
  size_t len;
  size_t width;
  union {
    char small[CIRCA_BUCK_SMALL];
    struct {
      struct buck_data *next;
      struct buck_data *last;
    };
  };
} Buck;

/*
** Accessors
*/

#define buck_small(T, B) buck_small_(sizeof(T), (B))
CIRCA bool buck_small_(size_t siz, Buck *b);

#define buck_set(T, B, A, V) buck_set_(sizeof(T), (B), (A), (V))
CIRCA void buck_set_(size_t siz, Buck *b, size_t a, void *v);

#define buck_get(T, B, A) (*((T*) buck_get_(sizeof(T), (B), (A))))
CIRCA void *buck_get_(size_t siz, Buck *b, size_t a);

/*
** Allocators
*/

#define buck_alloc(T, C, W) buck_alloc_(sizeof(T), (C), (W))
CIRCA Buck buck_alloc_(size_t siz, size_t cap, size_t width);

#define buck_require(T, B, C) buck_require_(sizeof(T), (B), (C))
CIRCA void buck_require_(size_t siz, Buck *b, size_t cap);

#define buck_shrink(T, B) buck_shrink_(sizeof(T), (B))
CIRCA void buck_shrink_(size_t siz, Buck *b);

#define buck_free(T, B) buck_free_(sizeof(T), (B))
CIRCA void buck_free_(size_t siz, Buck *b);

/*
** Stack Operations
*/

#define buck_push(T, B, V) buck_push_(sizeof(T), (B), (V))
CIRCA void buck_push_(size_t siz, Buck *b, void *v);

#define buck_pop(T, B) buck_pop_(sizeof(T), (B))
CIRCA void *buck_pop_(size_t siz, Buck *b);

/*
** Iterator Macros
*/

#define buck_foreach(T, B, V, E)                   \
do {                                               \
  T V;                                             \
  if (buck_small(T, B)) {                          \
    for (size_t I = 0; I < (B)->len; I++) {          \
      V = (*((T*) ((B)->small + I * sizeof(T))));       \
      E                                            \
    }                                              \
  } else {                                         \
    BuckData *bd = (B)->next;                         \
    for (size_t I = 0; I < (B)->len / (B)->width; I++) { \
      for (size_t J = 0; J < (B)->width; J++) {        \
        V = (*((T*) (bd->data + J * sizeof(T))));            \
        E                                            \
      }                                            \
      if (!bd->next) \
        break; \
      bd = bd->next;                               \
    }                                              \
  }                                                \
} while (0)

#endif // CIRCA_BUCK_H
