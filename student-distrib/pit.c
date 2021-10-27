#include "pit.h"

//would only use 0th port
//set pit to mode 3 with 100 HZ
static int count = 0;

/*
 * pit_init
 * description:
 * initilize the pit by sending commands to certain ports
 * input: none
 * output: none
 */
void pit_init()
{
    //mode 3
    outb(MODE_3, PIT_COMMAND);
    //high_byte, use 0xFF to mask out the low bytes
    outb(_100HZ & 0xFF, CHANNEL_0);
    //low byte
    outb(_100HZ >> 8, CHANNEL_0);
    //pit the 0th interrupt
    enable_irq(0);
}

/*
 * pit_handler
 * description:
 * when PIT interrupt is received, do the scheduling:
 * - save esp/ebp
 * - switch process paging
 * - set TSS
 * - update the x, y coordinates
 * - restore esp/ebp
 * input: none
 * output: none
 */
void pit_handler()
{
    int32_t vid_buffer_addr;

    //get current and next running terminal
    int32_t current = get_current_running_terminal();
    int32_t current_looking = get_current_looking_terminal();
    
    //calculate the next terminal id
    int32_t next = current + 1;
    next = next % MAX_TERM;
    
    // update the x, y coordinates
    set_x(terminals[next].x_pos);
    set_y(terminals[next].y_pos);
    
    // save the current pid
    terminals[current].current_pid =  get_pcb()->pid;

    //save esp and ebp
    asm volatile(
        "movl %%esp,%0 \n"
        "movl %%ebp,%1 \n"
        : "=g"(terminals[current].saved_esp), "=g"(terminals[current].saved_ebp)
        :
        : "memory");
        
    //store esp0;
    terminals[get_current_running_terminal()].esp0 = tss.esp0;

    // map the pte to the correct address
    if (current_looking != next)
    {
        vid_buffer_addr = VID_MEM_BUFFER1 + next * _4KB;
        set_pte(VID_MEM_INDEX, vid_buffer_addr);
        flush_tlb();
    }
    else
    {
        set_pte(VID_MEM_INDEX, VID_MEM_ADDR);
        flush_tlb();
    }

    // update the next running terminal
    set_next_running_terminal(next);

    //switch_stack, see if its is the first three shell, and launch shell if needed
    if (count < 3)
    {
        // switch stack
        asm volatile(
            "movl %0, %%esp \n"
            "movl %1, %%ebp \n"
            :
            : "g"(_8MB - ADDR_OFFSET - count * _8KB), "g"(_8MB - ADDR_OFFSET - count * _8KB));
        count++;
        // update tss except for the first shell
        if (count != 0)
        {
            // -1 since current running terminal is already updated
            tss.esp0 = terminals[get_current_running_terminal() - 1].esp0; 
        }
        send_eoi(IRQ_ZERO);     // send EOI
        execute((uint8_t *)"shell"); //never comes back
    }

    // reset paging
    reset_paging(_8MB + _4MB * terminals[get_current_running_terminal()].current_pid);

    // check is reset video paging is needed
    if(terminals[get_current_running_terminal()].user_vid_mem == HIGH){
        if(get_current_running_terminal() == get_current_looking_terminal()){
            reset_video_page(VID_MEM_ADDR);
        }else{
            reset_video_page(VID_MEM_BUFFER1 + _4KB * get_current_running_terminal());
        }
    }

    // reload tss.esp0
    tss.esp0 = terminals[get_current_running_terminal()].esp0;

    // send EOI to interrupt 0
    send_eoi(IRQ_ZERO);

    // stack switch, change esp, ebp
    asm volatile(
        "movl %0,%%esp \n"
        "movl %1,%%ebp \n"
        :
        : "g"(terminals[next].saved_esp), "g"(terminals[next].saved_ebp));
}
