/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#include "xprecompiled.h"
#define __MCF_XGLOBALS_IMPORT  __MCF_DLLEXPORT
#define __MCF_XGLOBALS_INLINE  __MCF_DLLEXPORT
#define __MCF_XGLOBALS_READONLY
#include "xglobals.h"

__MCF_DLLEXPORT
EXCEPTION_DISPOSITION
__cdecl
__MCF_seh_top(EXCEPTION_RECORD* rec, PVOID estab_frame, CONTEXT* ctx, PVOID disp_ctx)
  {
    (void) estab_frame;
    (void) ctx;
    (void) disp_ctx;

    /* GCC raises `0x20474343` to search for an exception handler, and raises
     * `0x21474343` to unwind the stack. If the control flow resumes after
     * `RaiseException()`, `std::terminate()` is called.  */
    ULONG nonmatch = rec->ExceptionFlags & EXCEPTION_NONCONTINUABLE;
    nonmatch |= (rec->ExceptionCode & 0x20FFFFFF) ^ 0x20474343;
    return nonmatch ? ExceptionContinueSearch : ExceptionContinueExecution;
  }

#if defined __MCF_M_X8632_ASM && !defined __MCF_IN_DLL && defined _MSC_VER
/* In the DLL we build a handler table by hand, but this is still necessary
 * for the static library. Safe SEH is only supported by Microsoft LINK, or
 * LLD in LINK mode.  */
__asm__ (".safeseh ___MCF_seh_top");
#endif

__MCF_DLLEXPORT
void
__MCF_initialize_winnt_timeout_v3(__MCF_winnt_timeout* to, const int64_t* ms_opt)
  {
    /* Initialize it to an infinite value.  */
    to->__li.QuadPart = INT64_MAX;
    to->__since = 0;

    /* If no timeout is given, wait indefinitely.  */
    if(!ms_opt)
      return;

    if(*ms_opt > 0) {
      /* If `*ms_opt` is positive, it denotes the number of milliseconds since
       * 1970-01-01T00:00:00Z, and has to be converted into the number of 100
       * nanoseconds since the 1601-01-01T00:00:00Z.  */
      if(*ms_opt > 910692730085477)
        return;

      to->__li.QuadPart = (11644473600000 + *ms_opt) * 10000;
    }
    else if(*ms_opt < 0) {
      /* If `*ms_opt` is negative, it denotes the number of milliseconds to
       * wait, relatively.  */
      if(*ms_opt < -922337203685477)
        return;

      to->__li.QuadPart = *ms_opt * 10000;
      QueryUnbiasedInterruptTime(&(to->__since));
    }
    else
      to->__li.QuadPart = 0;
  }

