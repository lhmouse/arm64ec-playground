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
#define __MCF_GTHR_AUX_IMPORT  __declspec(dllexport)
#define __MCF_GTHR_AUX_INLINE  __declspec(dllexport)
#include "gthr_aux.h"

__declspec(dllexport)
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
"\n .section .text$__MCF_gthr_call_once_seh_take_over, \"x\""
"\n   .p2align 2"
"\n .def " __MCF_USYM "do_call_once_seh_take_over; .scl 3; .type 32; .endef"
"\n " __MCF_USYM "do_call_once_seh_take_over:"
#if defined __MCF_M_X8632_ASM
 unimplemented
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
