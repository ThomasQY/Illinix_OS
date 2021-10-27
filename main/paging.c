#include "types.h"
#include "paging.h"

// align the pte and pde
uint32_t pte[PTE_SIZE] __attribute__((aligned (PTE_ALIGN_SIZE)));
uint32_t pde[PDE_SIZE] __attribute__((aligned (PDE_ALIGN_SIZE)));

/*
 * init_paging
 * description:
 * initialize the paging by maping the video memory and kernel in pde and
 * pte.
 * inout: none
 * output: none
 * side effect: map the video and kernel memory in pde and pte
 */
void init_paging(){

  int i;          // loop counter

  // intialize pte and pde by setting them all to 0
  for (i = 0; i < PTE_SIZE; i++) {
    pde[i] = 0;
    pte[i] = 0;
  }

  //video memory address, present, r/w, user , address is found in lib.c
  pte[VID_MEM_INDEX] = VID_MEM_ADDR | RW_MASK;
  pte[VID_BUF1_INDEX] = VID_MEM_BUFFER1 | RW_MASK;
  pte[VID_BUF2_INDEX] = VID_MEM_BUFFER2 | RW_MASK;
  pte[VID_BUF3_INDEX] = VID_MEM_BUFFER3 | RW_MASK;


  /*
   * for the video memory, pte already has last three bytes 0 because of aligned
   * user, r/w, present, 4KB
   */
  pde[PDE_VID_INDEX] = (uint32_t)pte | RW_MASK;

  //for the kernel space, 4MB, supervisor, r/w, present
  pde[PDE_KER_INDEX] = KERNEL_MEM_ADDR | KERNEL_MEM_INDEX;   // should be 0x00400083;

  /*todo: init pde: setup ped for process use
  * approach: set up 8 pde and assign to processes (static pde start address)
  * approach: always map pde to 128 memory map
  */
  //set, cr3, cr4, cr0
  asm volatile("            \n\
    pushl %%eax             \n\
    movl $pde, %%eax        \n\
    movl %%eax, %%cr3       \n\
    movl %%cr4, %%eax       \n\
    orl $0x00000010, %%eax  \n\
    movl %%eax, %%cr4       \n\
    movl %%cr0, %%eax       \n\
    orl $0x80000000, %%eax  \n\
    movl %%eax, %%cr0       \n\
    popl %%eax              "
    :          //no output
    : "r"(pde) //input pde
    : "memory", "cc", "eax"    //clobbered register
  );
}

/*
 * reset_paging
 *   DESCRIPTION: open new page for process
 *   INPUTS: uint32_t physical_mem_ -- physical memory to reset
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: clear the TLB and reset the physical memory
 */
void reset_paging(uint32_t physical_mem_){
  pde[PDE_POS] = (physical_mem_ & PHYS_MASK) | SET_PAGE_087;
  flush_tlb();
}

/*
 * reset_video_page
 *   DESCRIPTION: open new page for video memory 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: retset the video memory and flush the TLB
 */
void reset_video_page(uint32_t addr){
  // shift 22 bits to get the correct pde entry index
  pde[USER_VID_MEM >> 22] = (uint32_t)pte | URW_MASK;

  //change to current index and flush TLB
  pte[VID_MEM_INDEX] = addr | URW_MASK;
  flush_tlb();

  return;
}

/*
 * flush_tlb
 *   DESCRIPTION: flush the tlb when switch process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void flush_tlb(void){
    asm volatile("                   \n\
        movl %%cr3, %%eax            \n\
        movl %%eax, %%cr3            \n\
        "
        :
        :
        : "eax"
    );
}


/*
 * set_pte
 *   DESCRIPTION: reset the pte mapping
 *   INPUTS: index -- the pte index
 *           addr -- the address to be mapped
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void set_pte(uint32_t index, uint32_t addr){
   pte[index] = addr | RW_MASK;
   flush_tlb();
}

/*
 * get_old_pte
 *   DESCRIPTION: get the current video memory
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the current video memory
 *   SIDE EFFECTS: none
 */
uint32_t get_old_pte(void) {
  return pte[VID_MEM_INDEX];
}

/*
 * delete_user_video_page
 *   DESCRIPTION: delete user video page
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the current video memory
 *   SIDE EFFECTS: flush TLB
 */
void delete_user_video_page(){
  // shift 22 bits to get the correct pde entry index, set the pde not exit (0)
  pde[USER_VID_MEM >> 22] = 0;
  flush_tlb();
}


