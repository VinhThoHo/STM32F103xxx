#ifndef LCD_BUF_H_
#define LCD_BUF_H_

#include "HD44780.h"
extern char lcd_buf[LCD_ROWS][LCD_COLS];

void buf_locate(uint8_t x, uint8_t y);
void buf_char(char c);
void buf_clear(void);
void lcd_refresh(void);

void buf_str(char *text);
void buf_str_XY(uint8_t x, uint8_t y, char * text);
void buf_int(uint8_t value);

void buf_clear_menu(void);

#endif /* LCD_BUF_H_ */