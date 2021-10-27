#include "idt.h"

/*
 * initialize_idt
 * descriptoon:
 * initialize the idt gates by setup the the corresponding bits to
 * indicate whether it is interrupt, exception, or system call
 * input: none
 * output: none
 * side effect: idt is been load and initialized
 */
void initialize_idt()
{
    int i;                          // loop counter
    lidt(idt_desc_ptr);             // load idt

    // initiazie all the  256 idt
    for (i = 0; i < NUM_VEC; i++)
    {
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = ZERO;
        idt[i].reserved3 = ZERO;
        idt[i].reserved2 = ONE;
        idt[i].reserved1 = ONE;
        idt[i].reserved0 = ZERO;
        idt[i].size = ONE;
        idt[i].dpl = DPL_ZERO;
        idt[i].present = ONE;

        // mark as interrupt gates
        if (i >= start_interrupt)
        {
            idt[i].reserved3 = ZERO;
            SET_IDT_ENTRY(idt[i], general_protection);
        }

        // mark the DPL to 3 indicates a system call
        if (i == SYSTEM_CALL)
        {
            idt[i].dpl = DPL_THREE;
            idt[i].reserved3 = ONE;
        }
    }

    // set idt entries for the exceptions or interrupts
    SET_IDT_ENTRY(idt[DIVISON_BY_ZERO], division_by_zero);
    SET_IDT_ENTRY(idt[SYSTEM_RESERVED], system_reserved);
    SET_IDT_ENTRY(idt[NON_MASKABLE_INTR], non_maskable_interrupt);
    SET_IDT_ENTRY(idt[BREAK_POINT], break_point);
    SET_IDT_ENTRY(idt[OVER_FLOW], over_flow);
    SET_IDT_ENTRY(idt[BOUND_RANGE_EXCEEDED], bound_range_exceeded);
    SET_IDT_ENTRY(idt[INVALID_OPCODE], invalid_opcode);
    SET_IDT_ENTRY(idt[DEVICE_NOT_AVAILABLE], device_not_available);
    SET_IDT_ENTRY(idt[DOUBLE_FAULT], double_fault);
    SET_IDT_ENTRY(idt[COPROCESSOR_SEGMENT_OVERRUN], coprocessor_segment_overrun);
    SET_IDT_ENTRY(idt[INVALID_TSS], invalid_TSS);
    SET_IDT_ENTRY(idt[SEGMENT_NOT_PRESENT], segment_not_present);
    SET_IDT_ENTRY(idt[STACK_SEGMENT_FAULT], stack_segment_fault);
    SET_IDT_ENTRY(idt[GENERAL_PROTECTION], general_protection);
    SET_IDT_ENTRY(idt[PAGE_FAULT], page_fault);
    SET_IDT_ENTRY(idt[RESERVED_EXCEPTION], reserved_exception);
    SET_IDT_ENTRY(idt[FPU_FLOATING_POINT_ERROR], FPU_floating_point_error);
    SET_IDT_ENTRY(idt[ALIGNMENT_CHECK], alignment_check);
    SET_IDT_ENTRY(idt[MACHINE_CHECK], machine_check);
    SET_IDT_ENTRY(idt[SIMD_FLOATING_POINT_EXCEPTION], SIMD_floating_point_exception);

    SET_IDT_ENTRY(idt[SYSTEM_CALL], &syscall_linker);
    SET_IDT_ENTRY(idt[KEYBOARD_LINKER], &keyboard_linker);
    SET_IDT_ENTRY(idt[RTC_LINKER], &rtc_linker);
    SET_IDT_ENTRY(idt[PIT_LINKER], &pit_linker);
}

