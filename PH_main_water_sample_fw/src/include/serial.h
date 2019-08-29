#ifndef __SERIAL_H_
#define __SERIAL_H_

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "ringbuf.h"
#include "timerTick.h"
#include "tick.h"

typedef struct{
	uint8_t data[128];
	uint8_t event;
	uint8_t chk;
	uint8_t id;
} SERIAL_STRUCT;

void serial_init(void);
void serial_tx(uint8_t *source, uint16_t len);
void serial_StringToHex(uint8_t *source, uint16_t len, uint8_t *dest);
void serial_parser(uint8_t *data, uint16_t len);
void serial_manager(void);

#endif /*__SERIAL_H__*/