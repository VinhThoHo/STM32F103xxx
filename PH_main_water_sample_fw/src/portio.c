#include "portio.h"

PORT_CONTROL_STRUCT port;


void port_ctrl_init(void)
{
	gpio_set_mode(CTRL_PORT, GPIO_MODE_OUTPUT_50_MHZ, 
			GPIO_CNF_OUTPUT_PUSHPULL, 
			CTRL_PIN_1|CTRL_PIN_2|CTRL_PIN_3|CTRL_PIN_4|CTRL_PIN_5|CTRL_PIN_6|CTRL_PIN_7|CTRL_PIN_8);
	gpio_set_mode(BUZZER_PORT, GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL, BUZZER_PIN);
	gpio_clear(BUZZER_PORT, BUZZER_PIN);
	gpio_clear(CTRL_PORT, CTRL_PIN_1);
	gpio_clear(CTRL_PORT, CTRL_PIN_2);
	gpio_clear(CTRL_PORT, CTRL_PIN_3);
	gpio_clear(CTRL_PORT, CTRL_PIN_4);
	gpio_clear(CTRL_PORT, CTRL_PIN_5);
	gpio_clear(CTRL_PORT, CTRL_PIN_6);
	gpio_clear(CTRL_PORT, CTRL_PIN_7);
	gpio_clear(CTRL_PORT, CTRL_PIN_8);
	// tick_initTime(&port.tick_buzzer);
}

void port_ctrl_manager(void)
{
	if(!tick_checkMS(&port.tick_buzzer, 500))
	{
		BUZZER(0);
	}
}