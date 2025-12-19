/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#ifndef __MCFGTHREAD_XGLOBALS_
#define __MCFGTHREAD_XGLOBALS_

#include "fwd.h"
#include <minwindef.h>
#include <winnt.h>
#include <winerror.h>
#include <sysinfoapi.h>
#include <heapapi.h>

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#  error Windows platforms are assumed to be little-endian.
#endif

#ifdef __cplusplus
#  error This internal header is for C only.
#endif

#ifndef __MCF_XGLOBALS_IMPORT
#  define __MCF_XGLOBALS_IMPORT
#  define __MCF_XGLOBALS_INLINE  __MCF_GNU_INLINE
#  define __MCF_XGLOBALS_READONLY   const
#endif

/* Otherwise, SEH is table-based. This code must work on both x86_64 and ARM64,
 * as well as ARM64EC.  */
#  define __MCF_SEH_DEFINE_TERMINATE_FILTER  \
    __asm__ (".seh_handler __MCF_seh_top, @except, @unwind")  /* no semicolon  */

__MCF_ALWAYS_INLINE
void
__MCF_invoke_cxa_dtor(__MCF_cxa_dtor_any_ dtor, void* arg)
  {
    (* dtor) (arg);
  }

#endif  /* __MCFGTHREAD_XGLOBALS_  */
