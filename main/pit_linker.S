/* pit_linker.S - The assembly linkage to the PIT handler */

#define ASM     1

.text

.global pit_linker

# align four
.align 4

pit_linker:
	# save all the registers and flags
	push     %fs
	push     %es
	push     %ds
	push     %eax
	push     %ebp
	push     %edi
	push     %esi
	push     %edx
	push     %ecx
	push     %ebx

	# call the PIT handler
	call pit_handler

	# restore all the flags and registers
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	pop %ebp
	pop %eax
	pop %ds
	pop %es
	pop %fs

	# interrupt return
	iret
