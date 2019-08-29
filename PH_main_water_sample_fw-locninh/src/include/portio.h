#ifndef __PORTIO_H__
#define __PORTIO_H__
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "debug.h"
#include "timerTick.h"

#define CTRL_PORT GPIOC
#define CTRL_PIN_1 GPIO0
#define CTRL_PIN_2 GPIO1
#define CTRL_PIN_3 GPIO2
#define CTRL_PIN_4 GPIO3
#define CTRL_PIN_5 GPIO4
#define CTRL_PIN_6 GPIO5
#define CTRL_PIN_7 GPIO6
#define CTRL_PIN_8 GPIO7
#define BUZZER_PORT GPIOD
#define BUZZER_PIN GPIO15

#define O_CTRL1(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_1):gpio_clear(CTRL_PORT, CTRL_PIN_1)
#define O_CTRL2(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_2):gpio_clear(CTRL_PORT, CTRL_PIN_2)
#define O_CTRL3(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_3):gpio_clear(CTRL_PORT, CTRL_PIN_3)
#define O_CTRL4(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_4):gpio_clear(CTRL_PORT, CTRL_PIN_4)
#define O_CTRL5(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_5):gpio_clear(CTRL_PORT, CTRL_PIN_5)
#define O_CTRL6(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_6):gpio_clear(CTRL_PORT, CTRL_PIN_6)
#define O_CTRL7(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_7):gpio_clear(CTRL_PORT, CTRL_PIN_7)
#define O_CTRL8(bit) bit?gpio_set(CTRL_PORT, CTRL_PIN_8):gpio_clear(CTRL_PORT, CTRL_PIN_8)

#define BUZZER(bit) bit?gpio_set(BUZZER_PORT, BUZZER_PIN):gpio_clear(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_TOGGLE() gpio_toggle(BUZZER_PORT, BUZZER_PIN)

typedef struct
{
	uint8_t change;
	TIME tick_buzzer;
}PORT_CONTROL_STRUCT;

extern PORT_CONTROL_STRUCT port;

void port_ctrl_init(void);
void port_ctrl_manager(void);

#endif /*__PORTIO_H__*/