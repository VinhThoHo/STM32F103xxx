#ifndef __LED_H
#define __LED_H

#include "main.h"

#define atsh_led1_pin GPIO_PIN_6
#define atsh_led1_gpio_port GPIOC
#define atsh_led2_pin GPIO_PIN_7
#define atsh_led2_gpio_port GPIOC

#define atsh_set_led1() HAL_GPIO_WritePin(atsh_led1_gpio_port, atsh_led1_pin, GPIO_PIN_SET);
#define atsh_set_led2() HAL_GPIO_WritePin(atsh_led2_gpio_port, atsh_led2_pin, GPIO_PIN_SET);
#define atsh_reset_led1() HAL_GPIO_WritePin(atsh_led1_gpio_port, atsh_led1_pin, GPIO_PIN_RESET);
#define atsh_reset_led2() HAL_GPIO_WritePin(atsh_led2_gpio_port, atsh_led2_pin, GPIO_PIN_RESET);

static void led_init(void);
#endif