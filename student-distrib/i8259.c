/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* 
 * i8259_init
 * description:
 * Initialize the 8259 PIC 
 * input: none
 * output:none
 */
void i8259_init(void)
{
  //send ICW 1 - 4
  
  outb(ICW1, MASTER_COMMAND_PORT);
  outb(ICW1, SLAVE_COMMAND_PORT);

  outb(ICW2_MASTER, MASTER_DATA_PORT);
  outb(ICW2_SLAVE, SLAVE_DATA_PORT);

  outb(ICW3_MASTER, MASTER_DATA_PORT);
  outb(ICW3_SLAVE, SLAVE_DATA_PORT);

  outb(ICW4, MASTER_DATA_PORT);
  outb(ICW4, SLAVE_DATA_PORT);

  // mask all interrupts
  outb(MASTER_INITIAL_MASK, MASTER_DATA_PORT);
  outb(SLAVE_INITIAL_MASK, SLAVE_DATA_PORT);
  // record mask in global variable
  master_mask = MASTER_INITIAL_MASK;
  slave_mask = SLAVE_INITIAL_MASK;
}

/* 
 * enable_irq
 * description:
 * Enable (unmask) the specified IRQ
 * input: irq_num -- the IRQ number
 * output:none
 */
void enable_irq(uint32_t irq_num)
{
  if (irq_num >= TOTAL_IRQ_NUM)
  {
    // send slave
    slave_mask &= ~(SHIFT_1 << (irq_num & MASK_3_BIT));
    outb(slave_mask, SLAVE_DATA_PORT);
  }
  else
  {
    // send master
    master_mask &= ~(SHIFT_1 << irq_num);
    outb(master_mask, MASTER_DATA_PORT);
  }
}

/* 
 * disable_irq
 * description:
 * Disable (mask) the specified IRQ
 * input: irq_num -- the IRQ number
 * output:none
 */
void disable_irq(uint32_t irq_num)
{
  if (irq_num >= TOTAL_IRQ_NUM)
  {
    // send slave
    slave_mask |= (SHIFT_1 << (irq_num & MASK_3_BIT));
    outb(slave_mask, SLAVE_DATA_PORT);
  }
  else
  {
    // send master
    master_mask |= (SHIFT_1 << irq_num);
    outb(master_mask, MASTER_DATA_PORT);
  }
}

/* 
 * send_eoi
 * description:
 * Send end-of-interrupt signal for the specified IRQ
 * input: irq_num -- the IRQ number
 * output:none
 */
void send_eoi(uint32_t irq_num)
{
  if (irq_num >= TOTAL_IRQ_NUM)
  {
    irq_num -= TOTAL_IRQ_NUM;
    // send slave
    outb(irq_num | EOI, SLAVE_COMMAND_PORT);
    // tell master slave is done
    // TODO: only support one salve on IRQ 2: 0x20
    outb(SLAVE_PORT | EOI, MASTER_COMMAND_PORT);
  }
  else
  {
    // send master
    outb(irq_num | EOI, MASTER_COMMAND_PORT);
  }
}
