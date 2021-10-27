#include "cursor.h"

/* 
 * enable_cursor
 * description: 
 * enable the cursor from row cursor_start to cursor_end
 * input: cursor_start, cursor_end, the start and end row of the cursor on the screen
 * output: none
 * side effect: enable the cursor
 */
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(CURSOR_ST, CURSOR_CMD);
	outb((inb(CURSOR_DATA) & BIT_SIX_MASK) | cursor_start, CURSOR_DATA);
	outb(CURSOR_ED, CURSOR_CMD);
	outb((inb(CURSOR_DATA) & BIT_FIVE_MASK) | cursor_end, CURSOR_DATA);
}

/* 
 * disable_cursor
 * description: 
 * disable the cursor
 * input: none
 * output: none
 * side effect: disable the cursor
 */
void disable_cursor()
{
	outb(CURSOR_ST, CURSOR_CMD);
	outb(SPACE, CURSOR_DATA);
}

/* 
 * update_cursor
 * description: 
 * update the cursor to index x and y of the screen
 * input: 
 * x: the row on the screen
 * y: the column of the screen
 * side effect: update the cursor
 */
void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
	outb(CURSOR_LOCATION_LOW, CURSOR_CMD);
	outb((uint8_t) (pos & BYTE_MASK), CURSOR_DATA);
	outb(CURSOR_LOCATION_HIGH, CURSOR_CMD);
	outb((uint8_t) ((pos >> BYTE) & BYTE_MASK), CURSOR_DATA);
}
