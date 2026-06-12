	.file	"syscall_test.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movabsq	$5413375609171371336, %rax
	movabsq	$2851499209028197, %rdx
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movq	%rsp, %rdx
	movl	$15, %ecx
	movl	$1, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	syscall@PLT
	movl	$0, %esi
	movl	$60, %edi
	movl	$0, %eax
	call	syscall@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L4
	movl	$0, %eax
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L4:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
