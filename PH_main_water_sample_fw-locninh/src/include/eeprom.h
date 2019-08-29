#ifndef	_EEPROM_H__
#define _EEPROM_H__

#include <stdbool.h>
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>
#include <tick.h>
#include "clock_rtc.h"

typedef struct
{
	uint16_t devAddr;
} EEPROM_STRUCT;

typedef struct
{
  uint8_t bottle;
  DATE_TIME logtime;
  uint8_t buff[8];
} EEPROM_STORAGE;

extern EEPROM_STORAGE store[25];
extern EEPROM_STRUCT eeprom;


void eeprom_init(void);
void eeprom_write_page(uint16_t memAddr, uint8_t* buf, uint16_t length);
void eeprom_read_page(uint16_t memAddr, uint8_t* buf, uint16_t length);
void i2c_mem_write(uint8_t devAdd, uint16_t memAddr, uint8_t* buf, uint16_t length);
void i2c_mem_read(uint8_t devAdd, uint16_t memAddr, uint8_t* buf, uint16_t length);

void log_save(uint8_t bot);
void log_load(uint8_t bot, uint8_t buff[8]);
void log_clear(void);
#endif