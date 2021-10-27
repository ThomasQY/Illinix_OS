#ifndef PAGING_H
#define PAGING_H

#include "schedule.h"

#define PTE_SIZE              1024
#define PDE_SIZE              1024
#define PTE_ALIGN_SIZE        4096
#define PDE_ALIGN_SIZE        4096

#define _4KB                  0x00001000

#define VID_MEM_ADDR          0xB8000
#define VID_MEM_BUFFER1       (_4KB + VID_MEM_ADDR)
#define VID_MEM_BUFFER2       (_4KB + VID_MEM_BUFFER1)
#define VID_MEM_BUFFER3       (_4KB + VID_MEM_BUFFER2)
// #define VID_MEM_KEYBOARD      (_4KB + VID_MEM_BUFFER3)
#define VID_MEM_INDEX         0xB8
#define VID_BUF1_INDEX        0xB9
#define VID_BUF2_INDEX        0xBA
#define VID_BUF3_INDEX        0xBB
// #define VID_KEYBOARD_INDEX    0xBC
#define KERNEL_MEM_ADDR       0x400000
#define KERNEL_MEM_INDEX      0x83

#define PDE_VID_INDEX         0
#define PDE_KER_INDEX         1

#define RW_MASK               0x03
#define URW_MASK              0x07
#define PDE_POS               32
#define PHYS_MASK             0XFFFFF000
#define SET_PAGE_087          0x087
#define USER_VID_MEM          0x084B8000

#define ADDR_OFFSET           4
#define IRQ_ZERO              0


// define the initialization function
void init_paging();

void reset_paging(uint32_t physical_mem_ );

void reset_video_page(uint32_t addr);

void flush_tlb(void);

void set_pte(uint32_t index, uint32_t addr);

uint32_t get_old_pte(void);

void delete_user_video_page();

#endif