__MCF_DLLEXPORT
void
__MCF_adjust_winnt_timeout_v3(__MCF_winnt_timeout* to)
  {
    /* Absolute timeouts need no adjustment.  */
    if(to->__li.QuadPart >= 0)
      return;

    /* Add the number of 100 nanoseconds that have elapsed so far, to the
     * timeout which is negative, using saturation arithmetic.  */
    ULONGLONG old_since = to->__since;
    QueryUnbiasedInterruptTime(&(to->__since));
    to->__li.QuadPart += (LONGLONG) (to->__since - old_since);
    to->__li.QuadPart &= to->__li.QuadPart >> 63;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void
__MCF_batch_release_common(const void* key, size_t count)
  {
    /* A call to `ExitProcess()` terminates all the other threads, even if
     * they are waiting. We don't release the keyed event in this case, as it
     * blocks the calling thread infinitely if there is no thread to wake up.
     * See <https://github.com/lhmouse/mcfgthread/issues/21>.  */
    size_t woken_num = 0;
    while((woken_num != count) && !__MCF_is_process_shutting_down())
      if(__MCF_keyed_event_signal(key, &(__MCF_winnt_timeout) { .__li.QuadPart = -1000 }) == 0)
        woken_num ++;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
int
__MCF_win32_error_i(ULONG code, int val)
  {
    SetLastError(code);
    return val;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void*
__MCF_win32_error_p(ULONG code, void* ptr)
  {
    SetLastError(code);
    return ptr;
  }

__MCF_DLLEXPORT __MCF_NEVER_INLINE
void*
__MCF_win32_ntstatus_p(NTSTATUS status, void* ptr)
  {
    SetLastError(RtlNtStatusToDosError(status));
    return ptr;
  }

static
int
__fastcall
do_pop_static_dtor(__MCF_dtor_element* elem, _MCF_mutex* mtx, __MCF_dtor_queue* queue, void* dso)
  {
    _MCF_mutex_lock(mtx, __MCF_nullptr);
    int err = __MCF_dtor_queue_pop(elem, queue, dso);
    _MCF_mutex_unlock(mtx);
    return err;
  }

__MCF_DLLEXPORT
void
__MCF_run_static_dtors(_MCF_mutex* mtx, __MCF_dtor_queue* queue, void* dso)
  {
    __MCF_SEH_DEFINE_TERMINATE_FILTER;
    __MCF_dtor_element elem;

    while(do_pop_static_dtor(&elem, mtx, queue, dso) == 0)
      __MCF_invoke_cxa_dtor(elem.__dtor, elem.__this);
  }

/* These are constants that have to be initialized at load time. The
 * initializers prevent them from being placed into the`.bss` section.  */
__MCF_BR(GUID) const __MCF_crt_gthread_guid = { __MCF_GUID(9FB2D15C,C5F2,4AE7,868D,2769591B8E92) };
HANDLE __MCF_crt_heap = __MCF_BAD_PTR;
double __MCF_crt_pf_recip = 1;
SYSTEM_INFO __MCF_crt_sysinfo = { .dwPageSize = 1 };
HMODULE __MCF_crt_kernelbase = __MCF_BAD_PTR;
HMODULE __MCF_crt_ntdll = __MCF_BAD_PTR;
decltype_TlsGetValue2* __MCF_crt_TlsGetValue = __MCF_BAD_PTR;

/* This is a pointer to global data. If this library is linked statically,
 * all instances of this pointer in the same process should point to the
 * same memory. The initializer prevents it from being placed into the
 * `.bss` section.  */
__MCF_crt_xglobals* restrict __MCF_g = __MCF_BAD_PTR;

/* When building the shared library, invoke common routines from the DLL
 * entry point callback. This has the same signature as `DllMain()`.  */
#if defined __MSYS__
#  define DllMainCRTStartup  _msys_dll_entry
#elif defined __CYGWIN__
#  define DllMainCRTStartup  _cygwin_dll_entry
#elif defined _MSC_VER
#  define DllMainCRTStartup  _DllMainCRTStartup
#endif

int
__stdcall
DllMainCRTStartup(PVOID instance, ULONG reason, PVOID reserved);

__MCF_REALIGN_SP
int
__stdcall
DllMainCRTStartup(PVOID instance, ULONG reason, PVOID reserved)
  {
    (void) instance;
    (void) reason;
    (void) reserved;
    return 1;
  }

/* According to GCC documentation, these functions are required by a
 * freestanding implementation. When doing a non-emulative build, they are
 * also exported for reuse.  */
__MCF_DLLEXPORT __attribute__((__flatten__))
void*
memcpy(void* restrict dst, const void* restrict src, size_t size)
  {
    return __MCF_mcopy(dst, src, size);
  }

__MCF_DLLEXPORT __attribute__((__flatten__))
void*
memmove(void* dst, const void* src, size_t size)
  {
    return ((uintptr_t) dst - (uintptr_t) src >= size)
           ? __MCF_mcopy(dst, src, size)
           : __MCF_mcopy_backward(dst, src, size);
  }

__MCF_DLLEXPORT __attribute__((__flatten__))
void*
memset(void* dst, int val, size_t size)
  {
    return __MCF_mfill(dst, val, size);
  }

__MCF_DLLEXPORT __attribute__((__flatten__))
int
memcmp(const void* src, const void* dst, size_t size)
  {
    return __MCF_mcompare(src, dst, size);
  }

#if defined _MSC_VER
/* Microsoft LINK requires this for a reason.  */
const int _fltused __MCF_CRT_RDATA = 0x9875;
#endif

#if defined __MCF_M_X8632
/* On x86-32, the load config directory contains the address and size of the
 * exception handler table. Exception handlers that are not in this table
 * will be rejected by the system. `__MCF_i386_se_handler_table` points to an
 * array of RVAs to valid handlers, and the value of (not the value it points
 * to) `__MCF_i386_se_handler_count` is the number of handlers.  */
extern const ULONG __MCF_i386_se_handler_table[];
extern char __MCF_i386_se_handler_count[];
__asm__ (
"\n .section .rdata, \"dr\""
"\n   .p2align 2"
"\n ___MCF_i386_se_handler_table:"
"\n   .rva ___MCF_seh_top"
"\n   .rva ___MCF_gthr_do_i386_call_once_on_except"
"\n .equiv ___MCF_i386_se_handler_count, (. - ___MCF_i386_se_handler_table) / 4"
"\n .text"
);
#endif

#if defined __MCF_M_ARM64EC
/* This section has been heavily modified from 'chpe.S' from mingw-w64. Only
 * symbols that are documented by Microsoft are kept. Original code is declared
 * to be in the Public Domain.  */
extern const ULONG __MCF_arm64ec_chpe_metadata[];
__asm__ (
"\n .section .rdata, \"dr\""
"\n   .p2align 2"
"\n __MCF_arm64ec_chpe_metadata:"
"\n   .long 1" /* Version */
"\n   .rva __hybrid_code_map"
"\n   .long __hybrid_code_map_count"
"\n   .rva __x64_code_ranges_to_entry_points"
"\n   .rva __arm64x_redirection_metadata"
"\n   .rva __os_arm64x_dispatch_call_no_redirect"
"\n   .rva __os_arm64x_dispatch_ret"
"\n   .rva __os_arm64x_check_call"
"\n   .rva __os_arm64x_check_icall"
"\n   .rva __os_arm64x_check_icall_cfg"
"\n   .rva __arm64x_native_entrypoint"
"\n   .rva __hybrid_auxiliary_iat"
"\n   .long __x64_code_ranges_to_entry_points_count"
"\n   .long __arm64x_redirection_metadata_count"
"\n   .rva __os_arm64x_get_x64_information"
"\n   .rva __os_arm64x_set_x64_information"
"\n   .rva __arm64x_extra_rfe_table"
"\n   .long __arm64x_extra_rfe_table_size"
"\n   .rva __os_arm64x_x64_jump"
"\n   .rva __hybrid_auxiliary_iat_copy"
"\n   .rva __hybrid_auxiliary_delayload_iat"
"\n   .rva __hybrid_auxiliary_delayload_iat_copy"
"\n   .long __hybrid_image_info_bitfield"
"\n   .rva __os_arm64x_helper3"
"\n   .rva __os_arm64x_helper4"
"\n   .rva __os_arm64x_helper5"
"\n   .rva __os_arm64x_helper6"
"\n   .rva __os_arm64x_helper7"
"\n   .rva __os_arm64x_helper8"
/* These are pointers to helper routines, which will be filled after the image
 * is loaded by the operating system.  */
"\n .section .00cfg, \"dr\""
"\n   .p2align 3"
"\n .globl __os_arm64x_dispatch_call_no_redirect"
"\n __os_arm64x_dispatch_call_no_redirect:"
"\n   .quad 0"
"\n .globl __os_arm64x_dispatch_ret"
"\n __os_arm64x_dispatch_ret:"
"\n   .quad 0"
"\n .globl __os_arm64x_check_call"
"\n __os_arm64x_check_call:"
"\n   .quad 0"
"\n .globl __os_arm64x_check_icall"
"\n __os_arm64x_check_icall:"
"\n   .quad 0"
"\n .globl __os_arm64x_check_icall_cfg"
"\n __os_arm64x_check_icall_cfg:"
"\n   .quad 0"
"\n .globl __os_arm64x_get_x64_information"
"\n __os_arm64x_get_x64_information:"
"\n   .quad 0"
"\n .globl __os_arm64x_set_x64_information"
"\n __os_arm64x_set_x64_information:"
"\n   .quad 0"
"\n .globl __os_arm64x_x64_jump"
"\n __os_arm64x_x64_jump:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper3"
"\n __os_arm64x_helper3:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper4"
"\n __os_arm64x_helper4:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper5"
"\n __os_arm64x_helper5:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper6"
"\n __os_arm64x_helper6:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper7"
"\n __os_arm64x_helper7:"
"\n   .quad 0"
"\n .globl __os_arm64x_helper8"
"\n __os_arm64x_helper8:"
"\n   .quad 0"
/* This is the ARM64EC Adjustor Thunk. Calls to this function are synthesized
 * by the compiler.  */
"\n .section .text$__icall_helper_arm64ec, \"x\""
"\n   .p2align 2"
"\n .globl __icall_helper_arm64ec"
"\n .def __icall_helper_arm64ec; .scl 2; .type 32; .endef"
"\n __icall_helper_arm64ec:"
"\n .seh_proc __icall_helper_arm64ec"
"\n   stp fp, lr, [sp, -16]!"
"\n .seh_save_fplr_x 16"
"\n   mov fp, sp"
"\n .seh_set_fp"
"\n .seh_endprologue"
"\n   adrp x16, __os_arm64x_check_icall"
"\n   ldr x16, [x16, :lo12:__os_arm64x_check_icall]"
"\n   blr x16"
"\n .seh_startepilogue"
"\n   ldp fp, lr, [sp], 16"
"\n .seh_save_fplr_x 16"
"\n .seh_endepilogue"
"\n   br x11"
"\n .seh_endproc"
/* This is a common wrapper with an Exit Thunk for x86-64 callback functions
 * that return either values in RAX, or void.  */
"\n .globl __MCF_arm64ec_exit_thunk_p"
"\n .def __MCF_arm64ec_exit_thunk_p; .scl 2; .type 32; .endef"
"\n __MCF_arm64ec_exit_thunk_p:"
"\n .seh_proc __MCF_arm64ec_exit_thunk_p"
"\n   stp fp, lr, [sp, -16]!"
"\n .seh_save_fplr_x 16"
"\n   mov fp, sp"
"\n .seh_set_fp"
"\n   sub sp, sp, 32"
"\n .seh_stackalloc 32"
"\n .seh_endprologue"
"\n   adrp x16, __os_arm64x_dispatch_call_no_redirect"
"\n   ldr x16, [x16, :lo12:__os_arm64x_dispatch_call_no_redirect]"
"\n   blr x16"
"\n   mov x0, x8"
"\n .seh_startepilogue"
"\n   add sp, sp, 32"
"\n .seh_stackalloc 32"
"\n   ldp fp, lr, [sp], 16"
"\n .seh_save_fplr_x 16"
"\n .seh_endepilogue"
"\n   ret"
"\n .seh_endproc"
"\n .text"
);
#endif

struct _IMAGE_LOAD_CONFIG_DIRECTORY_10_0_26100_0
  {
    DWORD Size;
    DWORD TimeDateStamp;
    WORD MajorVersion;
    WORD MinorVersion;
    DWORD GlobalFlagsClear;
    DWORD GlobalFlagsSet;
    DWORD CriticalSectionDefaultTimeout;
    ULONG_PTR DeCommitFreeBlockThreshold;
    ULONG_PTR DeCommitTotalFreeThreshold;
    ULONG_PTR LockPrefixTable;
    ULONG_PTR MaximumAllocationSize;
    ULONG_PTR VirtualMemoryThreshold;
    ULONG_PTR __MCF_64_32(ProcessAffinityMask, ProcessHeapFlags);
    DWORD __MCF_64_32(ProcessHeapFlags, ProcessAffinityMask);
    WORD CSDVersion;
    WORD DependentLoadFlags;
    ULONG_PTR EditList;
    ULONG_PTR SecurityCookie;
    ULONG_PTR SEHandlerTable;
    ULONG_PTR SEHandlerCount;
    ULONG_PTR GuardCFCheckFunctionPointer;
    ULONG_PTR GuardCFDispatchFunctionPointer;
    ULONG_PTR GuardCFFunctionTable;
    ULONG_PTR GuardCFFunctionCount;
    DWORD GuardFlags;
    WORD CodeIntegrity_Flags;
    WORD CodeIntegrity_Catalog;
    DWORD CodeIntegrity_CatalogOffset;
    DWORD CodeIntegrity_Reserved;
    ULONG_PTR GuardAddressTakenIatEntryTable;
    ULONG_PTR GuardAddressTakenIatEntryCount;
    ULONG_PTR GuardLongJumpTargetTable;
    ULONG_PTR GuardLongJumpTargetCount;
    ULONG_PTR DynamicValueRelocTable;
    ULONG_PTR CHPEMetadataPointer;
    ULONG_PTR GuardRFFailureRoutine;
    ULONG_PTR GuardRFFailureRoutineFunctionPointer;
    DWORD DynamicValueRelocTableOffset;
    WORD DynamicValueRelocTableSection;
    WORD Reserved2;
    ULONG_PTR GuardRFVerifyStackPointerFunctionPointer;
    DWORD HotPatchTableOffset;
    DWORD Reserved3;
    ULONG_PTR EnclaveConfigurationPointer;
    ULONG_PTR VolatileMetadataPointer;
    ULONG_PTR GuardEHContinuationTable;
    ULONG_PTR GuardEHContinuationCount;
    ULONG_PTR GuardXFGCheckFunctionPointer;
    ULONG_PTR GuardXFGDispatchFunctionPointer;
    ULONG_PTR GuardXFGTableDispatchFunctionPointer;
    ULONG_PTR CastGuardOsDeterminedFailureMode;
    ULONG_PTR GuardMemcpyFunctionPointer;
  }
const _load_config_used __MCF_CRT_RDATA =
  {
    .Size = sizeof(_load_config_used),
    .DependentLoadFlags = LOAD_LIBRARY_SEARCH_SYSTEM32,
#if defined __MCF_M_X8632
    .SEHandlerTable = (ULONG_PTR) __MCF_i386_se_handler_table,
    .SEHandlerCount = (ULONG_PTR) __MCF_i386_se_handler_count,
#endif
#if defined __MCF_M_ARM64EC
    .CHPEMetadataPointer = (ULONG_PTR) __MCF_arm64ec_chpe_metadata,
#endif
  };
