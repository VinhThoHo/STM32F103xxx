#include "input.h"
#include "HD44780.h"
#include "tick.h"
#include "menu.h"
#include "debug.h"
#include "motor.h"

INPUT_STRUCT input;

void input_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOE);
	// rcc_periph_clock_enable(RCC_AFIO);
	gpio_set_mode(IN_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, IN_PIN1|IN_PIN2);
	for(input.name = 0; input.name < NUM_INPUT; input.name++)
	{
		input.ext[input.name].active = input.ext[input.name].actived = IN_RELEASE;
		input.ext[input.name].waitRelease = 0;
		input.ext[input.name].cnt = 0; 
		input.tick = 0;
	}
}

void input_get(uint8_t port_id)
{
	uint16_t inputGet;
	switch(port_id)
	{
		case 0: 
			inputGet = INPUT1();
			if(inputGet == !IN_PIN1) input.ext[port_id].active = IN_ACTIVE;
			else input.ext[port_id].active = IN_RELEASE;
			break;
		case 1: 
			inputGet = INPUT2();
			if(inputGet == !IN_PIN2) input.ext[port_id].active = IN_ACTIVE;
			else input.ext[port_id].active = IN_RELEASE;
			break;
		// case 2: 
		// 	inputGet = INPUT3();
		// 	if(inputGet == !IN_PIN3) input.ext[port_id].active = IN_ACTIVE;
		// 	else input.ext[port_id].active = IN_RELEASE;
		// 	break;
		// case 3: 
		// 	inputGet = INPUT4();
		// 	if(inputGet == !IN_PIN4) input.ext[port_id].active = IN_ACTIVE;
		// 	else input.ext[port_id].active = IN_RELEASE;
		// 	break;
		// case 4:
		// 	inputGet = INPUT5();
		// 	if(inputGet == !IN_PIN4) input.ext[port_id].active = IN_ACTIVE;
		// 	else input.ext[port_id].active = IN_RELEASE;
		// 	break;
		// case 5:
		// 	inputGet = INPUT6();
		// 	if(inputGet == !IN_PIN4) input.ext[port_id].active = IN_ACTIVE;
		// 	else input.ext[port_id].active = IN_RELEASE;
		// 	break;

	}
	if(input.ext[port_id].active != input.ext[port_id].actived)
	{
		++(input.ext[port_id].cnt);
		if(input.ext[port_id].cnt == NUM_OF_SAMPLE_INPUT)
		{
			input.ext[port_id].actived = input.ext[port_id].active;
			input.ext[port_id].cnt = 0;
		}
	}
	else
	{
		input.ext[port_id].cnt = 0;
	}
}

// uint8_t KeyPress(uint8_t id)
// {
// 	return key.ext[id].pressed;
// }

uint8_t input_process(void)
{	
	uint8_t currentState = 0;
	for(input.name = 0; input.name < NUM_INPUT; input.name++)
	{
		input_get(input.name);
		
		if(input.ext[input.name].actived == IN_ACTIVE)
		{
			if(input.ext[input.name].waitRelease == 0)
			{
				switch(input.name)
				{
					case IN1:
						currentState = IN1_ON;
						INFO("INPUT1\n");
						break;
					case IN2:
						currentState = IN2_ON;
						INFO("INPUT2\n");
						break;
					// case IN3:
					// 	currentState = IN3_ON;
					// 	INFO("INPUT3\n");
					// 	break;
					// case IN4:
					// 	currentState = IN4_ON;
					// 	INFO("INPUT4\n");
					// 	break;
					// case IN5:
					// 	currentState = IN5_ON;
					// 	INFO("INPUT5\n");
					// 	break;
					// case IN6:
					// 	currentState = IN6_ON;
					// 	INFO("INPUT6\n");
					default:
						currentState = NO_INPUT;
						break;
				}
			}
			input.ext[input.name].waitRelease = 1;
		}
		if(input.ext[input.name].actived == IN_RELEASE)
		{
			if(input.ext[input.name].waitRelease == 1)
			{	
				switch(input.name)
				{
					case IN1:
						currentState = IN1_OFF;
						break;
					case IN2:
						currentState = IN2_OFF;
						break;
					// case IN3:
					// 	currentState = IN3_OFF;
					// 	break;
					// case IN4:
					// 	currentState = IN4_OFF;
					// 	break;
					// case IN5:
					// 	currentState = IN5_OFF;
					// 	break;
					// case IN6:
					// 	currentState = IN6_OFF;
					// 	break;
					default:
						currentState = NO_INPUT;
						break;
				}
			}
			input.ext[input.name].waitRelease = 0;
		}
	}
	return currentState;
}

void input_manage(void)
{
	if(tick_expire_ms(&input.tick, 10))
	{
		switch(input_process())
		{
			case IN1_ON: 
				INFO("IN1 ON\n"); 
				break;
			case IN2_ON:
				INFO("IN2 ON\n");
				break;
			// case IN3_ON:
			// 	INFO("IN3 ON\n");
			// 	break;
			// case IN4_ON: 
			// 	INFO("IN4 ON\n"); 
			// 	break;
			// case IN5_ON: 
			// 	INFO("IN5 ON\n"); 
			// 	break;
			// case IN6_ON: 
			// 	INFO("IN6 ON\n"); 
			// 	break;
			case IN1_OFF:
				INFO("IN1 OFF \n");
				break;
			case IN2_OFF:
				INFO("IN2 OFF \n");
				break;
			// case IN3_OFF:
			// 	INFO("IN3 OFF \n");
			// 	break;
			// case IN4_OFF:
			// 	INFO("IN4 OFF \n");
			// 	break;
			// case IN5_OFF:
			// 	INFO("IN5 OFF \n");
			// 	break;
			// case IN6_OFF:
			// 	INFO("IN6 OFF \n");
			// 	break;
			default: 
				break; 
		}
	}
}
