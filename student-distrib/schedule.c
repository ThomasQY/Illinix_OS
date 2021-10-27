#include "schedule.h"

/*
 * initialize_new_ternimal
 * description: first time initialize ternimal struture
 * input: none
 * output: none
 */
void initialize_new_ternimals()
{
    int i;
    uint32_t my_esp;
    uint32_t my_ebp;

    // initialize all terminals
    for (i = 0; i < MAX_TERMINAL_NUM; i++)
    {
        // save the current esp, ebp to my_esp, my_ebp
        my_esp = _8MB - _8KB * i;
        my_ebp = _8MB - _8KB * i;
        if(i == TERM_ZERO) my_esp = my_esp - ADDR_OFFSET;
        terminals[i].saved_ebp = my_ebp;
        terminals[i].saved_esp = my_esp;
        terminals[i].enter_flag = LOW;
        terminals[i].user_vid_mem = LOW;
    }
    terminals[TERM_ZERO].initialized = YES;

    current_looking_terminal = TERM_ZERO;
    current_running_terminal = TERM_ZERO;
}

/*
 * switch_screen
 * description: switch the looking screen to the given terminal id
 * input: next_terminal_id -- the terminal id that will need to be switched to 
 * - save and restore video memory
 * - switch video paging
 * - switch keyboard buffer
 * - update visible video coordinates
 * output: none
 */
void switch_screen(uint32_t next_terminal_id) {

    // if the current terminal is the one is shown, do nothing
    if (next_terminal_id == current_looking_terminal)
    {
        return;
    }

    // calculate the position of next terminal video buffer
    int32_t NEXT_VID_MEM = VID_MEM_BUFFER1 + next_terminal_id * _4KB;
    // calculate the position of current terminal video buffer
    int32_t CUR_VID_MEM = VID_MEM_BUFFER1 + current_looking_terminal * _4KB;

    // store the current video memory to buffer and get the next terminal video memory from the buffer
    memcpy((void *)CUR_VID_MEM, (void *)VID_MEM_ADDR, _4KB);
    memcpy((void *)VID_MEM_ADDR, (void *)NEXT_VID_MEM, _4KB);

    // store the current keyboard buffer and get the next terminal keyboard buffer from the structure
    memcpy(terminals[current_looking_terminal].keyboard_buffer, get_keyboard_buffer(), KEYBOARD_BUFFER_SIZE);
    memcpy(get_keyboard_buffer(), terminals[next_terminal_id].keyboard_buffer, KEYBOARD_BUFFER_SIZE);
    
    // set the buffer pointer 
    terminals[current_looking_terminal].curr_buffer_ptr = get_buffer_ptr();
    set_buffer_ptr(terminals[next_terminal_id].curr_buffer_ptr);
    
    // save and update the x,y coordiates
    terminals[current_looking_terminal].x_pos = get_x();
    terminals[current_looking_terminal].y_pos = get_y();
    set_x(terminals[next_terminal_id].x_pos);
    set_y(terminals[next_terminal_id].y_pos);
    update_cursor(terminals[next_terminal_id].x_pos, terminals[next_terminal_id].y_pos);

    // update the current looking id
    current_looking_terminal = next_terminal_id;

}

/*
 * get_current_running_terminal
 * description: get the current running terminal id
 * input: none
 * output: the current running terminal id
 */
int32_t get_current_running_terminal() {
    return current_running_terminal;
}

/*
 * get_next_running_terminal
 * description: get the next running terminal id
 * input: none
 * output: the current running terminal id
 */
int32_t get_next_running_terminal() {
    // round robin, go to the next terminal by increment the current
    return (current_running_terminal + 1) % MAX_TERMINAL_NUM;
}

/*
 * set_next_running_terminal
 * description: set the next running terminal id to the given id
 * input: cur_running -- the next running terminal
 * output: none
 */
void set_next_running_terminal(uint32_t cur_running) {
    current_running_terminal = cur_running;
}

/*
 * get_next_looking_terminal
 * description: get the next looking terminal id
 * input: none
 * output: the current looking terminal id
 */
int32_t get_current_looking_terminal() {
    return current_looking_terminal;
}

