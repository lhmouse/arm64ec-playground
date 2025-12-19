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
#define __MCF_XGLOBALS_IMPORT  __declspec(dllexport)
#define __MCF_XGLOBALS_INLINE  __declspec(dllexport)
#define __MCF_XGLOBALS_READONLY
#include "fwd.h"

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

int
__stdcall
DllMainCRTStartup(PVOID instance, ULONG reason, PVOID reserved)
  {
    (void) instance;
    (void) reason;
    (void) reserved;
    return 1;
  }

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
const _load_config_used __attribute__((__section__(".rdata"), __used__)) =
  {
    .Size = sizeof(_load_config_used),
    .DependentLoadFlags = LOAD_LIBRARY_SEARCH_SYSTEM32,
#if defined __MCF_M_ARM64EC
    .CHPEMetadataPointer = (ULONG_PTR) __MCF_arm64ec_chpe_metadata,
#endif
  };
