#ifndef _SCHE_H
#define _SCHE_H

#include "types.h"
#include "lib.h"
#include "pcb.h"
#include "paging.h"
#include "do_sys.h"
#include "x86_desc.h"

#define MAX_TERMINAL_NUM            3
#define _2MB                        0x00200000
#define _8MB                        0x00800000
#define _4KB                        0x00001000
#define _4MB                        0x00400000
#define _8KB                        0x00002000
#define _128MB                      0x08000000
#define YES                         1
#define NO                          0
#define KEYBOARD_BUFFER_SIZE        128

#define TERM_ZERO                   0

typedef struct{

    uint32_t initialized;

    int8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
    // cursor position
    int x_pos;
    int y_pos;

    int32_t enter_flag;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    uint32_t esp0;

    //this stores the current pid number running on one terminal
    int32_t current_pid;
    int32_t user_vid_mem;
    int curr_buffer_ptr;
} scheduler_t;

scheduler_t terminals[MAX_TERMINAL_NUM];
uint32_t current_running_terminal;
uint32_t current_looking_terminal;

int32_t schedule_init(uint32_t t_id);

void initialize_new_ternimals();

void switch_screen(uint32_t next_terminal_id);

int32_t get_current_running_terminal();
int32_t get_current_looking_terminal();

void set_next_running_terminal(uint32_t cur_running);

#endif
