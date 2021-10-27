#ifndef DO_SYS_H
#define DO_SYS_H

#include "types.h"
#include "file_system.h"
#include "pcb.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"

//check for those numbers
#define PCB_BASE 0x7F0000 + _8KB * 2

#define PCB_SIZE 8192

#define USER_BASE 0x00800000

#define USER_SIZE       0x00400000

#define _2MB            0x00200000
#define _8MB            0x00800000
#define _4KB            0x00001000
#define _4MB            0x00400000
#define _8KB            0x00002000
#define _128MB          0x08000000
#define FUNCTION_PTR_SIZE 4
#define SET_ZERO        0
#define MAX_PCB         5
#define MAX_FILE        8
#define SKIP_INOUT      2
#define SB_MASK         0x000F
#define NAME_BUFFER     128
#define READ_DATA_OFFSET 24
#define PCB_OFFSET       4

#define PROGRAM_OFFSET  0x00048000

#define MAX_PROCESS_NUM 6 
#define PROCESS_ON      1
#define PROCESS_OFF     0

#define PROCESS_ZERO    0
#define PROCESS_ONE     1
#define PROCESS_TWO     2

#define HIGH            1
#define LOW             0




extern int32_t terminate_by_exception();

extern int32_t execute(const uint8_t * command);

extern int32_t halt(uint8_t status);

extern int32_t read(int32_t fd, void * buf, int32_t nbytes);

extern int32_t write(int32_t fd, const void * buf, int32_t nbytes);

extern int32_t open(const uint8_t * filename);

extern int32_t close(int32_t fd);

extern int32_t getargs(int8_t * buf, int32_t nbytes);

extern int32_t vidmap(uint8_t ** screen_start);

extern int32_t set_handler(int32_t signum, void * handler_address);

extern int32_t sig_return(void);

extern int32_t get_next_pid();

#endif
