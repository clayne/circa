/*
** seq.h | The Circa Library Set | Dynamic sequences.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_SEQ_H
#define CIRCA_SEQ_H

#include <stdlib.h>

/*
** Type Definitions
*/

typedef struct {
  size_t cap;
  size_t len;
  char data[];
} SeqData;

#define Seq(T) T*
typedef Seq(void) Seq;

/*
** Function Declarations
*/

/* Accessors */

static inline SeqData *seq(Seq s);

/*
** Function Implementations
*/

static inline
SeqData *seq(Seq s) {
  return ((SeqData*) s) - 1;
}

#endif // CIRCA_SEQ_H
