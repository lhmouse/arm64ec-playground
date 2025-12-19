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

__MCF_CXX(extern "C" {)
#ifndef __MCF_GTHR_AUX_IMPORT
#  define __MCF_GTHR_AUX_IMPORT
#  define __MCF_GTHR_AUX_INLINE  __MCF_GNU_INLINE
#endif

__MCF_GTHR_AUX_INLINE
void
__MCF_gthr_call_once_seh(_MCF_once* __once, __MCF_cxa_dtor_any_ __init_proc, void* __arg)
  __MCF_MAY_THROW;

__MCF_GTHR_AUX_IMPORT
void
__MCF_gthr_call_once_seh_take_over(_MCF_once* __once, __MCF_cxa_dtor_any_ __init_proc, void* __arg)
  __MCF_MAY_THROW;

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
