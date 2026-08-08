#ifndef __GDP_H__
#define __GDP_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	GDP_TOOL_PEN = 2,
	GDP_TOOL_ERASER = 0
} gdp_tool;

typedef enum {
	GDP_MODE_NORMAL = 0,
	GDP_MODE_XOR = 4
} gdp_mode;

typedef enum {
	GDP_STYLE_NORMAL = 0,
	GDP_STYLE_DOTTED = 1,
	GDP_STYLE_DASHED = 2,
	GDP_STYLE_DOTTED_DASHED = 3
} gdp_style;

typedef enum {
	GDP_DELTA_SIGN_DX_POS = 0,
	GDP_DELTA_SIGN_DX_NEG = 2,
	// NOTE: y axis is inverted
	GDP_DELTA_SIGN_DY_POS = 4, 
	GDP_DELTA_SIGN_DY_NEG = 0
} gdp_delta_sign;

#define GDP_STATUS_READY     0x04

#define GDP_CTRL_1_TOOL_UP   0x00
#define GDP_CTRL_1_TOOL_DOWN 0x01

#define GDP_CMD_CLS          0x04
#define GDP_CMD_LINE_DELTA   0x11
#define GDP_CMD_LINE_DX_POS  0x10
#define GDP_CMD_LINE_DX_NEG  0x16

[[sdcc::sfr(0x20)]] extern uint8_t gdp_cmd;    // W: command
[[sdcc::sfr(0x2F)]] extern uint8_t gdp_status; // R: status (ready)
[[sdcc::sfr(0x28)]] extern uint8_t gdp_x_hi;   // RW: x pos hi
[[sdcc::sfr(0x29)]] extern uint8_t gdp_x_lo;   // RW: x pos lo
[[sdcc::sfr(0x2A)]] extern uint8_t gdp_y_hi;   // RW: y pos hi
[[sdcc::sfr(0x2B)]] extern uint8_t gdp_y_lo;   // RW: y pos lo
[[sdcc::sfr(0x25)]] extern uint8_t gdp_dx;     // W: delta x
[[sdcc::sfr(0x27)]] extern uint8_t gdp_dy;     // W: delta y
[[sdcc::sfr(0x21)]] extern uint8_t gdp_ctrl_1; // RW: control register 1
[[sdcc::sfr(0x22)]] extern uint8_t gdp_ctrl_2; // RW: control register 2
[[sdcc::sfr(0x23)]] extern uint8_t gdp_char_size;
[[sdcc::sfr(0x36)]] extern uint8_t gdp_scroll; // W: graphic scroll
[[sdcc::sfr(0x30)]] extern uint8_t gfx_common; // RW: graphic common control

#define GDP_CMD       gdp_cmd
#define GDP_STATUS    gdp_status
#define GDP_X_HI      gdp_x_hi
#define GDP_X_LO      gdp_x_lo
#define GDP_Y_HI      gdp_y_hi
#define GDP_Y_LO      gdp_y_lo
#define GDP_DX        gdp_dx
#define GDP_DY        gdp_dy
#define GDP_CTRL_1    gdp_ctrl_1
#define GDP_CTRL_2    gdp_ctrl_2
#define GDP_CHAR_SIZE gdp_char_size
#define GDP_SCROLL    gdp_scroll
#define GFX_COMMON    gfx_common

extern uint8_t gdp_write_page;
extern uint8_t gdp_display_page;

void gdp_wait_ready();
void gdp_init();
void gdp_cls();

void gdp_set_xy(uint16_t x, uint8_t y);

void gdp_line_dx_pos(gdp_tool tool, gdp_style style, uint8_t dx);
void gdp_line_delta(gdp_tool tool, gdp_style style, uint8_t dx, uint8_t dy, gdp_delta_sign delta_sign);
void gdp_line_dx_pos_dotted(uint8_t dx);
void gdp_line_dx_pos_dotted_emu(uint8_t dx);

void gdp_write(gdp_tool tool, uint8_t *text);

void gdp_set_mode(gdp_mode mode);
void gdp_set_write_page(uint8_t page);
void gdp_set_display_page(uint8_t page);

void gdp_draw_row(uint8_t *image_row);
void gdp_draw_row_xor_mask(uint8_t *mask_row);
void gdp_draw_row_sprite(uint8_t *sprite_row);
void gdp_erase_row_sprite(uint8_t *sprite_row);
void gdp_draw(uint8_t **image, uint8_t last_row_idx, uint16_t x, uint8_t y);
void gdp_draw_xor_mask(uint8_t **mask, uint8_t last_row_idx, uint16_t x, uint8_t y);
void gdp_draw_sprite(uint8_t **sprite, uint8_t last_row_idx, uint16_t x, uint8_t y);
void gdp_erase_sprite(uint8_t **sprite, uint8_t last_row_idx, uint16_t x, uint8_t y);

void gdp_draw_rect(gdp_tool tool, uint16_t x, uint8_t y, uint8_t w, uint8_t h);
void gdp_fill_rect(gdp_tool tool, uint16_t x, uint8_t y, uint8_t w, uint8_t h);

void gdp_done();

#endif
