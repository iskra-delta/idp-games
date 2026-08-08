/*
 * utils.h
 *
 * Miha Grčar 2022
 *
 * Various utils for Adventure
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <partner/conio.h>
#include "hdr.h"

void create_fn(char *name, char *fn);

void game_file_open(char *path);
UINT8 *game_file_read(UINT8 *buf, UINT16 pos, UINT16 len);
void game_file_close(void);

BOOL game_file_write(char *path, UINT8 *buf, UINT16 len);

void to_lower(char *str);
void to_upper(char *str);

void con_in(char *buffer);
void parse_in(char *buffer, char *w1_buf, char *w2_buf, int w1_max_len, int w2_max_len);

UINT16 timer();

#endif
