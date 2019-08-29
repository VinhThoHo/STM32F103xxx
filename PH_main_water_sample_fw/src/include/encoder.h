#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h>

// typedef struct
// {
// 	bool dir;
// 	uint32_t speeds;
// 	uint32_t turn;
// }ENCODER_STRUCT;

// extern ENCODER_STRUCT ENCX;
// extern ENCODER_STRUCT ENCY;

extern volatile uint16_t cntx;
extern volatile uint16_t cnty;
extern volatile uint16_t cntz;

void encoder_init(void);
// void encoder_reset(void);
// int32_t encoder_value(void);
// void encodersRead(void);
// void encodersDir(void);

#endif