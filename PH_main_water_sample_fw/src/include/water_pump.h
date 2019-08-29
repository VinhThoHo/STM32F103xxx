#ifndef __WATER_PUMP__H__
#define __WATER_PUMP__H__

#include "motor.h"
#include "input.h"
#include "key.h"
#include "portio.h"
#include "sensor.h"
#include "input.h"
#include "timerTick.h"
#include "debug.h"
#include "tick.h"

typedef struct {
	uint8_t flag_charge; // co bao bom nuoc vao
	uint8_t flag_balance; // co bao can bang nuoc
	uint8_t flag_discharge; // co bao xa nuoc ra
	uint8_t flag_complete; // co bao hoan thanh
	uint8_t flag_motor;  // co bao motor kep ong
	uint8_t flag_wait_discharge;
	uint32_t timeout;
	TIME tick_water;
	TIME tick_balance;
	TIME tick_discharge;
	TIME tick_wait_off_motor;
	TIME tick_water_full;
	TIME tick_trigger;
	TIME tick_wait_discharge;
} WATER_STRUCT;


extern WATER_STRUCT water;

void water_init(void);
void water_charge_start(void);
void water_charge(void);
void water_balance(void);
void water_discharge(void);
void loop_water(uint8_t input);

#endif