	.def	"@feat.00";
	.scl	3;
	.type	0;
	.endef
	.globl	"@feat.00"
"@feat.00" = 0
	.file	"gthr_aux.c"
	.def	"#my_once";
	.scl	2;
	.type	32;
	.endef
	.section	.text,"xr",discard,"#my_once"
	.globl	"#my_once"                      // -- Begin function #my_once
	.p2align	2
"#my_once":                             // @"#my_once"
	.weak_anti_dep	my_once
my_once = "#my_once"
.seh_proc "#my_once"
// %bb.0:
	sub	sp, sp, #48
	.seh_stackalloc	48
	str	x30, [sp, #32]                  // 8-byte Folded Spill
	.seh_save_reg	x30, 32
	.seh_endprologue
	str	x2, [sp, #24]
	str	x1, [sp, #16]
	str	x0, [sp, #8]
	ldr	x0, [sp, #8]
	mov	x1, xzr
	bl	"#_MCF_once_wait"
	str	w0, [sp, #4]
	ldr	w8, [sp, #4]
	cbnz	w8, .LBB0_2
	b	.LBB0_1
.LBB0_1:
	b	.LBB0_3
.LBB0_2:
	ldr	x11, [sp, #16]
	ldr	x0, [sp, #24]
	adrp	x8, __os_arm64x_check_icall
	ldr	x8, [x8, :lo12:__os_arm64x_check_icall]
	adrp	x10, $iexit_thunk$cdecl$v$i8
	add	x10, x10, :lo12:$iexit_thunk$cdecl$v$i8
	blr	x8
	blr	x11
	b	.LBB0_3
.LBB0_3:
	.seh_startepilogue
	ldr	x30, [sp, #32]                  // 8-byte Folded Reload
	.seh_save_reg	x30, 32
	add	sp, sp, #48
	.seh_stackalloc	48
	.seh_endepilogue
	ret
	.seh_endfunclet
	.seh_endproc
                                        // -- End function
	.def	$iexit_thunk$cdecl$v$i8;
	.scl	2;
	.type	32;
	.endef
	.section	.wowthk$aa,"xr",discard,$iexit_thunk$cdecl$v$i8
	.globl	$iexit_thunk$cdecl$v$i8         // -- Begin function $iexit_thunk$cdecl$v$i8
	.p2align	2
$iexit_thunk$cdecl$v$i8:                // @"$iexit_thunk$cdecl$v$i8"
.seh_proc $iexit_thunk$cdecl$v$i8
// %bb.0:
	sub	sp, sp, #48
	.seh_stackalloc	48
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	.seh_save_fplr	32
	add	x29, sp, #32
	.seh_add_fp	32
	.seh_endprologue
	adrp	x8, __os_arm64x_dispatch_call_no_redirect
	ldr	x16, [x8, :lo12:__os_arm64x_dispatch_call_no_redirect]
	blr	x16
	.seh_startepilogue
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	.seh_save_fplr	32
	add	sp, sp, #48
	.seh_stackalloc	48
	.seh_endepilogue
	ret
	.seh_endfunclet
	.seh_endproc
                                        // -- End function
	.def	$ientry_thunk$cdecl$v$i8i8i8;
	.scl	2;
	.type	32;
	.endef
	.section	.wowthk$aa,"xr",discard,$ientry_thunk$cdecl$v$i8i8i8
	.globl	$ientry_thunk$cdecl$v$i8i8i8    // -- Begin function $ientry_thunk$cdecl$v$i8i8i8
	.p2align	2
$ientry_thunk$cdecl$v$i8i8i8:           // @"$ientry_thunk$cdecl$v$i8i8i8"
.seh_proc $ientry_thunk$cdecl$v$i8i8i8
// %bb.0:
	stp	q6, q7, [sp, #-176]!            // 32-byte Folded Spill
	.seh_save_any_reg_px	q6, 176
	stp	q8, q9, [sp, #32]               // 32-byte Folded Spill
	.seh_save_any_reg_p	q8, 32
	stp	q10, q11, [sp, #64]             // 32-byte Folded Spill
	.seh_save_any_reg_p	q10, 64
	stp	q12, q13, [sp, #96]             // 32-byte Folded Spill
	.seh_save_any_reg_p	q12, 96
	stp	q14, q15, [sp, #128]            // 32-byte Folded Spill
	.seh_save_any_reg_p	q14, 128
	stp	x29, x30, [sp, #160]            // 16-byte Folded Spill
	.seh_save_fplr	160
	add	x29, sp, #160
	.seh_add_fp	160
	.seh_endprologue
	blr	x9
	adrp	x8, __os_arm64x_dispatch_ret
	ldr	x0, [x8, :lo12:__os_arm64x_dispatch_ret]
	.seh_startepilogue
	ldp	x29, x30, [sp, #160]            // 16-byte Folded Reload
	.seh_save_fplr	160
	ldp	q14, q15, [sp, #128]            // 32-byte Folded Reload
	.seh_save_any_reg_p	q14, 128
	ldp	q12, q13, [sp, #96]             // 32-byte Folded Reload
	.seh_save_any_reg_p	q12, 96
	ldp	q10, q11, [sp, #64]             // 32-byte Folded Reload
	.seh_save_any_reg_p	q10, 64
	ldp	q8, q9, [sp, #32]               // 32-byte Folded Reload
	.seh_save_any_reg_p	q8, 32
	ldp	q6, q7, [sp], #176              // 32-byte Folded Reload
	.seh_save_any_reg_px	q6, 176
	.seh_endepilogue
	br	x0
	.seh_endfunclet
	.seh_endproc
                                        // -- End function
	.def	$iexit_thunk$cdecl$i8$i8i8;
	.scl	2;
	.type	32;
	.endef
	.section	.wowthk$aa,"xr",discard,$iexit_thunk$cdecl$i8$i8i8
	.globl	$iexit_thunk$cdecl$i8$i8i8      // -- Begin function $iexit_thunk$cdecl$i8$i8i8
	.p2align	2
$iexit_thunk$cdecl$i8$i8i8:             // @"$iexit_thunk$cdecl$i8$i8i8"
.seh_proc $iexit_thunk$cdecl$i8$i8i8
// %bb.0:
	sub	sp, sp, #48
	.seh_stackalloc	48
	stp	x29, x30, [sp, #32]             // 16-byte Folded Spill
	.seh_save_fplr	32
	add	x29, sp, #32
	.seh_add_fp	32
	.seh_endprologue
	adrp	x8, __os_arm64x_dispatch_call_no_redirect
	ldr	x16, [x8, :lo12:__os_arm64x_dispatch_call_no_redirect]
	blr	x16
	mov	x0, x8
	.seh_startepilogue
	ldp	x29, x30, [sp, #32]             // 16-byte Folded Reload
	.seh_save_fplr	32
	add	sp, sp, #48
	.seh_stackalloc	48
	.seh_endepilogue
	ret
	.seh_endfunclet
	.seh_endproc
                                        // -- End function
	.def	"#_MCF_once_wait$exit_thunk";
	.scl	2;
	.type	32;
	.endef
	.section	.wowthk$aa,"xr",discard,"#_MCF_once_wait$exit_thunk"
	.globl	"#_MCF_once_wait$exit_thunk"    // -- Begin function #_MCF_once_wait$exit_thunk
	.p2align	2
"#_MCF_once_wait$exit_thunk":           // @"#_MCF_once_wait$exit_thunk"
	.weak_anti_dep	_MCF_once_wait
_MCF_once_wait = "#_MCF_once_wait"
	.weak_anti_dep	"#_MCF_once_wait"
"#_MCF_once_wait" = "#_MCF_once_wait$exit_thunk"
.seh_proc "#_MCF_once_wait$exit_thunk"
// %bb.0:
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.seh_save_reg_x	x30, 16
	.seh_endprologue
	adrp	x8, __os_arm64x_check_icall
	ldr	x8, [x8, :lo12:__os_arm64x_check_icall]
	adrp	x11, _MCF_once_wait
	add	x11, x11, :lo12:_MCF_once_wait
	adrp	x10, $iexit_thunk$cdecl$i8$i8i8
	add	x10, x10, :lo12:$iexit_thunk$cdecl$i8$i8i8
	blr	x8
	.seh_startepilogue
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	.seh_save_reg_x	x30, 16
	.seh_endepilogue
	br	x11
	.seh_endfunclet
	.seh_endproc
                                        // -- End function
	.section	.hybmp$x,"yi"
	.symidx	"#my_once"
	.symidx	$ientry_thunk$cdecl$v$i8i8i8
	.word	1
	.symidx	_MCF_once_wait
	.symidx	$iexit_thunk$cdecl$i8$i8i8
	.word	4
	.symidx	"#_MCF_once_wait$exit_thunk"
	.symidx	_MCF_once_wait
	.word	0
	.section	.drectve,"yni"
	.ascii	" /EXPORT:#my_once,EXPORTAS,my_once"
	.section	.debug$S,"dr"
	.p2align	2, 0x0
	.word	4                               // Debug section magic
	.word	241
	.word	.Ltmp1-.Ltmp0                   // Subsection size
.Ltmp0:
	.hword	.Ltmp3-.Ltmp2                   // Record length
.Ltmp2:
	.hword	4353                            // Record kind: S_OBJNAME
	.word	0                               // Signature
	.byte	0                               // Object name
	.p2align	2, 0x0
.Ltmp3:
	.hword	.Ltmp5-.Ltmp4                   // Record length
.Ltmp4:
	.hword	4412                            // Record kind: S_COMPILE3
	.word	16384                           // Flags and language
	.hword	246                             // CPUType
	.hword	21                              // Frontend version
	.hword	1
	.hword	7
	.hword	0
	.hword	21017                           // Backend version
	.hword	0
	.hword	0
	.hword	0
	.asciz	"clang version 21.1.7 (https://github.com/llvm/llvm-project.git 292dc2b86f66e39f4b85ec8b185fd8b60f5213ce)" // Null-terminated compiler version string
	.p2align	2, 0x0
.Ltmp5:
.Ltmp1:
	.p2align	2, 0x0
	.addrsig
	.addrsig_sym "#my_once"
	.addrsig_sym _MCF_once_wait
	.addrsig_sym $iexit_thunk$cdecl$v$i8
	.addrsig_sym $ientry_thunk$cdecl$v$i8i8i8
	.addrsig_sym $iexit_thunk$cdecl$i8$i8i8
	.addrsig_sym "#_MCF_once_wait$exit_thunk"
	.addrsig_sym __os_arm64x_check_icall
	.addrsig_sym __os_arm64x_dispatch_call_no_redirect
