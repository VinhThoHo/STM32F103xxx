#ifndef __HD44780_H
#define __HD44780_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/*----------------------------*/

#define HD44780_LIGHT_Pin GPIO_PIN_0
#define HD44780_LIGHT_GPIO_Port GPIOD

#define HD44780_E_Pin GPIO_PIN_1
#define HD44780_E_GPIO_Port GPIOD

#define HD44780_RW_Pin GPIO_PIN_2
#define HD44780_RW_GPIO_Port GPIOD

#define HD44780_RS_Pin GPIO_PIN_3
#define HD44780_RS_GPIO_Port GPIOD

/*----------------------------*/

#define HD44780_DB4_Pin GPIO_PIN_4
#define HD44780_DB4_GPIO_Port GPIOD

#define HD44780_DB5_Pin GPIO_PIN_5
#define HD44780_DB5_GPIO_Port GPIOD

#define HD44780_DB6_Pin GPIO_PIN_6
#define HD44780_DB6_GPIO_Port GPIOD

#define HD44780_DB7_Pin GPIO_PIN_7
#define HD44780_DB7_GPIO_Port GPIOD

/*----------------------------*/

void TM_HD44780_Init(uint8_t cols, uint8_t rows);
void TM_HD44780_DisplayOn(void);
void TM_HD44780_DisplayOff(void);
void TM_HD44780_Clear(void);
void TM_HD44780_Puts(uint8_t x, uint8_t y, char* str);
void TM_HD44780_BlinkOn(void);
void TM_HD44780_BlinkOff(void);
void TM_HD44780_CursorOn(void);
void TM_HD44780_CursorOff(void);
void TM_HD44780_ScrollLeft(void);
void TM_HD44780_ScrollRight(void);
void TM_HD44780_CreateChar(uint8_t location, uint8_t* data);
void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);

#ifdef __cplusplus
}
#endif

#endif