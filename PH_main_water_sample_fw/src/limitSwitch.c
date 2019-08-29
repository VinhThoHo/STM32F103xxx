#include "limitSwitch.h"
#include "tick.h"
#include "debug.h"

uint32_t lastInterrupt[NUM_SWITCHS];

bool bSwitchPressed[NUM_SWITCHS];
bool bEventSend[NUM_SWITCHS];


void limit_switch_config(void)
{
	rcc_periph_clock_enable(RCC_GPIOE);
	gpio_set_mode(SW_LIMIT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, X_AXIS_MIN_PIN|Y_AXIS_MIN_PIN);
	for(uint8_t i = 0; i < NUM_SWITCHS; ++i)
	{
		bSwitchPressed[i] = limit_switch_pressed(i);
		lastInterrupt[i] = 0;
		bEventSend[i] = 0;
	}
}

bool limit_switch_pressed(uint8_t sw)
{
	uint8_t val;
	switch(sw)
	{
		case LIMIT_SWITCH_X_MIN: 
			val = GPIO_IDR(SW_LIMIT_PORT)&X_AXIS_MIN_PIN;
			break;
		case LIMIT_SWITCH_Y_MIN:
			val = GPIO_IDR(SW_LIMIT_PORT)&Y_AXIS_MIN_PIN;
			break;
		default:
			return 0;
	}
	return !val;
}

void limit_switch_task(void)
{
	uint32_t now = tick_get();
	for(uint8_t i = 0; i < NUM_SWITCHS; i++)
	{
		bool cur_state;
		cur_state = limit_switch_pressed(i);
		if(cur_state != bSwitchPressed[i])
		{
			lastInterrupt[i] = now;
			bSwitchPressed[i] = cur_state;
			bEventSend[i] = 0;
		}
		else
		{
			if(cur_state && !bEventSend[i] && (now - lastInterrupt[i] >= 50))
			{
				bEventSend[i] = 1;
			}
		}
	}
}