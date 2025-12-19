/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#ifndef __MCFGTHREAD_GTHR_AUX_
#define __MCFGTHREAD_GTHR_AUX_

#include "fwd.h"
#include "once.h"
#include "cond.h"
#include "mutex.h"
#include "shared_mutex.h"
#include <time.h>  /* struct timespec  */

__MCF_CXX(extern "C" {)
#ifndef __MCF_GTHR_AUX_IMPORT
#  define __MCF_GTHR_AUX_IMPORT
#  define __MCF_GTHR_AUX_INLINE  __MCF_GNU_INLINE
#endif

/* Define reusable types.  */
typedef struct __MCF_gthr_rc_mutex __MCF_gthr_rc_mutex;
typedef struct __MCF_gthr_thread_record __MCF_gthr_thread_record;
typedef void* __MCF_gthr_thread_procedure(void* __arg);

struct __MCF_gthr_rc_mutex
  {
    __MCF_BR(uint32_t) __owner;  /* owner thread ID  */
    int __depth;  /* recursion depth  */
    __MCF_BR(_MCF_mutex) __mutex;
  };

struct __MCF_gthr_thread_record
  {
    uint8_t __magic_guid[16];
    __MCF_gthr_thread_procedure* __proc;
    void* __arg_or_result;
  };

/* These functions implement `__gthread_once()`. If `__once_fn` initiates stack
 * unwinding, by throwing an exception for example, the state of `*__once` will
 * be restored correctly.
 * FIXME: At the moment (2024-03-14) GCC does not support SEH on i686.  */
__MCF_GTHR_AUX_INLINE
void
__MCF_gthr_call_once_seh(_MCF_once* __once, __MCF_cxa_dtor_any_ __init_proc, void* __arg)
  __MCF_MAY_THROW;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_call_once_seh_take_over(_MCF_once* __once, __MCF_cxa_dtor_any_ __init_proc, void* __arg)
  __MCF_MAY_THROW;

/* This is an auxiliary function for converting a `__MCF_timespec` to the
 * number of milliseconds since the Unix epoch, with boundary checking.  */
__MCF_GTHR_AUX_IMPORT __MCF_FN_PURE
int64_t
__MCF_gthr_timeout_from_timespec(const __MCF_timespec* __abs_time)
  __MCF_noexcept;

/* These are auxiliary functions to implement recursive mutexes, and are not
 * to be called directly.  */
__MCF_GTHR_AUX_INLINE
void
__MCF_gthr_rc_mutex_init(__MCF_gthr_rc_mutex* __rmtx)
  __MCF_noexcept;

__MCF_GTHR_AUX_INLINE
int
__MCF_gthr_rc_mutex_recurse(__MCF_gthr_rc_mutex* __rmtx)
  __MCF_noexcept;

__MCF_GTHR_AUX_INLINE
int
__MCF_gthr_rc_mutex_wait(__MCF_gthr_rc_mutex* __rmtx, const int64_t* __timeout_opt)
  __MCF_noexcept;

__MCF_GTHR_AUX_INLINE
void
__MCF_gthr_rc_mutex_release(__MCF_gthr_rc_mutex* __rmtx)
  __MCF_noexcept;

/* These are auxiliary functions for condition variables. The argument is a
 * pointer to a plain `_MCF_mutex`.  */
__MCF_GTHR_AUX_IMPORT
intptr_t
__MCF_gthr_mutex_unlock_callback(intptr_t __arg)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_mutex_relock_callback(intptr_t __arg, intptr_t __unlocked)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
int
__MCF_gthr_cond_mutex_wait(_MCF_cond* __cnd, _MCF_mutex* __mtx, const int64_t* __timeout_opt)
  __MCF_noexcept;

/* These are auxiliary functions for condition variables. The argument is a
 * pointer to a `_MCF_shared_mutex`.  */
__MCF_GTHR_AUX_IMPORT
intptr_t
__MCF_gthr_shared_mutex_unlock_callback(intptr_t __arg)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_shared_mutex_relock_shared_callback(intptr_t __arg, intptr_t __unlocked)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_shared_mutex_relock_exclusive_callback(intptr_t __arg, intptr_t __unlocked)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
int
__MCF_gthr_cond_shared_mutex_wait_shared(_MCF_cond* __cnd, _MCF_shared_mutex* __smtx,
                                         const int64_t* __timeout_opt)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
int
__MCF_gthr_cond_shared_mutex_wait_exclusive(_MCF_cond* __cnd, _MCF_shared_mutex* __smtx,
                                            const int64_t* __timeout_opt)
  __MCF_noexcept;

/* These are auxiliary functions for condition variables. The argument is a
 * pointer to a `__MCF_gthr_rc_mutex`.  */
__MCF_GTHR_AUX_IMPORT
intptr_t
__MCF_gthr_recursive_mutex_unlock_callback(intptr_t __arg)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_recursive_mutex_relock_callback(intptr_t __arg, intptr_t __unlocked)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
int
__MCF_gthr_cond_recursive_mutex_wait(_MCF_cond* __cnd, __MCF_gthr_rc_mutex* __rmtx,
                                     const int64_t* __timeout_opt)
  __MCF_noexcept;

/* These are auxiliary functions for threads.  */
__MCF_GTHR_AUX_IMPORT
_MCF_thread*
__MCF_gthr_thread_create_v3(__MCF_gthr_thread_procedure* __proc, void* __arg)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_thread_join_v3(_MCF_thread* __thrd, void** __resp_opt)
  __MCF_noexcept;

__MCF_GTHR_AUX_IMPORT __MCF_NEVER_RETURN
void
__MCF_gthr_thread_exit_v3(void* __resp)
  __MCF_noexcept;

__MCF_GTHR_AUX_INLINE
void
__MCF_gthr_call_once_seh(_MCF_once* __once, __MCF_cxa_dtor_any_ __init_proc, void* __arg)
  __MCF_MAY_THROW
  {
    int __err = _MCF_once_wait(__once, __MCF_nullptr);
    if(__err == 0)
      return;  /* already initialized  */

    __MCF_ASSERT(__err == 1);
    __MCF_gthr_call_once_seh_take_over(__once, __init_proc, __arg);
  }

__MCF_CXX(})  /* extern "C"  */
#endif  /* __MCFGTHREAD_GTHR_AUX_  */
