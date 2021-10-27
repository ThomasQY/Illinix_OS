#include "rtc_handler.h"

volatile int intr_rtc = 0;              // flag: 1 if an rtc interrupt occured

/*
 * rtc_init
 * description:
 * Initialize the RTC by sending coressponding commands
 * to the ports
 * input: none
 * output: none
 */
extern void rtc_init(){
  char reg_b;                 // RTC register B
  char reg_a;
  // set rtc registers
  cli();
  outb(RTC_REGISTER_B|RTC_NMI,RTC_COMMAND_PORT);
  reg_b = inb(RTC_DATA_PORT);

  // printf("%x\n", reg_b);
  // enable rtc periodic interrupt
  outb(RTC_REGISTER_B|RTC_NMI,RTC_COMMAND_PORT);
  outb(reg_b | RTC_ENABLE_PIE,RTC_DATA_PORT);

  // set default interrupt rate to 2HZ
  outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
  reg_a = inb(RTC_DATA_PORT);
  outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
  outb((reg_a & WRITE_REG_A_MASK)|DEFAULT_RATE,RTC_DATA_PORT);

  // enable irq_8
  enable_irq(IRQ_NUM_EIGHT);
  sti();
}

/*
 * rtc_change-rate
 * description:
 * change rate of rtc as specified by parameter
 * input: new rtc rate desiered
 * output: 0 if successful -1 otherwise
 */
int32_t
rtc_write(int32_t fd, const void* buf, int32_t nbytes){
  char reg_a;
  int8_t new_rate = 0;
  int32_t rate = *((int32_t *)buf);
 // sanity check
 if(  (nbytes > RTC_BYTE_MAX)
    | (rate > RATE_MAX)
    | (rate < RATE_MIN)
    | (rate & (rate - 1))  // should be 0 if rate is power of 2
    ) return -1;
 // compute new rate
  while(rate >>= 1) new_rate += 1;  // now  new_rate  log(2)(rate)
  new_rate = FIFTEEN - new_rate + RATE_OFFSET;  // compute dividend
 // change rtc rate
  cli();
    outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
    reg_a = inb(RTC_DATA_PORT);
    outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
    outb((reg_a & WRITE_REG_A_MASK)|new_rate,RTC_DATA_PORT);
  sti();
  return 0;
}


/*
 * rtc_handler
 * description:
 * handle the RTC by sending corresponidng command to the port
 * input: none
 * output: none
 */
extern void rtc_handler(int32_t fd, const void* buf, int32_t nbytes){
  outb(RTC_REGISTER_C,RTC_COMMAND_PORT); // select register C
  inb(RTC_DATA_PORT);                    // throw away whatever we just read
  // test_interrupts();                  // test whether it works
  intr_rtc = 1;                          // flag an interrupt has occured
  send_eoi(IRQ_NUM_EIGHT);               // send EOI after handler finishs
}

/*
 * rtc_read
 * description:
 * wait until an rtc interrupt occur and return 0
 * input: none
 * output: 0 when rtc interrupt occur
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
  intr_rtc = 0;                           // wait for rtc interrupt
  // sti();
  while(!intr_rtc);
  // cli();
  return 0;
}

/*
 * rtc_close
 * description:
 * necessary when we treat rtc as a file, just return
 * input: none
 * output: always 0
 */
 int32_t rtc_close(int32_t fd){
   return 0;
 }

 /*
  * rtc_open
  * description:
  * set rtc frequency back to default 2 HZ
  * input: none
  * output: always 0
  */
 int32_t rtc_open(const uint8_t * filename){
   char reg_a;
   outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
   reg_a = inb(RTC_DATA_PORT);
   outb(RTC_REGISTER_A|RTC_NMI,RTC_COMMAND_PORT);
   outb((reg_a & WRITE_REG_A_MASK)|DEFAULT_RATE,RTC_DATA_PORT);
   return 0;
 }
