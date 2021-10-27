#ifndef PCB_H
#define PCB_H

#include "types.h"
#include "file_system.h"
#include "keyboard.h"
#include "do_sys.h"

#define PCB_MASK        0xFFFFE000  /* something */
/*0x800000 -> 0x796000 is left for 8 pcb to use */

//might need to change here

/*do -- when finding next pcb avaliable */
#define PCB_STACK_START 0x7E000
#define INUSE 1
#define UNUSE 0
#define USER_VID_MEM 0x084B8000

typedef int32_t (*func_ptr)();
/* (pcb_ptr ++) give address of next pcb */


typedef struct file_descriptor{
  /* information about file used by pcb */
  func_ptr * file_operations_table_ptr;
  struct inode_t * f_inode;
  uint32_t f_file_position;
  uint32_t f_flag;
} fd_t;

typedef struct pcb_struct {
	uint32_t pid;
  /* a single process can acquire maximum 8 files (include stdin/stdout) */
  uint32_t parent_pid;
	struct file_descriptor descriptors[8];
  //kernel ebp, esp and esp0
  int32_t             parent_ebp;           
  int32_t             parent_esp;              
  int32_t             parent_esp0;
} pcb_t ;

/* create 8kb structure use to traverse avaliable pcb in kernel space */
// typedef struct {
//   uint8_t skip[1000];
// }pcb_pointer_t;

//pcb_pointer_t * pcb_ptr  = (pcb_pointer_t *)PCB_STACK_START;

void pcb_init(pcb_t * pcb, int32_t next_pid);

pcb_t* get_pcb();

#endif
