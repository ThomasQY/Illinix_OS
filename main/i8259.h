/* i8259.h - Defines used in interactions with the 8259 interrupt
 * controller
 * vim:ts=4 noexpandtab
 */

#ifndef _I8259_H
#define _I8259_H

#include "types.h"

/* Ports that each PIC sits on */
#define MASTER_COMMAND_PORT    0x20
#define MASTER_DATA_PORT    (MASTER_COMMAND_PORT+1)
#define SLAVE_COMMAND_PORT     0xA0
#define SLAVE_DATA_PORT     (SLAVE_COMMAND_PORT+1)

/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI                 0x60
// salve is connected to IRQ2, tell master this port has finished when we send EOI
#define SLAVE_PORT          0x02

// value for mask and unmask PICs
// master initial mask enable IRQ0 & 1 for keyboard
#define MASTER_INITIAL_MASK 0xF9
#define SLAVE_INITIAL_MASK  0xFF
// On each PIC IRQs are numbered  0 - 7 
#define TOTAL_IRQ_NUM        8
#define SHIFT_1              1
// 0x07 cause there are at most 7 IRQs on one PIC
#define MASK_3_BIT           0x07

/* Externally-visible functions */

/* Initialize both PICs */
void i8259_init(void);
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num);
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num);
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num);

#endif /* _I8259_H */
