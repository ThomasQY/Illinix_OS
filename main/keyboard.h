#ifndef keyboard_H
#define keyboard_H

#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "types.h"
#include "tests.h"
#include "schedule.h"

#define KEYBOARD_PORT          0x60
#define KEYBOARD_MAP_TABLE     0x80

#define LEFT_SHIFT_PRESS       0x2A     
#define RIGHT_SHIFT_PRESS      0x36
#define LEFT_SHIFT_RELEASE     0xAA
#define RIGHT_SHIFT_RELEASE    0xB6
#define CTRL_PRESS             0x1D    
#define CTRL_RELEASE           0x9D
#define L_PRESS                0x26
#define CAPS_PRESS             0x3A
#define CAPS_RELEASE           0xBA
#define ZERO                   0
#define IRQ_NUM_ONE            1
#define BACK_SPACE             0x0E
#define KEY_BUFFER_SIZE        128
#define ENTER                  0x1C
#define TAB                    0x0F
#define ONE                    1

#define Q_PRESSED              0x10
#define P_PRESSED              0x19
#define A_PRESSED              0x1E
#define L_PRESSED              0x26
#define Z_PRESSED              0x2C
#define M_PRESSED              0x32

#define F1_PRESS               0x3B
#define F2_PRESS               0x3C
#define F3_PRESS               0x3D
#define ALT_PRESS              0x38
#define ALT_RELEASE            0xB8

#define VID_INDEX              0xB8
#define VID_ADDR               0xB8000

#define TERM_ZERO              0
#define TERM_ONE               1
#define TERM_TWO               2

#define HIGH                   1
#define LOW                    0



extern int32_t terminal_open(const uint8_t* filename);

extern int32_t terminal_close(int32_t fd);

extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);

extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);

int8_t keyboard_buffer[KEY_BUFFER_SIZE];

//To test for the read wnad write function
int8_t keyboard_buffer_2[KEY_BUFFER_SIZE];

// handler for keyboard input
extern void keyboard_input();

//local test for read and write
void read_write_test();

int8_t * get_keyboard_buffer();

int is_letter(int key);

void set_buffer_ptr(int new_ptr);

int get_buffer_ptr();

#endif
