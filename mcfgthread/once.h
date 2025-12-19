/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#ifndef __MCFGTHREAD_ONCE_
#define __MCFGTHREAD_ONCE_

#include "fwd.h"

__MCF_CXX(extern "C" {)
#ifndef __MCF_ONCE_IMPORT
#  define __MCF_ONCE_IMPORT
#  define __MCF_ONCE_INLINE  __MCF_GNU_INLINE
#endif

/* Initializes a once-initialization flag dynamically. Static ones should be
 * initialized with `{0}`, like other structs.
 *
 * Each once-initialization flag can be in any of the three states: UNLOCKED,
 * LOCKED and READY. An once-initialization flag is initialized to the UNLOCKED
 * state.  */
__MCF_ONCE_INLINE
void
_MCF_once_init(int* __once)
  __MCF_noexcept;

/* Attempts to lock a once-initialization flag.
 *
 * The return value of this function has the same semantics with the
 * `__cxa_guard_acquire()` function from the Itanium C++ ABI, as explained on
 * <https://itanium-cxx-abi.github.io/cxx-abi/abi.html#once-ctor>.
 *
 * If the `__timeout_opt` argument points to a positive integer, it denotes the
 * expiration time in number of milliseconds since 1970-01-01T00:00:00Z. If it
 * points to a negative integer, the absolute value of it denotes the number of
 * milliseconds to wait. If it points to zero, the function returns immediately
 * without waiting. If it is null, the function waits indefinitely.
 *
 * If this once-initialization flag is in the UNLOCKED state, this function
 * changes it into the LOCKED state and returns 1. If it is in the LOCKED state
 * because another thread has locked it, this function blocks until the other
 * thread releases the lock, or returns -1 if the operation has timed out. If
 * the once flag is already in the READY state, this function does nothing and
 * returns 0 immediately.  */
__MCF_ONCE_IMPORT
int
_MCF_once_wait_slow(int* __once, const int64_t* __timeout_opt)
  __MCF_noexcept;

__MCF_ONCE_INLINE
int
_MCF_once_wait(int* __once, const int64_t* __timeout_opt)
  __MCF_noexcept;

/* Cancels a once-initialization flag which shall be in the LOCKED state. If
 * the flag has not been locked already, the behavior is undefined.
 *
 * This function changes it into the UNLOCKED state and wakes up the next
 * thread that is sleeping on this flag, whose call to `_MCF_once_wait()` will
 * return 1.  */
__MCF_ONCE_IMPORT
void
_MCF_once_abort(int* __once)
  __MCF_noexcept;

/* Releases a once-initialization flag which shall be in the LOCKED state. If
 * the flag has not been locked already, the behavior is undefined.
 *
 * This function changes it into the READY state and wakes up all threads that
 * are sleeping on this flag, whose calls to `_MCF_once_wait()` will return 0.  */
__MCF_ONCE_IMPORT
void
_MCF_once_release(int* __once)
  __MCF_noexcept;

/* Define inline functions after all declarations.
 * We would like to keep them away from declarations for conciseness, which also
 * matches the disposition of non-inline functions. Note that however, unlike C++
 * inline functions, they have to have consistent inline specifiers throughout
 * this file.  */
__MCF_ONCE_INLINE
void
_MCF_once_init(int* __once)
  __MCF_noexcept
  {
  }

__MCF_ONCE_INLINE
int
_MCF_once_wait(int* __once, const int64_t* __timeout_opt)
  __MCF_noexcept
  {
    return 1;
  }

__MCF_CXX(})  /* extern "C"  */
#endif  /* __MCFGTHREAD_ONCE_  */
