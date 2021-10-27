#ifndef rtc_handler_H
#define rtc_handler_H

#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "types.h"

#define RTC_COMMAND_PORT 0x70
#define RTC_DATA_PORT    0x71
#define RTC_REGISTER_A   0x0A
#define RTC_SET_NMI      0x80
#define RTC_REGISTER_B   0x0B
#define RTC_REGISTER_C   0x0C
#define RTC_ENABLE_PIE   0x40

#define IRQ_NUM_EIGHT    8

// OR'd when write to register: disable NMI
#define RTC_NMI          0x80

// parameters for cp2
#define RATE_MAX          1024
#define RATE_MIN          2
#define WRITE_REG_A_MASK  0xF0  // protect upper 4 bit of reg a while change rate
#define DEFAULT_RATE      0x0F  // this is a default rate 32768 >> (15-1) = 2
#define FIFTEEN           15 //2^15
#define RATE_OFFSET       1
#define RTC_BYTE_MAX      4


extern void rtc_init();
extern void rtc_handler();
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);
int32_t rtc_close(int32_t fd);
int32_t rtc_open(const uint8_t * filename);



#endif
