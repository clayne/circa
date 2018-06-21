   //                                            ///////////////////////////////
  // ctx.h | Circa's contexts for multitasking. ///////////////////////////////
 // https://github.com/davidgarland/circa      ///////////////////////////////
//                                            ///////////////////////////////

#ifndef CIRCA_CTX_H
#define CIRCA_CTX_H

#include "../circa.h"

  //            ////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//            ////////////////////////////////////////////////////////////////

/// ctx_new_ ///
// Description
//   Creates a new context for a given set of inputs and outputs for a task.
// Arguments
//   sarg: Argument Type Size (size_t)
//   sret: Return Type Size (size_t)
//   args: Argument Sequence (Seq)
//   rets: Return Sequence (Seq)
// Returns
//   Context (Ctx)

_circa_ _circa_alcs_
Ctx ctx_new_(size_t sarg, size_t sret, Seq args, Seq rets)
{
  {
    circa_assert(sarg > 0);
    circa_assert(sret > 0);
    circa_assert(args != NULL);
    circa_assert(rets != NULL);
  }
  Ctx c = NULL;
  while (c == NULL) c = malloc(sizeof(*c));
  c->args = args;
  c->rets = rets;
  return c;
}

/// ctx_del_ ///
// Description
//   Deletes a context.
// Arguments
//   c: Context (Ctx)
// Returns
//   Null Context (Ctx)

_circa_
Ctx ctx_del_(Ctx c)
{
  if (c != NULL) {
    #ifdef CIRCA_SECURE
      memset(c, 0, sizeof(*c));
    #endif
    free(c);
  }
  return NULL;
}

  //              //////////////////////////////////////////////////////////////
 // Tasking Ops  //////////////////////////////////////////////////////////////
//              //////////////////////////////////////////////////////////////

#endif // CIRCA_CTX_H
