#ifndef __INPUT_H
#define __INPUT_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"

#define IN_PORT GPIOE
#define IN_PIN1 GPIO0
#define IN_PIN2 GPIO1
// #define IN_PIN3 GPIO2
// #define IN_PIN4 GPIO3
// #define IN_PIN5 GPIO4
// #define IN_PIN6 GPIO5

#define INPUT1() GPIO_IDR(IN_PORT)&IN_PIN1
#define INPUT2() GPIO_IDR(IN_PORT)&IN_PIN2
// #define INPUT3() GPIO_IDR(IN_PORT)&IN_PIN3
// #define INPUT4() GPIO_IDR(IN_PORT)&IN_PIN4
// #define INPUT5() GPIO_IDR(IN_PORT)&IN_PIN5
// #define INPUT6() GPIO_IDR(IN_PORT)&IN_PIN6

#define NUM_OF_SAMPLE_INPUT 15
#define NUM_INPUT 2

typedef enum {
	IN1 = 0,
	IN2,
	// IN3,
	// IN4,
	// IN5,
	// IN6
}INPUT_NAME;

typedef enum {
	IN_RELEASE = 0,
	IN_ACTIVE = 1
}INPUT_EVENT;

typedef struct {
	uint8_t actived;
	uint8_t active;
	uint8_t waitRelease;
	uint8_t cnt;
}INPUT_EXT;

typedef struct {
	uint8_t name;
	INPUT_EXT ext[NUM_INPUT];
	uint32_t tick;
}INPUT_STRUCT;

typedef enum {
	NO_INPUT,
	IN1_ON,
	IN1_OFF,
	IN2_ON,
	IN2_OFF,
	// IN3_ON,
	// IN3_OFF,
	// IN4_ON,
	// IN4_OFF,
	// IN5_ON,
	// IN5_OFF,
	// IN6_ON,
	// IN6_OFF
}INPUT_ID;

void input_init(void);
void input_get(uint8_t port_id);
uint8_t input_process(void);
void input_manage(void);

#endif /* __INPUT_H */