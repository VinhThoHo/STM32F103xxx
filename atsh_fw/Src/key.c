#include "key.h"
#include "u8g2.h"
#include "main_screen.h"

KEY_STRUCT key;
extern uint8_t toggle;

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pins : PC13 PC10 PC11 PC12 */
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	for (key.name = 0; key.name < NUM_KEY; key.name++)
	{
		key.ext[key.name].press = key.ext[key.name].pressed = KEY_RELEASE;
		key.ext[key.name].waitRelease = 0;
		key.ext[key.name].cnt = 0;
	}
	key.tick = HAL_GetTick();
}

void KeyGet(uint8_t port_id)
{
	uint16_t keyGet;
	switch (port_id)
	{
	case 0:
		keyGet = key1_in;
		if (keyGet == !key1)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 1:
		keyGet = key2_in;
		if (keyGet == !key2)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 2:
		keyGet = key3_in;
		if (keyGet == !key3)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	case 3:
		keyGet = key4_in;
		if (keyGet == !key4)
			key.ext[port_id].press = KEY_PRESS;
		else
			key.ext[port_id].press = KEY_RELEASE;
		break;
	default:
		break;
	}
	if (key.ext[port_id].press != key.ext[port_id].pressed)
	{
		++(key.ext[port_id].cnt);
		if (key.ext[port_id].cnt == NUM_OF_SAMPLE_KEY)
		{
			key.ext[port_id].pressed = key.ext[port_id].press;
			key.ext[port_id].cnt = 0;
		}
	}
	else
	{
		key.ext[port_id].cnt = 0;
	}
}

BUTTON_ID KeyProcess(void)
{
	uint8_t currentState = NONE;
	for (key.name = 0; key.name < NUM_KEY; key.name++)
	{
		KeyGet(key.name);

		if (key.ext[key.name].pressed == KEY_PRESS)
		{
			if (key.ext[key.name].waitRelease == 0)
			{
				switch (key.name)
				{
				case KEY_NEON:
					currentState = NEON;
					// printf("KEY NEON\n");
					break;
				case KEY_UV:
					currentState = UV;
					// printf("KEY UV\n");
					break;
				case KEY_FAN:
					currentState = FAN;
					// printf("KEY FAN\n");
					break;
				case KEY_SOCKET:
					currentState = SOCKET;
					// printf("KEY SOCKET\n");
					break;
				default:
					currentState = NONE;
					break;
				}
			}
			key.ext[key.name].waitRelease = 1;
		}
		if (key.ext[key.name].pressed == KEY_RELEASE)
		{
			// if(key.ext[key.name].waitRelease == 1)
			// {
			// 	switch(key.name)
			// 	{
			// 		case KEY_NEON:
			// 			//HD44780_Clear();
			// 			HD44780_Puts(0,1,"MENU RELEASE");
			// 			break;
			// 		case KEY_UV:
			// 			//HD44780_Clear();
			// 			HD44780_Puts(0,1,"UP RELEASE");
			// 			break;
			// 		case KEY_FAN:
			// 			//HD44780_Clear();
			// 			HD44780_Puts(0,1,"DOWN RELEASE");
			// 			break;
			// 		case KEY_SOCKET:
			// 			//HD44780_Clear();
			// 			HD44780_Puts(0,1,"EXIT RELEASE");
			// 			break;
			// 	}
			// }
			key.ext[key.name].waitRelease = 0;
		}
	}
	return currentState;
}

void KeyManage(void)
{
	if (HAL_GetTick() - key.tick > 1)
	{
		switch (KeyProcess())
		{
		case NEON:
			toggle = 1;
			main_scr.lampStatus = ~main_scr.lampStatus;
			break;
		case UV:
			toggle = 1;
			main_scr.uvStatus = ~main_scr.uvStatus;
			break;
		case FAN:
			toggle = 1;
			main_scr.fanStatus = ~main_scr.fanStatus;
			break;
		case SOCKET:
			toggle = 1;
			main_scr.socketStatus = ~main_scr.socketStatus;
			break;
		default:
			break;
		}
		key.tick = HAL_GetTick();
	}
}