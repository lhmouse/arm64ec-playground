int _MCF_once_wait(int* once, const void* timeout_opt);
void _MCF_once_release(int* once);
typedef void my_once_fn(void*);

__declspec(dllexport)
void
my_once(int* once, my_once_fn* init_proc, void* arg)
  {
    int __err = _MCF_once_wait(once, 0);
    if(__err == 0)
      return;  /* already initialized  */

    /* This can't be declared as a function, otherwise GCC will make the
     * definition visible externally.  */
    extern const char do_call_once_seh_take_over[];
    typedef __typeof__(my_once) self_type;
    (*(self_type*) do_call_once_seh_take_over) (once, init_proc, arg);
  }

__asm__ (
"\n .text"
"\n   .p2align 2"
"\n .def do_call_once_seh_take_over; .scl 3; .type 32; .endef"
"\n do_call_once_seh_take_over:"
#if defined __i686__
 unimplemented
#elif defined __amd64__ && !defined __arm64ec__
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
#elif defined __aarch64__ || defined __arm64ec__
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
"\n   stp fp, lr, [sp, -32]!"
"\n .seh_save_fplr_x 32"
"\n   mov fp, sp"
"\n .seh_set_fp"
"\n .seh_endprologue"
/* Stash `once` for the handler.  */
"\n   str x0, [sp, 16]"
/* Make the call `(*init_proc) (arg)`.  */
"\n   mov x0, x2"
#  if defined __aarch64__
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
#endif
"\n .text"
);
