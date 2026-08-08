#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <partner/bcd.h>
#include <partner/conio.h>

#define LO(x) ((uint8_t *)&x)[0]
#define HI(x) ((uint8_t *)&x)[1]

// timer

[[sdcc::sfr(0xA0)]] extern uint8_t ctc_tenths_cs; // R: clock, tenths of a centisecond
[[sdcc::sfr(0xA1)]] extern uint8_t ctc_hundreds;  // R: clock, hundreds of a second
[[sdcc::sfr(0xA2)]] extern uint8_t ctc_seconds;   // R: clock, seconds

#define CTC_TENTHS_CS ctc_tenths_cs
#define CTC_HUNDREDS  ctc_hundreds
#define CTC_SECONDS   ctc_seconds

extern int16_t timer_start;
extern int16_t timer_offset;

int16_t timer();
uint16_t timer_ms();

void timer_reset(int16_t offset);
int16_t timer_diff();
int16_t timer_diff_ex(uint16_t timer_start, uint16_t timer_offset);

// keyboard

#define KBD_STATUS_READY  0x04
#define KBD_CMD_BEEP      0x02
#define KBD_CMD_BEEP_LONG 0x04

[[sdcc::sfr(0xD9)]] extern uint8_t kbd_status; // R: status (ready)
[[sdcc::sfr(0xD8)]] extern uint8_t kbd_cmd;    // W: command

#define KBD_STATUS kbd_status
#define KBD_CMD    kbd_cmd

void kbd_wait_ready();
void kbd_beep(bool long_beep);
char kbd_get_key();

// other

bool sys_is_emu();
int sys_rand();
char *itoa(long value, char *buffer, int base);
void msleep(int millisec);

#endif
