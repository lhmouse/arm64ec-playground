/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#include "xprecompiled.h"
#define __MCF_ONCE_IMPORT  __MCF_DLLEXPORT
#define __MCF_ONCE_INLINE  __MCF_DLLEXPORT
#include "once.h"
#include "xglobals.h"

__MCF_DLLEXPORT
int
_MCF_once_wait_slow(_MCF_once* once, const int64_t* timeout_opt)
  {
    return 1;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void
_MCF_once_abort(_MCF_once* once)
  {
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void
_MCF_once_release(_MCF_once* once)
  {
  }
