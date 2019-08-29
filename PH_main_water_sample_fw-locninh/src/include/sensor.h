#ifndef __ANALOG_H__
#define __ANALOG_H__
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"
#include "debug.h"
#include "timerTick.h"

#define ADC_CHANNEL 8
#define ADC_PORTA GPIOA
#define ADC_PORTB GPIOB
#define ADC_LM1_CH	GPIO5
#define ADC_LM2_CH	GPIO1
#define ADC_LM3_CH	GPIO0
#define ADC_MT1_CH	GPIO2
#define ADC_MT2_CH	GPIO3
#define ADC_MT3_CH	GPIO4

#define DMA_PRIORITY 2
#define TIMER_PRIORIY 4
#define ADC_TRIG_PRIORITY 1

#define ADC_SAMPLE 20

#define V25  1750
#define Avg_Slope  5

 
typedef enum{
	SS_LM1 = 0,
	SS_LM2,
	SS_LM3,
	SS_MT1,
	SS_MT2,
	SS_MT3,
	SS_TEMP,
	SS_VREF
}SENSOR_ID;

typedef struct
{
	uint8_t sample;
	uint8_t flag;
	uint16_t port[ADC_CHANNEL];
	uint16_t ssmt[3];
	uint16_t sslm[3];
	uint16_t temp;
	uint16_t vref;
	TIME time;
	uint16_t adcValue[ADC_SAMPLE*ADC_CHANNEL];
	uint16_t adcValueMean[ADC_SAMPLE*ADC_CHANNEL];
}ADC_STRUCT;

void sensor_init(void);
void adc_setup(void);
void dma_setup(void);
void sensor_start(void);
void sensor_stop(void);
uint8_t sensor_get_flag(void);
void get_samples(void);
void sensor_tick_setup(void);
void adc_trigger(void);
uint16_t adc_get_buff(uint8_t index, uint16_t *buff, uint8_t sample);
float sensor_get(SENSOR_ID sensor);
void sensor_manager(void);
float temperature_cal(uint32_t measure);
// uint16_t read_adc_naiive(uint8_t channel);

#endif /*__ANALOG_H__*/