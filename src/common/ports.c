#include <stdint.h>

// XCC uses the attributes on the extern declarations for port I/O codegen.
// Keep one definition of each absolute port symbol for the linker here.

[[sdcc::sfr(0x39)]] uint8_t avdc_cmd;
[[sdcc::sfr(0x39)]] uint8_t avdc_status;
[[sdcc::sfr(0x36)]] uint8_t avdc_access;
[[sdcc::sfr(0x38)]] uint8_t avdc_init_port;
[[sdcc::sfr(0x34)]] uint8_t avdc_chr;
[[sdcc::sfr(0x35)]] uint8_t avdc_attr;
[[sdcc::sfr(0x3C)]] uint8_t avdc_cur_lwr;
[[sdcc::sfr(0x3D)]] uint8_t avdc_cur_upr;
[[sdcc::sfr(0xAC)]] uint8_t avdc_mode_setting;
[[sdcc::sfr(0x32)]] uint8_t avdc_common_txt_attr;
[[sdcc::sfr(0x3E)]] uint8_t avdc_screen_start_2_lower;
[[sdcc::sfr(0x3F)]] uint8_t avdc_screen_start_2_upper;
[[sdcc::sfr(0x20)]] uint8_t avdc_gdp_status;

[[sdcc::sfr(0x20)]] uint8_t gdp_cmd;
[[sdcc::sfr(0x2F)]] uint8_t gdp_status;
[[sdcc::sfr(0x28)]] uint8_t gdp_x_hi;
[[sdcc::sfr(0x29)]] uint8_t gdp_x_lo;
[[sdcc::sfr(0x2A)]] uint8_t gdp_y_hi;
[[sdcc::sfr(0x2B)]] uint8_t gdp_y_lo;
[[sdcc::sfr(0x25)]] uint8_t gdp_dx;
[[sdcc::sfr(0x27)]] uint8_t gdp_dy;
[[sdcc::sfr(0x21)]] uint8_t gdp_ctrl_1;
[[sdcc::sfr(0x22)]] uint8_t gdp_ctrl_2;
[[sdcc::sfr(0x23)]] uint8_t gdp_char_size;
[[sdcc::sfr(0x36)]] uint8_t gdp_scroll;
[[sdcc::sfr(0x30)]] uint8_t gfx_common;

[[sdcc::sfr(0xA0)]] uint8_t ctc_tenths_cs;
[[sdcc::sfr(0xA1)]] uint8_t ctc_hundreds;
[[sdcc::sfr(0xA2)]] uint8_t ctc_seconds;
[[sdcc::sfr(0xD9)]] uint8_t kbd_status;
[[sdcc::sfr(0xD8)]] uint8_t kbd_cmd;
