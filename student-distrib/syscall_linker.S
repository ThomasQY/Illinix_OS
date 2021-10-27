 # acquire eax and jump to corresponding handler
.text
.global syscall_linker
.align 4



syscall_linker:
    # check valid eax
    cmpl $10,%eax
    jg invalid
	cmpl $0, %eax
	jg valid_call
    # invalid call, return -1
invalid:
  	movl $-1, %eax
    iret
valid_call:
    # save registers, jump to corresponding call function
    pushl     %fs
  	pushl     %es
  	pushl     %ds
  	pushl     %ebp
  	pushl     %edi
  	pushl     %esi
  	pushl     %edx
  	pushl     %ecx
  	pushl     %ebx
	# pushal
    # push parameters
    subl $1,%eax                # jump table is 0_indexed
    pushl %edx
  	pushl %ecx
  	pushl %ebx
    
    call *syscall_table(,%eax,4) # 4 for .long
    # pop parameters
    addl $12,%esp               # 3 parameters , 3*4 = 12

    popl %ebx
  	popl %ecx
  	popl %edx
  	popl %esi
  	popl %edi
  	popl %ebp
 	popl %ds
 	popl %es
  	popl %fs
	# movl %eax, EAX_TEMP
	# popal
	# movl EAX_TEMP, %eax
    iret

EAX_TEMP:
.long 0	
syscall_table:
  .long halt,execute,read,write,open,close,getargs,vidmap,set_handler,sig_return
