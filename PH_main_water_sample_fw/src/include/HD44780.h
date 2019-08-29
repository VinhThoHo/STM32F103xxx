#ifndef HD44780_H
#define HD44780_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"

#define LCD_ROWS 4
#define LCD_COLS 20

#define HD44780_DB_PORT GPIOD
#define HD44780_DB4_PIN GPIO0
#define HD44780_DB5_PIN GPIO1
#define HD44780_DB6_PIN GPIO2
#define HD44780_DB7_PIN GPIO3

#define HD44780_CTRL_PORT GPIOD
#define HD44780_RS_PIN GPIO4
#define HD44780_RW_PIN GPIO5
#define HD44780_E_PIN GPIO6
#define HD44780_LIGHT_PIN GPIO7

#define HD44780_CLEARDISPLAY		0x01
#define HD44780_RETURNHOME			0x02
#define HD44780_ENTRYMODESET		0x04
#define HD44780_ENTRYRIGHT			0x00
#define HD44780_ENTRYLEFT			0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00


#define HD44780_DISPLAYCONTROL		0x08
#define HD44780_CURSORSHIFT			0x10
#define HD44780_FUNCTIONSET			0x20
#define HD44780_SETCGRAMADDR		0x40
#define HD44780_SETDDRAMADDR		0x80

// #define HD44780_ENTRYRIGHT			0x00
// #define HD44780_ENTRYLEFT			0x02
// #define HD44780_ENTRYSHIFTINCREMENT 0x01
// #define HD44780_ENTRYSHIFTDECREMENT 0x00

#define HD44780_DISPLAYON			0x04
#define HD44780_CURSORON			0x02
#define HD44780_BLINKON				0x01

#define HD44780_DISPLAYMOVE			0x08
#define HD44780_CURSORMOVE			0x00
#define HD44780_MOVERIGHT			0x04
#define HD44780_MOVELEFT			0x00

#define HD44780_8BITMODE			0x10
#define HD44780_4BITMODE			0x00
#define HD44780_2LINE				0x08
#define HD44780_1LINE				0x00
#define HD44780_5x10DOTS			0x04
#define HD44780_5x8DOTS				0x00

typedef struct
{
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
	bool light;
} HD44780_struct;

void HD44780_Init(uint8_t cols, uint8_t rows);
void HD44780_DisplayOn(void);
void HD44780_DisplayOff(void);
void HD44780_Clear(void);
void HD44780_Puts(uint8_t x, uint8_t y, char* str);

void HD44780_BlinkOn(void);
void HD44780_BlinkOff(void);
void HD44780_CursorOn(void);
void HD44780_CursorOff(void);
void HD44780_ScrollLeft(void);
void HD44780_ScrollRight(void);
void HD44780_CreateChar(uint8_t location, uint8_t* data);
void HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);
void HD44780_InitPins(void);
void HD44780_Cmd(uint8_t cmd);
void HD44780_Cmd4bit(uint8_t cmd);
void HD44780_Data(uint8_t data);
void HD44780_CursorSet(uint8_t col, uint8_t row);
void LCD_DisplayCGRAM0(uint8_t address, uint8_t *ptrTable);
void LCD_DisplayCGRAM1(uint8_t address, uint8_t *ptrTable);
bool get_lcd_light(void);
void LCD_LIGHT(bool en);
#endif /*HD44780*/
