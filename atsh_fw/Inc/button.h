#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "main.h"

#define key1         HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10)
#define key2        HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)
#define key3        HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)
#define key4         HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)

void button_init(void);
uint8_t get_button(void);
void button_test(void);
uint8_t button_counter(uint8_t c);
#endif