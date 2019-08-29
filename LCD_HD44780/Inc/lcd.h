#ifndef __LCD_H
#define __LCD_H

#include "main.h"

#define LIGHT_SET() HAL_GPIO_WritePin(LCD_LIGHT_GPIO_Port, LCD_LIGHT_Pin, GPIO_PIN_SET);
#define RS_SET() HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
#define RW_SET() HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_SET);
#define EN_SET() HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET);
#define D4_SET() HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_SET);
#define D5_SET() HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_SET);
#define D6_SET() HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_SET);
#define D7_SET() HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_SET);

#define LIGHT_RESET() HAL_GPIO_WritePin(LCD_LIGHT_GPIO_Port, LCD_LIGHT_Pin, GPIO_PIN_RESET);
#define RS_RESET() HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
#define RW_RESET() HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
#define EN_RESET() HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
#define D4_RESET() HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_RESET);
#define D5_RESET() HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_RESET);
#define D6_RESET() HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_RESET);
#define D7_RESET() HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_RESET);

void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t dt);
void LCD_Char(char chr);
void LCD_String(char * str);
void LCD_SetPos(uint8_t x, uint8_t y);
void LCD_Clear(void);
void LCD_Init(void);

#endif