#ifndef _DS18B20_H_
#define _DS18B20_H_

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include "timerTick.h"

#define DS18B20_CONVERT_T_CMD               0x44
#define DS18B20_WRITE_STRATCHPAD_CMD        0x4E
#define DS18B20_READ_STRATCHPAD_CMD         0xBE
#define DS18B20_COPY_STRATCHPAD_CMD         0x48
#define DS18B20_RECALL_E_CMD                0xB8
#define DS18B20_SEARCH_ROM_CMD              0xF0
#define DS18B20_READ_POWER_SUPPLY_CMD       0xB4
#define DS18B20_SKIPROM_CMD                 0xCC
#define DS18B20_MATCHROM_CMD                0x55

#define MAX_SENSORS                         10

#define DS_OK                               0
#define DS_ERR                              1

typedef enum
{
    DS_INIT,
    DS_SEARCH,
    DS_WAIT,
    DS_READ
}DS_STATE;

typedef struct
{
    uint32_t port;
    uint16_t pin;
    float temp;
    uint8_t id[8];
    DS_STATE status;
	uint8_t state;
    TIME time;
}DS18B20_STRUCT;

uint8_t DS18B20_CRC(uint8_t *data, uint16_t number_of_bytes_in_data);
void ds_input(DS18B20_STRUCT* ds);
void ds_output(DS18B20_STRUCT* ds);
void ds_low(DS18B20_STRUCT* ds);
void ds_high(DS18B20_STRUCT* ds);
uint16_t OneWire_GetBusLine(DS18B20_STRUCT* ds);
uint8_t OneWire_ReadBit(DS18B20_STRUCT* ds);
void OneWire_WriteBit(DS18B20_STRUCT* ds, uint8_t bit);
void OneWire_WriteByte(DS18B20_STRUCT* ds, uint8_t data);
uint8_t OneWire_ReadByte(DS18B20_STRUCT* ds);
uint8_t OneWire_Reset(DS18B20_STRUCT* ds);
uint8_t OneWire_SearchROM(DS18B20_STRUCT* ds, uint8_t diff, uint8_t *id);
void OneWire_FindROM(DS18B20_STRUCT* ds, uint8_t *diff, uint8_t id[]);

void DS18B20_Init(DS18B20_STRUCT* ds, uint32_t GPIOx, uint16_t GPIO_Pin);
void DS18B20_StartConversion(DS18B20_STRUCT* ds, uint8_t SensorNum);
void DS18B20_GetROM(DS18B20_STRUCT* ds, uint8_t SensorNum, uint8_t *buffer);
uint8_t DS18B20_ScanBus(DS18B20_STRUCT* ds);
float DS18B20_GetTemp(DS18B20_STRUCT* ds, uint8_t SensorNum);
uint8_t DS18B20_GetState(DS18B20_STRUCT* ds);
#endif