#define ASM     1

.text

.global keyboard_linker

# align four
.align 4
keyboard_linker:
	# save all flags and registers
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
	
	# call the keyboard handler
	call keyboard_input
	
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

