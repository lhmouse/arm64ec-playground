/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#include "xprecompiled.h"
#define __MCF_MUTEX_IMPORT  __MCF_DLLEXPORT
#define __MCF_MUTEX_INLINE  __MCF_DLLEXPORT
#include "mutex.h"
#include "xglobals.h"

__MCF_DLLEXPORT
int
_MCF_mutex_lock_slow(_MCF_mutex* mutex, const int64_t* timeout_opt)
  {
   return 0;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void
_MCF_mutex_unlock_slow(_MCF_mutex* mutex)
  {

  }
