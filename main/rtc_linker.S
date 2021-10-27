/* rtc_linker.S - The assembly linkage to the RTC handler */

#define ASM     1

.text

.global rtc_linker

# align four
.align 4

rtc_linker:
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

	# call the RTC handler
	call rtc_handler

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
