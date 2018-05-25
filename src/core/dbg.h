   ///////////////////////////////////////////////////////////////////
  // dbg.h | The Circa Core Library for C | Debugging ///////////////
 // Author: David Garland (https://github.com/davidgarland/circa) //
///////////////////////////////////////////////////////////////////

#ifndef CIRCA_DBG_H
#define CIRCA_DBG_H

#include "cfg.h"

  //                //
 // Debug Messages //
//                //

#if CIRCA_DBG
  #define circa_printf(FMT_, ...) printf((FMT), __VA_ARGS__)
  #define circa_assert(...) if (!(__VA_ARGS__)) \
  {  \
    printf("\n[circa] In file `%s`, on line `%i`:\n", (__FILE__), (__LINE__)); \
    printf("  Assertion `%s` failed.\n", #__VA_ARGS__); \
    exit(1); \
  }
#else
  #define circa_printf(FMT_, ...)
  #define circa_assert(...)
#endif

#endif // CIRCA_DBG_H
