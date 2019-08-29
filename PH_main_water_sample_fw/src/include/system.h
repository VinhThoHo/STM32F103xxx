#ifndef __SYSTEM__H__
#define __SYSTEM__H__

#include "HD44780.h"
#include "debug.h"
#include "key.h"
#include "move.h"
#include "timerTick.h"
#include "tick.h"

typedef enum 
{
	STATE_IDLE,
	STATE_CONFIG,
	STATE_MOVEXY,
	STATE_SAMPLER,
} STATE_T;

typedef struct {
	TIME tick;
	STATE_T state;
	bool disp;
	TIME time_screen;
	TIME timeout;
	TIME wait_discharge_time;
	float tempGet;
	uint8_t bot;
	uint8_t num_bot;
	uint8_t flag_move;
	uint8_t x;
	uint8_t y;
	uint8_t cnt;
} SYS_STRUCT;

void sys_init(void);
void sys_manager(void);
#endif  /*__SYSTEM__H__*/