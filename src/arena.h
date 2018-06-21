   //                                       ////////////////////////////////////
  // arena.h | Circa's arena datatype.     ////////////////////////////////////
 // https://github.com/davidgarland/circa ////////////////////////////////////
//                                       ////////////////////////////////////

#ifndef CIRCA_ARENA_H
#define CIRCA_ARENA_H

#include "../circa.h"

  //           /////////////////////////////////////////////////////////////////
 // Accessors /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

_circa_
struct ArenaData *arena(Arena a)
{
  {
    circa_assert(a != NULL);
  }
  return ((struct ArenaData*) a) - 1;
}

  //            ////////////////////////////////////////////////////////////////
 // Allocators ////////////////////////////////////////////////////////////////
//            ////////////////////////////////////////////////////////////////

/// arena_new ///
// Description
//   Returns a new arena of a given capacity.
// Arguments
//   T: Type (type)
//   cap: Capacity (size_t)
// Returns
//   Arena (Arena)

_circa_ _circa_alcs_
Arena arena_new_(size_t siz, size_t cap)
{
  {
    circa_assert(siz > 0);
    circa_assert(cap > 0);
  }
  struct ArenaData *a = NULL;
  while (a == NULL) a = malloc(sizeof(*a) + (cap * siz));
  a->cap = cap;
  void *in_use = NULL;
  while (in_use == NULL) in_use = malloc(cap * sizeof(bool));
  memset(in_use, 0, cap * sizeof(bool));
  a->in_use = in_use;
  return a->data;
}

/// arena_rsz ///
// Description
//   Resizes an arena to a given capacity.
// Arguments
//   T: Type (type)
//   a: Arena (Arena)
//   cap: Capacity (size_t)
// Returns
//   Arena (Arena)

_circa_ _circa_rets_
Arena arena_rsz_(size_t siz, Arena a, size_t cap)
{
  {
    circa_assert(siz > 0);
    circa_assert(a != NULL);
    circa_assert(cap > 0);
  }
  struct ArenaData *a2 = NULL;
  while (a2 == NULL) a2 = realloc(arena(a), sizeof(*a2) + (cap * siz));
  a2->cap = cap;
  void *in_use = NULL;
  while (in_use == NULL) in_use = realloc(a2->in_use, (cap * sizeof(bool)));
  memset(in_use, 0, cap * sizeof(bool));
  a2->in_use = in_use;
  return a2->data;
}

/// arena_del ///
// Description
//   Deletes an arena.
// Arguments
//   T: Type (type)
//   a: Arena (Arena)
// Returns
//   Null Arena (Arena)

_circa_ _circa_rets_
Arena arena_del_(size_t siz, Arena a)
{
  {
    circa_assert(siz > 0);
  }
  if (a != NULL) {
    #ifdef CIRCA_SECURE
      memset(arena(a), 0, siz * arena(a)->cap);
    #endif
    free(arena(a));
  }
  return NULL;
}

  //           /////////////////////////////////////////////////////////////////
 // Array Ops /////////////////////////////////////////////////////////////////
//           /////////////////////////////////////////////////////////////////

/// arena_take ///
// Description
//   Take the first available block of memory from an arena.
// Arguments
//   T: Type (type)
//   a: Arena (Arena)
// Returns
//   Pointer (void*)

_circa_ _circa_rets_
void *arena_take_(size_t siz, Arena *a)
{
  {
    circa_assert(siz > 0);
    circa_assert(a != NULL);
  }
  const size_t cap = arena(*a)->cap;
  bool *const in_use = arena(*a)->in_use;
  for (size_t i = 0; i < cap; i++) {
    if (!in_use[i]) {
      in_use[i] = true;
      return (((char*) (*a)) + (siz * i));
    }
  }
  *a = arena_rsz_(siz, *a, cap + 1 + CIRCA_ALLOC);
  in_use[cap + 1] = true;
  return (((char*) (*a)) + (siz * (cap + 1)));
}

/// arena_give ///
// Description
//   Return a block of memory to an arena.
// Arguments
//   T: Type (type)
//   a: Arena (Arena)
//   ptr: Pointer (void*)
// Returns
//   Arena (Arena)

_circa_ _circa_rets_
void *arena_give_(size_t siz, Arena a, void *ptr)
{
  {
    circa_assert(siz > 0);
    circa_assert(a != NULL);
    circa_assert(ptr != NULL);
  }
  const ptrdiff_t diff = ((char*) ptr) - ((char*) a);
  const size_t addr = diff / siz;
  #ifdef CIRCA_SECURE
    memset((char*) a + diff, 0, siz);
  #endif
  arena(a)->in_use[addr] = false;
  return a;
}

#endif // CIRCA_ARENA_H
