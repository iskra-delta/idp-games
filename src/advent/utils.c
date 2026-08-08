/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Various utils for Adventure
 *
 * Parts of this written by Tomaž Štih
 */

#include <stdio.h>
#include <string.h>
#include "vocab.h"
#include "utils.h"

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

[[sdcc::sfr(0xA0)]] UINT8 ctc_tenths_cs;
[[sdcc::sfr(0xA1)]] UINT8 ctc_hundreds;
[[sdcc::sfr(0xA2)]] UINT8 ctc_seconds;

#define CTC_TENTHS_CS ctc_tenths_cs
#define CTC_HUNDREDS  ctc_hundreds
#define CTC_SECONDS   ctc_seconds

static FILE *game_file;

void create_fn(char *name, char *fn) {
    UINT8 len = length(name) - 1;
    if (len > 8) { len = 8; }
    memcpy(fn, name, len);
    memcpy(fn + len, ".SAV", 5);
    to_upper(fn);
}

void game_file_open(char *path) {
    game_file_close();
    game_file = fopen(path, "rb");
}

UINT8 *game_file_read(UINT8 *buf, UINT16 pos, UINT16 len) {
    if (game_file == NULL || fseek(game_file, pos, SEEK_SET) != 0) {
        return NULL;
    }
    if (fread(buf, 1, len, game_file) != len) {
        return NULL;
    }
    return buf;
}

void game_file_close(void) {
    if (game_file != NULL) {
        fclose(game_file);
        game_file = NULL;
    }
}

BOOL game_file_write(char *path, UINT8 *buf, UINT16 len) {
    FILE *file = fopen(path, "wb");

    if (file == NULL) {
        return FALSE;
    }
    if (fwrite(buf, 1, len, file) != len) {
        fclose(file);
        return FALSE;
    }
    fclose(file);
    return TRUE;
}

void to_lower(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 'a' - 'A';
        }
    }
}

void to_upper(char *str) {
    for (; *str; str++) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 'a' - 'A';
        }
    }
}

void con_in(char *buffer) {
    printf("? ");
    char ch;
    UINT8 len = 0;
    do {
        // TODO: history, back/fwd arrow, insert, delete...
        while (!(ch = kbhit()));
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (len < 2 * MAXSTR + 1) {
                buffer[len] = ch;
                printf("%c", ch);
                buffer[++len] = '\0'; 
            } 
        } else if (ch == '\b') { 
            if (len > 0) {
                printf("\b \b");
                buffer[--len] = '\0';
            }
        } 
    } while (ch != '\r' || len == 0);
    printf("\n\r");
}

void parse_in(char *buffer, char *w1_buf, char *w2_buf, int w1_max_len, int w2_max_len) {
    // word 1
    char *p_buf = buffer, *w_start;
    for (; *p_buf == ' '; p_buf++);
    w_start = p_buf;
    UINT8 len = 0;
    for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
    if (len > w1_max_len) {
        memcpy(w1_buf, w_start, w1_max_len);
        w1_buf[w1_max_len] = '\0';
    } else {
        memcpy(w1_buf, w_start, len);
        w1_buf[len] = '\0';
    }
    to_lower(w1_buf);
    // word 2
    w2_buf[0] = '\0';
    if (w2_max_len > 0) {
        for (; *p_buf == ' '; p_buf++); 
        if (*p_buf != '\0') { // is there a word?
            w_start = p_buf;
            len = 0;
            for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
            if (len > w2_max_len) {
                memcpy(w2_buf, w_start, w2_max_len);
                w2_buf[w2_max_len] = '\0';
            } else {
                memcpy(w2_buf, w_start, len);
                w2_buf[len] = '\0';
            }
            to_lower(w2_buf);
        }
    }
}

UINT16 timer() {
    UINT8 s = CTC_SECONDS;
    UINT8 s100 = CTC_HUNDREDS;
    UINT8 cs10 = CTC_TENTHS_CS;
    UINT8 s100_check = CTC_HUNDREDS;
    UINT8 s_check = CTC_SECONDS;
    UINT16 ms;
    if (s100 == s100_check) {
        ms = BCD2BIN(s100) * 10 + BCD2BIN(cs10 >> 4);
    } else {
        ms = BCD2BIN(s100_check) * 10;
    }
    if (s == s_check) {
        return BCD2BIN(s) * 1000 + ms;
    } else {
        return BCD2BIN(s_check) * 1000;
    }
}
