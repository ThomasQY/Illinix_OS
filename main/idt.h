#ifndef IDT_H
#define IDT_H

#include "lib.h"
#include "x86_desc.h"
#include "paging.h"
#include "i8259.h"
#include "keyboard_linker.h"
#include "rtc_linker.h"
#include "syscall_linker.h"
#include "pit_linker.h"

#define initialize_zero         0
#define devide_by_zero          0

#define ONE                     1
#define ZERO                    0

// DPL level
#define DPL_ZERO                0
#define DPL_THREE               3

// the idt exception or intrruption number
#define DIVISON_BY_ZERO                     0
#define SYSTEM_RESERVED                     1
#define NON_MASKABLE_INTR                   2
#define BREAK_POINT                         3
#define OVER_FLOW                           4
#define BOUND_RANGE_EXCEEDED                5
#define INVALID_OPCODE                      6
#define DEVICE_NOT_AVAILABLE                7
#define DOUBLE_FAULT                        8
#define COPROCESSOR_SEGMENT_OVERRUN         9
#define INVALID_TSS                         10
#define SEGMENT_NOT_PRESENT                 11
#define STACK_SEGMENT_FAULT                 12
#define GENERAL_PROTECTION                  13
#define PAGE_FAULT                          14
#define RESERVED_EXCEPTION                  15
#define FPU_FLOATING_POINT_ERROR            16
#define ALIGNMENT_CHECK                     17
#define MACHINE_CHECK                       18
#define SIMD_FLOATING_POINT_EXCEPTION       19
#define KEYBOARD_LINKER                     33
#define RTC_LINKER                          40
#define PIT_LINKER                          0x20

// system call
#define SYSCALL_ONE                         1
#define SYSCALL_TWO                         2
#define SYSCALL_THREE                       3
#define SYSCALL_FOUR                        4
#define SYSCALL_FIVE                        5
#define SYSCALL_SIX                         6
#define SYSCALL_SEVEN                       7
#define SYSCALL_EIGHT                       8
#define SYSCALL_NINE                        9
#define SYSCALL_TEN                         10


// initialize exception functions
void initialize_idt();
void division_by_zero();
void system_reserved();
void non_maskable_interrupt();
void break_point();
void over_flow();
void bound_range_exceeded();
void invalid_opcode();
void device_not_available();
void double_fault();
void coprocessor_segment_overrun();
void invalid_TSS();
void segment_not_present();
void stack_segment_fault();
void general_protection();
void page_fault();
void reserved_exception();
void FPU_floating_point_error();
void alignment_check();
void machine_check();
void SIMD_floating_point_exception();
void SystemCall_temp();
void Keyboard_input();


#endif
