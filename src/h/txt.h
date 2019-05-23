/*
** txt.h | The Circa Library Set | Dynamic text.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_TXT_H
#define CIRCA_TXT_H

/*
** Dependencies
*/

/* Circa */

#include "seq.h"

/*
** Type Definitions
*/

typedef Seq(char) Txt;

/*
** Function Declarations
*/

/* Accessors */

static inline SeqData *txt(Txt t);

/* Allocators */

#define txt_alloc(C) txt_alloc_((C))
CIRCA CIRCA_ALLOCS Txt txt_alloc_(size_t cap);

#define txt_realloc(S, C) (S) = txt_realloc_(S, C)
CIRCA CIRCA_RETURNS Txt txt_realloc_(Txt t, size_t cap);

#define txt_require(S, C) (S) = txt_require_(S, C)
CIRCA CIRCA_RETURNS Txt txt_require_(Txt t, size_t cap);

#define txt_free(S) (S) = txt_free_((S))
CIRCA CIRCA_RETURNS Txt txt_free_(Txt t);

/* Sequence Operations */

#define txt_push(S, C) (S) = txt_push_((S), (C))
CIRCA CIRCA_RETURNS Txt txt_push_(Txt t, char c);

/* Stack Operations */

/*
** Function Definitions
*/

static inline
SeqData *txt(Txt t) {
  return seq(t);
}

#endif // CIRCA_TXT_H