/*
 *  division_by_zero
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void division_by_zero()
{
    cli();
    clear();
    // int8_t conv_buf[1];
    // conv_buf[0] = 0;
    // puts(conv_buf);
    // puts("divide_by_zero_error_exception!\n");
    printf("division by zero");
    // terminate_by_exception();
    while (1)
        ;
}

/*
 * system_reserved
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void system_reserved()
{
    cli();
    clear();
    // int8_t conv_buf[1];
    // conv_buf[0] = 0;
    // puts(conv_buf);
    // puts("divide_by_zero_error_exception!\n");
    printf("system reserved");
    while (1)
        ;
}

/*
 * non_maskable_interrupt
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void non_maskable_interrupt()
{
    cli();
    clear();
    // puts("non maskable interrupt!\n");
    printf("nmi");
    while (1)
        ;
}

/*
 * break_point
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void break_point()
{
    cli();
    clear();
    // puts("new break point!\n");
    printf("breakp");
    while (1)
        ;
}

/*
 * over_flow
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void over_flow()
{
    cli();
    clear();
    // puts("overflow!\n");
    printf("overflow");
    while (1)
        ;
}

/*
 * bound_range_exceeded
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void bound_range_exceeded()
{
    cli();
    clear();
    // puts("Exceed the range!\n");
    printf("brx");
    while (1)
        ;
}

/*
 * invalid_opcode
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void invalid_opcode()
{
    cli();
    clear();
    // puts("Invalid opcode!\n");
    printf("invalid");
    while (1)
        ;
}

/*
 * device_not_available
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void device_not_available()
{
    cli();
    clear();
    // puts("device not available!\n");
    printf("device_not_available");
    while (1)
        ;
}

/*
 * double_fault
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void double_fault()
{
    cli();
    clear();
    // puts("double fault!\n");
    printf("double_fault");
    while (1)
        ;
}

/*
 * coprocessor_segment_overrun
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void coprocessor_segment_overrun()
{
    cli();
    clear();
    // puts("coprocessor segment overrun!\n");
    printf("coprocessor");
    while (1)
        ;
}

/*
 * invalid_TSS
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void invalid_TSS()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("invalid_tss");
    while (1)
        ;
}

/*
 * segment_not_present
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void segment_not_present()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("segment_not");
    while (1)
        ;
}

/*
 * stack_segment_fault
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void stack_segment_fault()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("stack segment");
    while (1)
        ;
}

/*
 * general_protection
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void general_protection()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("general");
    while (1)
        ;
}

/*
 * page_fault
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void page_fault()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("page fault");
    // terminate_by_exception();
    while(1) ;
}

/*
 * reserved_exception
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void reserved_exception()
{
    cli();
    clear();
    printf("reserved");
    while (1)
        ;
}

/*
 * FPU_floating_point_error
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void FPU_floating_point_error()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("FPU");
    while (1)
        ;
}

/*
 * alignment_check
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void alignment_check()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("alignment");
    while (1)
        ;
}

/*
 * machine_check
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void machine_check()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("machine");
    while (1)
        ;
}

/*
 * SIMD_floating_point_exception
 * decription:
 * interrupt handler the deals with the given interrupt
 * input: none
 * output: none
 * sideffect: print the interrupt type on screen and hold the control
 */
void SIMD_floating_point_exception()
{
    cli();
    clear();
    // puts("divide_by_zero_error_exception!\n");
    printf("SIMD");
    while (1)
        ;
}

/*
 * SystemCall_temp
 * decription:
 * identify which system call and jump to the corresponding handler to
 * deal with the given interrupt.
 * input: none
 * output: none
 * sideffect: system call is not implemented yet
 */
void SystemCall_temp()
{
    int call_num;
    printf("system call");
    asm volatile(
        "movl %%eax, %0"
        : "=r"(call_num)
        :
        : "memory");

    switch (call_num)
    {
    case (SYSCALL_ONE):
        // putc("halt");
        break;
    case (SYSCALL_TWO):
        // putc("execute");
        break;
    case (SYSCALL_THREE):
        // putc("read");
        break;
    case (SYSCALL_FOUR):
        // putc("write");
        break;
    case (SYSCALL_FIVE):
        // putc("open");
        break;
    case (SYSCALL_SIX):
        // putc("close");
        break;
    case (SYSCALL_SEVEN):
        // putc("getargs");
        break;
    case (SYSCALL_EIGHT):
        // putc("vidmap");
        break;
    case (SYSCALL_NINE):
        // putc("set_handler");
        break;
    case (SYSCALL_TEN):
        // putc("SIGRETURN");
        break;
    }
}
