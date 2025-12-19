/* This file is part of MCF Gthread.
 * Copyright (C) 2022-2025 LH_Mouse. All wrongs reserved.
 *
 * MCF Gthread is free software. Licensing information is included in
 * LICENSE.TXT as a whole. The GCC Runtime Library Exception applies
 * to this file.  */

#include "xprecompiled.h"
#define __MCF_GTHR_AUX_IMPORT  __MCF_DLLEXPORT
#define __MCF_GTHR_AUX_INLINE  __MCF_DLLEXPORT
#include "gthr_aux.h"
#include "xglobals.h"

__MCF_DLLEXPORT
void
__MCF_gthr_call_once_seh_take_over(_MCF_once* once, __MCF_cxa_dtor_any_ init_proc, void* arg)
  {
    /* This can't be declared as a function, otherwise GCC will make the
     * definition visible externally.  */
    extern const char do_call_once_seh_take_over[];
    typedef __typeof__(__MCF_gthr_call_once_seh_take_over) self_type;
    (* __MCF_CAST_PTR(self_type, do_call_once_seh_take_over)) (once, init_proc, arg);
  }

__asm__ (
"\n .section .text$" __MCF_USYM "__MCF_gthr_call_once_seh_take_over, \"x\""
"\n .def " __MCF_USYM "do_call_once_seh_take_over; .scl 3; .type 32; .endef"
"\n " __MCF_USYM "do_call_once_seh_take_over:"
#if defined __MCF_M_X8632_ASM
/* On x86-32, SEH is stack-based. The stack is used as follows:
 *
 *    -24: argument to subroutines
 *    -20: unused
 *    -16: establisher frame; pointer to previous frame
 *    -12: `_do_i386_call_once_on_except`
 *     -8: unused
 *     -4: saved ESI
 * EBP  0: saved frame pointer
 *      4: return address
 * CFA  8: `once`
 *     12: `init_proc`
 *     16: `arg`
 */
"\n   push ebp"
"\n   mov ebp, esp"
"\n   push esi"
"\n   sub esp, 20"
/* Initialize the constant zero.  */
"\n   xor esi, esi"
/* Install an SEH handler.  */
"\n   mov eax, DWORD PTR fs:[esi]"
"\n   lea ecx, [ebp - 16]"
"\n   mov DWORD PTR [ecx], eax"
"\n   mov DWORD PTR [ecx + 4], OFFSET _do_i386_call_once_on_except"
"\n   mov DWORD PTR fs:[esi], ecx"
/* Make the call `(*init_proc) (arg)`. The argument is passed both via the
 * ECX register and on the stack, to allow both `__cdecl` and `__thiscall`
 * functions to work properly.  */
"\n   mov ecx, DWORD PTR [ebp + 16]"
"\n   mov DWORD PTR [ebp - 24], ecx"
"\n   call DWORD PTR [ebp + 12]"
/* Dismantle the SEH handler.  */
"\n   mov eax, DWORD PTR [ebp - 16]"
"\n   mov DWORD PTR fs:[esi], eax"
/* Disarm the once flag with a tail call.  */
"\n   lea esp, [ebp - 4]"
"\n   pop esi"
"\n   pop ebp"
"\n   jmp __MCF_once_release"
/* Define the exception handler, which is called either when an exception is
 * raised, or the stack is being unwound.  */
"\n .def _do_i386_call_once_on_except; .scl 3; .type 32; .endef"
"\n _do_i386_call_once_on_except:"
/* Check whether `ExceptionFlags` contains `EXCEPTION_UNWINDING`.  */
"\n   mov ecx, DWORD PTR [esp + 4]"
"\n   test BYTE PTR [ecx + 4], 2"
"\n   jz 1001f"
/* Locate the once flag from `EstablisherFrame`, and reset it.  */
"\n   mov eax, DWORD PTR [esp + 8]"
"\n   push DWORD PTR [eax + 24]"
"\n   call __MCF_once_abort"
"\n   pop eax"
"\n 1001:"
/* Return `ExceptionContinueSearch`.  */
"\n   mov eax, 1"
"\n   ret"
#  if defined __MCF_IN_DLL
"\n .globl ___MCF_gthr_do_i386_call_once_on_except"
"\n .equiv ___MCF_gthr_do_i386_call_once_on_except, _do_i386_call_once_on_except"
#  elif defined _MSC_VER
"\n .safeseh _do_i386_call_once_on_except"
#  endif
#elif defined __MCF_M_X8664_ASM
/* On x86-64, SEH is table-based. We register an unwind handler which is not
 * called when an exception is raised, but is called when the stack is being
 * unwound. The stack is used as follows:
 *
 *    -32: shadow slot for subroutines
 *    -24: ditto
 *    -16: ditto
 *     -8: ditto
 * RBP  0: establisher frame; saved frame pointer
 *      8: return address
 * CFA 16: shadow slot for `once` from RCX
 *     24: shadow slot for `init_proc` from RDX
 *     32: shadow slot for `arg` from R8
 *     40: unused
 */
"\n .seh_proc do_call_once_seh_take_over"
"\n .seh_handler do_amd64_call_once_on_unwind, @unwind"
"\n   push rbp"
"\n .seh_pushreg rbp"
"\n   mov rbp, rsp"
"\n .seh_setframe rbp, 0"
"\n   sub rsp, 32"
"\n .seh_stackalloc 32"
"\n .seh_endprologue"
/* Stash `once` for the handler.  */
"\n   mov QWORD PTR [rbp + 16], rcx"
/* Make the call `(*init_proc) (arg)`.  */
"\n   mov rcx, r8"
"\n   call rdx"
/* Disarm the once flag with a tail call. The x64 stack unwinder recognizes
 * `add rsp, SIZE` as the start of the epilogue.  */
"\n   mov rcx, QWORD PTR [rbp + 16]"
"\n   add rsp, 32"
"\n   pop rbp"
"\n   jmp _MCF_once_release"
"\n .seh_endproc"
/* Define the unwind handler, which is called the stack is being unwound.  */
"\n .def do_amd64_call_once_on_unwind; .scl 3; .type 32; .endef"
"\n do_amd64_call_once_on_unwind:"
"\n   sub rsp, 40"
/* Locate the once flag from `EstablisherFrame`, and reset it.  */
"\n   mov rcx, QWORD PTR [rdx + 16]"
"\n   call _MCF_once_abort"
/* Return `ExceptionContinueSearch`.  */
"\n   mov eax, 1"
"\n   add rsp, 40"
"\n   ret"
#elif defined __MCF_M_ARM64_ASM
/* On ARM64, SEH is table-based. Unlike x86-64 but like x86-32, there is only
 * one kind of handler which is called in either case. The stack is used as
 * follows:
 *
 *  SP  0: saved FP
 *      8: saved LR
 *     16: `once` from X0
 *     24: unused
 * CFA 32: establisher frame
 */
"\n .seh_proc do_call_once_seh_take_over"
"\n .seh_handler do_arm64_call_once_on_except, @except"
"\n   stp fp, lr, [sp, -32]!"
"\n .seh_save_fplr_x 32"
"\n   mov fp, sp"
"\n .seh_set_fp"
"\n .seh_endprologue"
/* Stash `once` for the handler.  */
"\n   str x0, [sp, 16]"
/* Make the call `(*init_proc) (arg)`.  */
"\n   mov x0, x2"
#  if defined __MCF_M_ARM64
"\n   blr x1"
#  else
"\n   mov x11, x1"
"\n   adrp x10, __MCF_arm64ec_exit_thunk_p"
"\n   add x10, x10, :lo12:__MCF_arm64ec_exit_thunk_p"
"\n   bl __icall_helper_arm64ec"
#  endif
/* Disarm the once flag with a tail call.  */
"\n   ldr x0, [sp, 16]"
"\n .seh_startepilogue"
"\n   ldp fp, lr, [sp], 32"
"\n .seh_save_fplr_x 32"
"\n .seh_endepilogue"
"\n   b _MCF_once_release"
"\n .seh_endproc"
/* Define the exception handler, which is called either when an exception is
 * raised, or the stack is being unwound.  */
"\n .def do_arm64_call_once_on_except; .scl 3; .type 32; .endef"
"\n do_arm64_call_once_on_except:"
"\n   stp fp, lr, [sp, -16]!"
/* Check whether `ExceptionFlags` contains `EXCEPTION_UNWINDING`.  */
"\n   ldr w8, [x0, 4]"
"\n   tbz w8, 1, 3001f"
/* Locate the once flag from `EstablisherFrame`, and reset it.  */
"\n   ldur x0, [x1, -16]"
"\n   bl _MCF_once_abort"
"\n 3001:"
/* Return `ExceptionContinueSearch`.  */
"\n   mov w0, 1"
"\n   ldp fp, lr, [sp], 16"
"\n   ret"
#endif
"\n .text"
);
