#include "pcb.h"

/*
 * err_func
 *   DESCRIPTION: Error function to return -1
 *   INPUTS: int32_t fd -- which file to open
 *           uint8_t *buf -- the buffer to be written
 *           int32_t length -- how long reading the file
 *   OUTPUTS: none
 *   RETURN VALUE: -1 for failure
 *   SIDE EFFECTS: none
 */
int32_t err_func(int32_t fd, uint8_t *buf, int32_t length)
{
  return -1;
}

/* function pointers of stdin and stdout */
func_ptr stdin_funcs[4] = {terminal_read, err_func, terminal_open, terminal_close};
func_ptr stdout_funcs[4] = {err_func, terminal_write, terminal_open, terminal_close};

/*
 * get_pcb
 *   DESCRIPTION: return the current pcb
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: pcb pointer of current pcb
 *   SIDE EFFECTS: none
 */
pcb_t *get_pcb()
{
  uint32_t _addr;
  pcb_t *ret;
  asm volatile(
      "movl %%esp, %0"
      : "=m"(_addr)
      :
      : "memory");
  _addr &= PCB_MASK;
  ret = (pcb_t *)_addr;
  return ret;
}

/*
* pcb_init
* description: setup pcb structure for process
* input:  pcb_t *pcb -- pcb to be initialized
*          int32_t next_pid -- setup its nextpid
* output:
* side effect: pcb setup for process called pcb_init
*/
void pcb_init(pcb_t *pcb, int32_t next_pid)
{
  int i;
  pcb->pid = next_pid;
  if(next_pid == 0) {
    pcb->parent_pid = -1;
  } else {
    pcb_t * parent = get_pcb();
    pcb->parent_pid = parent->pid;
  }
  //set descriptor[0], [1] to stdin stdout
  pcb->descriptors[0].f_flag = INUSE;
  pcb->descriptors[0].file_operations_table_ptr = stdin_funcs;
  pcb->descriptors[1].f_flag = INUSE;
  pcb->descriptors[1].file_operations_table_ptr = stdout_funcs;
  for (i = 2; i < 8; i++)
  {
    pcb->descriptors[i].f_flag = UNUSE;
  }
}
