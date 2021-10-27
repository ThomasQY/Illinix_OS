#ifndef cursor_H
#define cursor_H

#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "types.h"

/* reference: https://wiki.osdev.org/Text_Mode_Cursor
            : http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
*/
#define CURSOR_ST               0x0A
#define CURSOR_ED               0x0B
#define CURSOR_LOCATION_HIGH    0x0E
#define CURSOR_LOCATION_LOW     0x0F
#define VGA_WIDTH               80
#define BYTE                    8
#define BYTE_MASK               0xFF
#define CURSOR_CMD              0x3D4
#define CURSOR_DATA             0x3D5
#define SPACE                   0x20
#define BIT_FIVE_MASK           0xE0
#define BIT_SIX_MASK            0xC0

extern void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
extern void disable_cursor();
extern void update_cursor(int x, int y);

#endif
