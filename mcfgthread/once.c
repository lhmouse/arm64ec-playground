/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#define WIN32_LEAN_AND_MEAN  1
#define NOMINMAX  1
#define NOGDI  1
#define NOMSG  1
#define _WIN32_WINNT  0x0601
#include <windows.h>
#define __MCF_ONCE_IMPORT  __declspec(dllexport)
#define __MCF_ONCE_INLINE  __declspec(dllexport)
#include "once.h"
#include "xglobals.h"

__declspec(dllexport)
int
_MCF_once_wait_slow(_MCF_once* once, const int64_t* timeout_opt)
  {
    return 1;
  }

__declspec(dllexport) __MCF_NEVER_INLINE
void
_MCF_once_abort(_MCF_once* once)
  {
  }

__declspec(dllexport) __MCF_NEVER_INLINE
void
_MCF_once_release(_MCF_once* once)
  {
  }
