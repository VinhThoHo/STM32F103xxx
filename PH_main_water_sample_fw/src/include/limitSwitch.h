#ifndef __LIMITSWITCH_H
#define __LIMITSWITCH_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"

#define NUM_SWITCHS 4

#define SW_LIMIT_PORT GPIOE
#define X_AXIS_MIN_PIN GPIO2
#define Y_AXIS_MIN_PIN GPIO3
#define X_AXIS_MAX_PIN GPIO4
#define Y_AXIS_MAX_PIN GPIO5

enum  { LIMIT_SWITCH_X_MIN, LIMIT_SWITCH_Y_MIN };
extern bool bEventSend[NUM_SWITCHS];

void limit_switch_config(void);
bool limit_switch_pressed(uint8_t sw);
void limit_switch_task(void);

#endif