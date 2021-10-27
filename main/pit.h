#ifndef _PIT_H
#define _PIT_H

#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "types.h"
#include "pcb.h"
#include "do_sys.h"
#include "schedule.h"

#define MODE_3          0x36
#define PIT_COMMAND     0x43
#define _100            100
#define PIT_FREQ        1193180
#define _100HZ           PIT_FREQ / _100
#define CHANNEL_0       0x40
#define MAX_TERM        3

void pit_init();

void pit_handler();


#endif
