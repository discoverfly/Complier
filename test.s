	.file	"test.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	.align 4
LC2:
	.ascii "please input your student number:\0"
LC3:
	.ascii "%d\0"
	.align 4
LC6:
	.ascii "the score of student number %d is %f higher than 60.\12\0"
	.align 4
LC7:
	.ascii "the score of student number %d is %f lower than 60.\12\0"
	.data
	.align 4
LC0:
	.long	76
	.long	82
	.long	90
	.long	86
	.long	79
	.long	62
	.align 4
LC1:
	.long	2
	.long	2
	.long	1
	.long	2
	.long	2
	.long	3
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	andl	$-16, %esp
	subl	$112, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	call	___main
	leal	72(%esp), %edx
	movl	$LC0, %ebx
	movl	$6, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	leal	48(%esp), %edx
	movl	$LC1, %ebx
	movl	$6, %eax
	movl	%edx, %edi
	movl	%ebx, %esi
	movl	%eax, %ecx
	rep movsl
	movl	$LC2, (%esp)
	call	_printf
	leal	44(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC3, (%esp)
	call	_scanf
	movl	LC4, %eax
	movl	%eax, 108(%esp)
	movl	$0, 104(%esp)
	movl	$0, 100(%esp)
	jmp	L2
L3:
	movl	100(%esp), %eax
	movl	72(%esp,%eax,4), %edx
	movl	100(%esp), %eax
	movl	48(%esp,%eax,4), %eax
	imull	%edx, %eax
	movl	%eax, 28(%esp)
	fildl	28(%esp)
	flds	108(%esp)
	faddp	%st, %st(1)
	fstps	108(%esp)
	movl	100(%esp), %eax
	movl	48(%esp,%eax,4), %eax
	addl	%eax, 104(%esp)
	incl	100(%esp)
L2:
	cmpl	$5, 100(%esp)
	jle	L3
	fildl	104(%esp)
	flds	108(%esp)
	fdivp	%st, %st(1)
	fstps	96(%esp)
	flds	96(%esp)
	flds	LC5
	fxch	%st(1)
	fucompp
	fnstsw	%ax
	testb	$5, %ah
	jne	L9
L8:
	flds	96(%esp)
	flds	LC5
	fsubrp	%st, %st(1)
	fstps	96(%esp)
	flds	96(%esp)
	movl	44(%esp), %eax
	fstpl	8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC6, (%esp)
	call	_printf
	jmp	L6
L9:
	flds	LC5
	fsubs	96(%esp)
	fstps	96(%esp)
	flds	96(%esp)
	movl	44(%esp), %eax
	fstpl	8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC7, (%esp)
	call	_printf
L6:
	movl	$0, %eax
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE6:
	.section .rdata,"dr"
	.align 4
LC4:
	.long	0
	.align 4
LC5:
	.long	1114636288
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_scanf;	.scl	2;	.type	32;	.endef
