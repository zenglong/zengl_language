.section .text
.globl setjmp
setjmp:
	mov 4(%esp),%edx
	mov %ebx, (%edx)
	mov %esi, 4(%edx)
	mov %edi, 8(%edx)
	mov %ebp, 12(%edx)
	leal 4(%esp), %ecx
	mov %ecx, 16(%edx)
	mov 0(%esp), %ecx
	mov %ecx, 20(%edx)
	xor %eax, %eax
	ret

