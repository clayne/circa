/*
** buck.c | The Circa Library Set | Dynamic buckets.
** https://github.com/davidgarland/circa
*/

/*
** Dependencies
*/

/* Standard */

#include <string.h>

/* Circa */

#include "../h/core.h"
#include "../h/debug.h"
#include "../h/bits.h"
#include "../h/buck.h"

/*
** Accessors
*/

CIRCA
bool buck_small_(size_t siz, Buck *b) {
  circa_guard (!siz || !b)
    return (circa_throw(CE_ARG), false);
  
  return (siz * b->len) <= CIRCA_BUCK_SMALL;
}

CIRCA
void buck_set_(size_t siz, Buck *b, size_t a, void *v) {
  circa_guard (!siz || !b || !v) {
    circa_throw(CE_ARG);
    return;
  }

  register const size_t len = usz_max(b->len, a + 1);
  buck_require_(siz, b, len);
  
  if (buck_small_(siz, b)) {
    memcpy(b->small + (a * siz), v, siz);
  } else {
    register const size_t skips = a / b->width;
    BuckData *bd = b->next;
    
    for (size_t i = 0; i < skips; i++)
      bd = bd->next;
    
    memcpy(bd->data + a - skips * b->width, v, siz);
  }

  b->len = len;
  return;
}

CIRCA
void *buck_get_(size_t siz, Buck *b, size_t a) {
  circa_guard (!siz || !b)
    return (circa_throw(CE_ARG), NULL);
  
  circa_guard (a > b->len)
    return (circa_throw(CE_OOB), NULL);
  
  if (buck_small_(siz, b))
    return b->small + a * siz;

  register const size_t skips = a / b->width;
  BuckData *bd = b->next;
    
  for (size_t i = 0; i < skips; i++)
    bd = bd->next ? bd->next : bd; // TODO: Why is this needed??

  return bd->data + (a - skips * b->width) * siz; // ^ without this, this causes an invalid read
}

/*
** Allocators
*/

CIRCA
Buck buck_alloc_(size_t siz, size_t cap, size_t width) {
  circa_guard (!siz || !cap || !width)
    return (circa_throw(CE_ARG), (Buck) {0});

  Buck b = { .width = width };
  
  if (cap * siz > CIRCA_BUCK_SMALL) {
    cap = usz_roundup(cap, width);
    b.cap = cap;
    
    b.next = CIRCA_MALLOC(sizeof(*b.next) + width * siz);
    b.last = b.next;
  
    if (!b.last)
      return (circa_throw(CE_OOM), (Buck) {0});
    
    b.last->next = NULL;
    cap = (cap > width) ? cap - width : 0;
    
    while (cap > width) {
      b.last->next = CIRCA_MALLOC(sizeof(*b.next) + width * siz);
      b.last = b.last->next;

      if (!b.last)
        return (buck_free_(siz, &b), circa_throw(CE_OOM), (Buck) {0});

      b.last->next = NULL;
      cap = (cap > width) ? cap - width : 0;
    }
  } else {
    b.cap = cap;
  }
  
  return b;
}

CIRCA
void buck_require_(size_t siz, Buck *b, size_t cap) {
  circa_guard (!siz || !cap) {
    circa_throw(CE_ARG);
    return;
  }

  register const size_t width = b->width;

  if (buck_small_(siz, b)) {
    b->cap = cap;
    if (!buck_small_(siz, b)) {
      b->cap = usz_roundup(b->cap, width);
      struct buck_data *bd = CIRCA_MALLOC(sizeof(*bd) + width * siz);
      bd->next = NULL;
      memcpy(bd->data, b->small, b->len * siz);
      b->next = bd;
      b->last = bd;
    }
    return;
  }

  cap = usz_roundup(cap, width);

  while (cap - b->cap > width) {
    b->last->next = CIRCA_MALLOC(sizeof(*b->next) + width * siz);
    b->last = b->last->next;
    b->last->next = NULL;
    b->cap += width;
  }
}

CIRCA
void buck_shrink_(size_t siz, Buck *b) {
  circa_guard (!siz || !b) {
    circa_throw(CE_ARG);
    return;
  }
  
  if (buck_small_(siz, b))
    return;

  register const size_t len = usz_roundup(b->len, b->width);
  register const size_t links = b->cap / b->width;
  register const size_t skips = links - len;

  BuckData *bs = b->next;
  BuckData *bl;

  for (size_t i = 1; i < skips; i++)
    bs = bs->next;

  while (bs->next) {
    bl = bs;
    bs = bs->next;
    memset(bl, 0, sizeof(*bl));
    CIRCA_FREE(bl);
  }
}

CIRCA
void buck_free_(size_t siz, Buck *b) {
  circa_guard (!siz) {
    circa_throw(CE_ARG);
    return;
  }

  if (!b)
    return;

  if (!buck_small(siz, b)) {
    BuckData *bd = b->next;
    if (bd) {
      BuckData *nxt;
      do {
        nxt = bd->next;
        CIRCA_FREE(bd);
        bd = nxt;
      } while (bd);
    }
  }

  b->next = NULL;
  b->last = NULL;
  b->cap = 0;
  b->len = 0;
  b->width = 0;
}

/*
** Stack Operations
*/

CIRCA
void buck_push_(size_t siz, Buck *b, void *v) {
  circa_guard (!siz || !b || !v) {
    circa_throw(CE_ARG);
    return;
  }

  buck_require_(siz, b, b->len + 1);

  if (buck_small_(siz, b)) {
    memcpy(b->small + b->len++ * siz, v, siz);
    return;
  }

  memcpy(b->last->data + (b->len++ % b->width) * siz, v, siz);
}
