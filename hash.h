/*
** hash.h | An implementation of the fnv1a hash.
** https://github.com/davidgarland/circa
*/

#ifndef CIRCA_HASH_H
#define CIRCA_HASH_H

/*
** Compatibiltity Checks
*/

#if !defined(__STDC_VERSION__)
  #error "[circa/hash.h]: C99 is required."
#endif

#if __STDC_VERSION__ < 199901L
  #error "[circa/hash.h]: C99 is required."
#endif

/*
** Dependencies
*/

/* Internal */

#include "core.h"
#include "types.h"

/*
** Function Declarations
*/

_circa_ size_t fnv1a_seed(void);
_circa_ u64 fnv1a_seed_u64(void);
_circa_ u32 fnv1a_seed_u32(void);

_circa_ size_t fnv1a(u8 byte, size_t hash);
_circa_ u64 fnv1a_u64(u8 byte, u64 hash);
_circa_ u32 fnv1a_u32(u8 byte, u32 hash);

/*
** Function Implementations
*/

/*
** -- fnv1a_seed --
** Description
**   Yields the optimal seed for an fnv1a hash.
** Arguments
**   void
** Returns
**   Seed (size_t)
*/

_circa_
size_t fnv1a_seed(void)
{
  if (sizeof(size_t) == 8) return fnv1a_seed_u64();
  return fnv1a_seed_u32();
}

/*
** -- fnv1a_seed_u64 --
** Description
**   Yields the optimal seed for a 64 bit fnv1a hash.
** Arguments
**   void
** Returns
**   Seed (u64)
*/

_circa_
u64 fnv1a_seed_u64(void)
{
  return 14695981039346656037U;
}

/*
** -- fnv1a_seed_u32 --
** Description
**   Yields the optimal seed for a 32 biy fnv1a hash.
** Arguments
**   void
** Returns
**   Seed (u32)
*/

_circa_
u32 fnv1a_seed_u32(void)
{
  return 2166136261U;
}


/*
** -- fnv1a --
** Description
**   Computes the fnv1a hash for a byte given a previous hash value.
**   When computing the first byte, pass `fnv1a_seed()`.
** Arguments
**   byte: Byte (u8)
**   hash: Hash (size_t)
** Returns
**   Hash (size_t)
*/

_circa_
size_t fnv1a(u8 byte, size_t hash)
{
  if (sizeof(size_t) == 8) return fnv1a_u64(byte, hash);
  return fnv1a_u32(byte, hash);
}

/*
** -- fnv1a_u64 --
** Description
**   Computes the fnv1a hash for a byte given a previous 64 bit hash value.
**   When computing the first byte, pass `fnv1a_seed_u64()`.
** Arguments
**   byte: Byte (u8)
**   hash: Hash (u64)
** Returns
**   Hash (u64)
*/

_circa_
u64 fnv1a_u64(u8 byte, u64 hash)
{
  const u64 prime = 1099511628211U;
  hash ^= byte;
  hash *= prime;
  return hash;
}

/*
** -- fnv1a_u32 --
** Description
**   Computes the fnv1a hash for a byte given a previous 32 bit hash value.
**   When computing the first byte, pass `fnv1a_seed_u32()`.
** Arguments
**   byte: Byte (u8)
**   hash: Hash (u64)
*/

_circa_
u32 fnv1a_u32(u8 byte, u32 hash)
{
  const u32 prime = 16777619U;
  hash ^= byte;
  hash *= prime;
  return hash;
}

#endif /* CIRCA_HASH_H */
